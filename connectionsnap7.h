#pragma once

#include "connectionplugins/connectioninterface.h"

#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "snap7.h"

#define MIN_TSAP 0
#define MAX_TSAP 0xfff

namespace Connection
{
    class INDISnap : public Interface
    {
    public:
        INDISnap(INDI::DefaultDevice *dev);
        virtual ~INDISnap() = default;

        virtual bool Connect() override;
        virtual bool Disconnect() override;

        virtual void Activated() override;
        virtual void Deactivated() override;
        virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;
        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
        virtual bool saveConfigItems(FILE *fp) override;

        virtual std::string name() override
        {
            return "CONNECTION_SNAP7";
        }

        virtual std::string label() override
        {
            return "Snap7";
        }

        void setDefaultHost(const char *host);
        void setDefaultLocalTsap(double localTsap);
        void setDefaultRemoteTsap(double remoteTsap);
        bool writeData(int position, uint8_t * value);
        bool writeData(int position, uint8_t *value, int millis);
        bool readData(int position, uint8_t * value);

    protected:


        ITextVectorProperty AddressTP{};
        IText AddressT[1] {};

        INumberVectorProperty TsapNP{};
        INumber TsapN[2] {};

        TS7Client *S7Client{};

    };
}
