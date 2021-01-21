// Includes

#include "CommandTypes.hpp"

#include "Devices/Column.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;
using std::to_string;

// Function definitions

/*!
    @brief Constructor for the Column object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Column::Column(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      buzzerOn(false),
      ledOn(false),
      smokeValue(0),
      smokeTreshold(240)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Column objects
*/
Column::~Column()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Column::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"COLUMN_LED_ON", ledOn},
        {"COLUMN_BUZZER_ON", buzzerOn},
        {"COLUMN_SMOKE_SENSOR_VALUE", smokeValue},
        {"COLUMN_SMOKE_TRESHOLD_VALUE", smokeTreshold}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Column::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        ledOn = (bool)jsonMessage["COLUMN_LED_ON"];
        buzzerOn = (bool)jsonMessage["COLUMN_BUZZER_ON"];
        smokeValue = (int)jsonMessage["COLUMN_SMOKE_SENSOR_VALUE"];

        updateWebsite();
        break;
    }
    case COLUMN_BUTTON_PRESSED:
    {
        buttonWasPressed((bool)jsonMessage["value"]);
        break;
    }
    case COLUMN_LED_ON:
    {
        turnLedOn((bool)jsonMessage["value"]);
        turnBuzzerOn((bool)jsonMessage["value"]);
        break;
    }
    case COLUMN_BUZZER_ON:
    {
        turnBuzzerOn((bool)jsonMessage["value"]);
        break;
    }
    case COLUMN_SMOKE_SENSOR_VALUE:
    {
        newSmokeSensorValue((int)jsonMessage["value"]);
        break;
    }
    case COLUMN_SMOKE_TRESHOLD_VALUE:
    {
        newSmokeTresholdValue((int)jsonMessage["value"]);
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

void Column::newSmokeSensorValue(int value)
{
    smokeValue = value;
    if (smokeValue > smokeTreshold)
    {
        turnBuzzerOn(true);
        turnLedOn(true);
    }

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_SMOKE_SENSOR_VALUE));
    jsonMessage["value"] = smokeValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Column::buttonWasPressed(bool buttonPressed)
{
    if (buttonPressed && !ledOn)
    {
        turnLedOn(true);
    }
}

/*!
    @brief Turns the LED on or off
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
void Column::turnLedOn(bool p_ledOn)
{
    ledOn = p_ledOn;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_LED_ON));
    jsonMessage["value"] = ledOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Turns the buzzer on or off
    @param[in] p_buzzerOn A boolean stating if the buzzer should turn on (true) or off (false)
*/
void Column::turnBuzzerOn(bool p_buzzerOn)
{
    buzzerOn = p_buzzerOn;
    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_BUZZER_ON));
    jsonMessage["value"] = buzzerOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Updates the treshold value for the smoke sensor
    @param[in] value The new treshold value
*/
void Column::newSmokeTresholdValue(int value)
{
    std::cout << "New smoke value " << value << std::endl;
    smokeTreshold = value;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_SMOKE_TRESHOLD_VALUE));
    jsonMessage["value"] = smokeTreshold;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}