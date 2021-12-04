#pragma once

#include "libindi/indidome.h"
#include "connectionsnap7.h"

namespace Connection
{
    class INDISnap;
}

class GersbachRoof : public INDI::Dome
{
public:
    GersbachRoof();
    virtual ~GersbachRoof() = default;

    virtual const char *getDefaultName() override;

    virtual bool initProperties() override;
    virtual bool updateProperties() override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;

    //virtual void TimerHit() override;

protected:
    virtual bool Handshake() override;

    virtual bool Abort() override;
    virtual IPState Park() override;
    virtual IPState UnPark() override;

    virtual IPState ControlShutter(ShutterOperation operation) override;
    Connection::INDISnap *snapConnection{nullptr};
    uint8_t hi = 1;
    uint8_t lo = 0;

};