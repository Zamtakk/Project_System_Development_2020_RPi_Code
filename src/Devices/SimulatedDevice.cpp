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
      potmeterValue(0),
      activeLed(1)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICEINFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
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
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"led1State", led1State},
        {"led2State", led2State},
        {"led3State", led3State}};

    return deviceInfo.dump();
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((SimulatedDeviceCommands)jsonMessage["command"])
    {
    case SIMULATED_BUTTON1_CHANGE:
        buttonPress(1, (bool)jsonMessage["value"]);
        break;
    case SIMULATED_BUTTON2_CHANGE:
        buttonPress(2, (bool)jsonMessage["value"]);
        break;
    case SIMULATED_LED1_CHANGE:
        ledStateUpdate(1, (int)jsonMessage["value"]);
        break;
    case SIMULATED_LED2_CHANGE:
        ledStateUpdate(2, (int)jsonMessage["value"]);
        break;
    case SIMULATED_LED3_CHANGE:
        ledStateUpdate(3, (int)jsonMessage["value"]);
        break;
    case SIMULATED_POTMETER_CHANGE:
        potmeterChange((int)jsonMessage["value"]);
        break;
    default:
        break;
    }
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool SimulatedDevice::isLedOn(int ledNr)
{
    if (ledNr == 1)
    {
        return (led1State > 10);
    }
    else if (ledNr == 2)
    {
        return (led2State > 10);
    }
    else if (ledNr == 3)
    {
        return (led3State > 10);
    }
    else
    {
        return false;
    }
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::buttonPress(int buttonNr, bool buttonPressed)
{
    if (buttonNr == 1 && buttonPressed == true)
    {
        activeLed++;
        if (activeLed > 3)
        {
            activeLed = 1;
        }
    }
    else if (buttonNr == 2 && buttonPressed == true)
    {
        activeLed--;
        if (activeLed < 1)
        {
            activeLed = 3;
        }
    }
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool SimulatedDevice::ledStateUpdate(int ledNr, int value)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::potmeterChange(int value)
{
}