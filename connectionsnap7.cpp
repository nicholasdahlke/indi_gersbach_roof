#include "connectionsnap7.h"
#include "indilogger.h"

namespace Connection
{
    extern const char *CONNECTION_TAB;

    INDISnap::INDISnap(INDI::DefaultDevice *dev) : Interface(dev, CONNECTION_CUSTOM)
    {
        char defaultHostname[MAXINDINAME] = {0};
        double defaultLocalTsap = 0x000;
        double defaultRemoteTsap = 0x000;

        IUFillText(&AdressT[0], "HOST", "Host address", defaultHostname);
        IUFillNumber(&LocalTsapN[0], "LOCALTSAP", "Local TSAP", "%#x", MIN_TSAP, MAX_TSAP, 1, defaultLocalTsap);
        IUFillNumber(&RemoteTsapN[0], "REMOTETSAP", "Remote TSAP", "%#x", MIN_TSAP, MAX_TSAP, 1, defaultRemoteTsap);
    }

    bool INDISnap::Connect()
    {
        if (AdressT[0].text == nullptr || AdressT[0].text[0] == '\0')
        {
            LOG_ERROR("Error! Server address is missing or invalid.");
            return false;
        }
        const char *hostname = AdressT[0].text;

        const double localTsap = LocalTsapN[0].value;
        const double remoteTsap = RemoteTsapN[0].value;

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
        m_Device->defineProperty(&AdressTP);
        m_Device->defineProperty(&LocalTsapNP);
        m_Device->defineProperty(&RemoteTsapNP);
    }

    void INDISnap::Deactivated()
    {
        m_Device->deleteProperty(AdressTP.name);
        m_Device->deleteProperty(RemoteTsapNP.name);
        m_Device->deleteProperty(LocalTsapNP.name);
    }

    void INDISnap::setDefaultHost(const char *host)
    {
        IUSaveText(&AdressT[0], host);
    }

    void INDISnap::setDefaultLocalTsap(double localTsap)
    {
        //TODO: Implement this thing
    }

    void INDISnap::setDefaultRemoteTsap(double remoteTsap)
    {
        //TODO: Implementation missing
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