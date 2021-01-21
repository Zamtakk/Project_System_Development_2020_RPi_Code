// Includes

#include "CommandTypes.hpp"

#include "Devices/SimulatedDevice.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;

// Function definitions

/*!
    @brief Constructor for the SimulatedDevice object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
SimulatedDevice::SimulatedDevice(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      led1Value(0),
      led2Value(0),
      led3Value(0),
      activeLed(1)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for SimulatedDevice objects
*/
SimulatedDevice::~SimulatedDevice()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string SimulatedDevice::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"SIMULATED_LED1_VALUE", led1Value},
        {"SIMULATED_LED2_VALUE", led2Value},
        {"SIMULATED_LED3_VALUE", led3Value}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void SimulatedDevice::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        led1Value = (int)jsonMessage["SIMULATED_LED1_VALUE"];
        led2Value = (int)jsonMessage["SIMULATED_LED2_VALUE"];
        led3Value = (int)jsonMessage["SIMULATED_LED3_VALUE"];

        updateWebsite();
        break;
    }
    case SIMULATED_BUTTON1_PRESSED:
    {
        buttonWasPressed(1, (bool)jsonMessage["value"]);
        break;
    }
    case SIMULATED_BUTTON2_PRESSED:
    {
        buttonWasPressed(2, (bool)jsonMessage["value"]);
        break;
    }
    case SIMULATED_LED1_VALUE:
    {
        turnLedOn(1, (int)jsonMessage["value"]);
        break;
    }
    case SIMULATED_LED2_VALUE:
    {
        turnLedOn(2, (int)jsonMessage["value"]);
        break;
    }
    case SIMULATED_LED3_VALUE:
    {
        turnLedOn(3, (int)jsonMessage["value"]);
        break;
    }
    case SIMULATED_DIMMER_VALUE:
    {
        newDimmerValue((int)jsonMessage["value"]);
        break;
    }
    case HEARTBEAT:
    {
        status = (DeviceStatus)jsonMessage["heartbeat"]["status"];

        updateWebsite();
        break;
    }
    default:
        break;
    }
}

/*!
    @brief Handles the logic of a button press event
    @param[in] buttonNr An indicator defining which button was pressed
    @param[in] buttonPressed A boolean stating if the button was pressed (true) or released (false)
*/
void SimulatedDevice::buttonWasPressed(int buttonNr, bool buttonPressed)
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
    @brief Turns the specified LED on or off
    @param[in] ledNr An indicator defining which LED should change
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
bool SimulatedDevice::turnLedOn(int ledNr, int value)
{
    json jsonMessage = json::parse(newMessage(uuid, type, 0));

    if (ledNr == 1)
    {
        jsonMessage["command"] = SIMULATED_LED1_VALUE;
        led1Value = value;
    }
    else if (ledNr == 2)
    {
        jsonMessage["command"] = SIMULATED_LED2_VALUE;
        led2Value = value;
    }
    else if (ledNr == 3)
    {
        jsonMessage["command"] = SIMULATED_LED3_VALUE;
        led3Value = value;
    }
    else
    {
        return false;
    }

    jsonMessage["value"] = value;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return false;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());

    return true;
}

/*!
    @brief A handler for new dimmer values, updates the selected led with a new value
    @param[in] value The new value of the dimmer
*/
void SimulatedDevice::newDimmerValue(int value)
{
    turnLedOn(activeLed, value);
}
