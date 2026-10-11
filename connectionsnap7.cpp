#include "connectionsnap7.h"
#include "indilogger.h"

#include <cstring>

namespace Connection
{
    extern const char *CONNECTION_TAB;

    INDISnap::INDISnap(INDI::DefaultDevice *dev) : Interface(dev, CONNECTION_CUSTOM)
    {
        char defaultHostname[MAXINDINAME] = {0};
        double defaultLocalTsap = 0x000;
        double defaultRemoteTsap = 0x000;

        IUFillText(&AddressT[0], "HOST", "Host address", defaultHostname);
        IUFillTextVector(&AddressTP, AddressT, 1, getDeviceName(), "SNAP7_ADDRESS", "PLC address", CONNECTION_TAB,
                         IP_RW, 60, IPS_IDLE);

        IUFillNumber(&TsapN[0], "LOCALTSAP", "Local TSAP", "%#x", MIN_TSAP, MAX_TSAP, 1, defaultLocalTsap);
        IUFillNumber(&TsapN[1], "REMOTETSAP", "Remote TSAP", "%#x", MIN_TSAP, MAX_TSAP, 1, defaultRemoteTsap);
        IUFillNumberVector(&TsapNP, TsapN, 2, getDeviceName(), "SNAP7_TSAP", "PLC TSAP", CONNECTION_TAB,
                           IP_RW, 60, IPS_IDLE);
    }

    bool INDISnap::Connect()
    {
        if (AddressT[0].text == nullptr || AddressT[0].text[0] == '\0')
        {
            LOG_ERROR("Error! Server address is missing or invalid.");
            return false;
        }
        const char *hostname = AddressT[0].text;

        const word localTsap = static_cast<word>(TsapN[0].value);
        const word remoteTsap = static_cast<word>(TsapN[1].value);

        LOGF_INFO("Connecting to %s with Local TSAP %#x and Remote TSAP %#x", hostname, localTsap, remoteTsap);


        S7Client = new TS7Client;
        if (S7Client->SetConnectionParams(hostname, localTsap, remoteTsap) != 0)
        {
            LOG_ERROR("Invalid parameters");
            delete S7Client;
            S7Client = nullptr;
            return false;
        }
        int rc = S7Client->Connect();
        if (rc != 0)
        {
            LOG_ERROR("Something went wrong while connecting.");
            delete S7Client;
            S7Client = nullptr;
            return false;
            //TODO: Implement correct error handling
        }

        bool handshakeRc = Handshake();
        if (handshakeRc)
        {
            LOGF_INFO("%s is online.", getDeviceName());
        }
        else
        {
            LOG_ERROR("Handshake failed.");
        }
        return handshakeRc;
    }

    bool INDISnap::Disconnect()
    {
        if(S7Client == nullptr || S7Client->Connected() == 0)
        {
            LOG_ERROR("Can't disconnect from device, that isn't connected.");
            return false;
        }

        int rc = S7Client->Disconnect();
        if (rc != 0)
        {
            LOG_ERROR("Something went wrong while disconnecting");
            return false;
        }

        return true;
    }

    void INDISnap::Activated()
    {
        m_Device->defineProperty(&AddressTP);
        m_Device->defineProperty(&TsapNP);
    }

    void INDISnap::Deactivated()
    {
        m_Device->deleteProperty(AddressTP.name);
        m_Device->deleteProperty(TsapNP.name);
    }

    bool INDISnap::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
    {
        if (dev != nullptr && std::strcmp(dev, getDeviceName()) != 0)
            return false;

        if (std::strcmp(name, AddressTP.name) != 0)
            return false;

        for (int i = 0; i < n; ++i)
        {
            IText *text = IUFindText(&AddressTP, names[i]);
            if (text != nullptr)
                IUSaveText(text, texts[i]);
        }

        AddressTP.s = IPS_OK;
        IDSetText(&AddressTP, "Snap7 address updated.");
        return true;
    }

    bool INDISnap::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
    {
        if (dev != nullptr && std::strcmp(dev, getDeviceName()) != 0)
            return false;

        if (std::strcmp(name, TsapNP.name) != 0)
            return false;

        for (int i = 0; i < n; ++i)
        {
            INumber *number = IUFindNumber(&TsapNP, names[i]);
            if (number != nullptr)
                number->value = values[i];
        }

        TsapNP.s = IPS_OK;
        IDSetNumber(&TsapNP, "Snap7 TSAP values updated.");
        return true;
    }

    bool INDISnap::saveConfigItems(FILE *fp)
    {
        IUSaveConfigText(fp, &AddressTP);
        IUSaveConfigNumber(fp, &TsapNP);
        return true;
    }

    void INDISnap::setDefaultHost(const char *host)
    {
        IUSaveText(&AddressT[0], host);
    }

    void INDISnap::setDefaultLocalTsap(double localTsap)
    {
        TsapN[0].value = localTsap;
    }

    void INDISnap::setDefaultRemoteTsap(double remoteTsap)
    {
        TsapN[1].value = remoteTsap;
    }

    bool INDISnap::writeData(int position, uint8_t * value)
    {
        byte buffer[1];
        buffer[0] = *value;

        if (S7Client == nullptr)
            return false;

        int rc = S7Client->DBWrite(1, position, 1, buffer);
        if (rc != 0)
            return false;

        return true;

    }

    bool INDISnap::writeData(int position, uint8_t *value, int millis)
    {
        writeData(position, value);
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
        uint8_t resetValue = (*value == 0) ? 1 : 0;
        writeData(position, &resetValue);
        return true;
    }

    bool INDISnap::readData(int position, uint8_t *value)
    {
        byte buffer[1] = {0};

        if (S7Client == nullptr)
            return false;

        int rc = S7Client->DBWrite(1, position, 1, buffer);
        if (rc != 0)
            return false;
        *value = buffer[0];
        return true;
    }

}
