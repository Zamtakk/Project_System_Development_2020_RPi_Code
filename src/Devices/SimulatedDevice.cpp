#include "Devices/SimulatedDevice.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;

/*!
    @brief 
    @param[in] 
    @return 
*/
SimulatedDevice::SimulatedDevice(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      led1State(false),
      led2State(false),
      led3State(false),
      potmeterValue(0)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
SimulatedDevice::~SimulatedDevice()
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
string SimulatedDevice::GetDeviceInfo()
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::HandleMessage(string message)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool SimulatedDevice::isLedOn(int ledNr)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::buttonPress(int buttonNr, bool buttonPressed)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool SimulatedDevice::ledStateOn(int ledNr, bool stateOn)
{
}