#include "config.h"
#include "indi_gersbach_roof.h"
#include "libindi/indicom.h"

static std::unique_ptr<GersbachRoof> driver(new GersbachRoof());

GersbachRoof::GersbachRoof()
{
    setVersion(CDRIVER_VERSION_MAJOR, CDRIVER_VERSION_MINOR);

    setDomeConnection(CONNECTION_NONE);
    SetDomeCapability(
            DOME_CAN_ABORT |
            DOME_CAN_PARK |
            DOME_HAS_SHUTTER
            );
}

const char *GersbachRoof::getDefaultName()
{
    return "Dach Sternwarte Gersbach";
}

bool GersbachRoof::initProperties()
{
    INDI::Dome::initProperties();
    addAuxControls();

    snapConnection = new Connection::INDISnap(this);
    snapConnection->registerHandshake([&]() { return  Handshake(); });
    registerConnection(snapConnection);

    return true;
}

void GersbachRoof::ISGetProperties(const char *dev)
{
    INDI::Dome::ISGetProperties(dev);
}

bool GersbachRoof::updateProperties()
{
    return INDI::Dome::updateProperties();
}

bool GersbachRoof::ISNewSwitch(const char *dev, const char *name, ISState *states, char **names, int n)
{
    return INDI::Dome::ISNewSwitch(dev, name, states, names, n);
}

bool GersbachRoof::Handshake()
{
    snapConnection->writeData(1, &hi);
    uint8_t buffer = {0};
    snapConnection->readData(2, &buffer);
    snapConnection->writeData(1, &lo);
    if (buffer == 1)
        return true;
    return false;
}

IPState GersbachRoof::Park()
{
    snapConnection->writeData(3, &hi, 10);
    return IPS_BUSY;
}

IPState GersbachRoof::UnPark()
{
    snapConnection->writeData(5, &hi, 10);
    return IPS_BUSY;
}

bool GersbachRoof::Abort()
{
    snapConnection->writeData(4, &hi, 10);
    return true;
}

IPState GersbachRoof::ControlShutter(ShutterOperation operation)
{
    switch (operation) {
        case INDI::Dome::SHUTTER_OPEN:
            snapConnection->writeData(7, &hi, 10);
            break;
        case INDI::Dome::SHUTTER_CLOSE:
            snapConnection->writeData(8, &hi, 10);
            break;
    }
    return IPS_BUSY;
}