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
      led1Value(0),
      led2Value(0),
      led3Value(0),
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
        {"led1Value", led1Value},
        {"led2Value", led2Value},
        {"led3Value", led3Value}};

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
        ledValueUpdate(1, (int)jsonMessage["value"]);
        break;
    case SIMULATED_LED2_CHANGE:
        ledValueUpdate(2, (int)jsonMessage["value"]);
        break;
    case SIMULATED_LED3_CHANGE:
        ledValueUpdate(3, (int)jsonMessage["value"]);
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
        return (led1Value > 10);
    }
    else if (ledNr == 2)
    {
        return (led2Value > 10);
    }
    else if (ledNr == 3)
    {
        return (led3Value > 10);
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
bool SimulatedDevice::ledValueUpdate(int ledNr, int value)
{
    json jsonMessage = json::parse(newMessage(uuid, type, 0));

    if (ledNr == 1)
    {
        jsonMessage["command"] = SIMULATED_LED1_CHANGE;
        led1Value = value;
    }
    else if (ledNr == 2)
    {
        jsonMessage["command"] = SIMULATED_LED2_CHANGE;
        led2Value = value;
    }
    else if (ledNr == 3)
    {
        jsonMessage["command"] = SIMULATED_LED3_CHANGE;
        led3Value = value;
    }
    else
    {
        return false;
    }
    
    jsonMessage["value"] = value;
    socketServer->SendMessage(uuid, jsonMessage.dump());
    return true;
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void SimulatedDevice::potmeterChange(int value)
{
    ledValueUpdate(activeLed, value);
}