#include "Devices/Column.hpp"
#include "CommandTypes.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;
using std::to_string;

/*!
    @brief Constructor for the Column object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
Column::Column(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      buzzerState(false),
      ledState(false),
      gasSensorValue(0),
      gasSensorTreshold(20)
{
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
        {"buzzerState", buzzerState},
        {"ledState", ledState},
        {"gasSensorValue", gasSensorValue},
        {"gasSensorTreshold", gasSensorTreshold}};

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
    case DEVICEINFO:
    {
        buzzerState = (bool)jsonMessage["buzzerState"];
        ledState = (bool)jsonMessage["ledState"];
        gasSensorValue = (int)jsonMessage["gasSensorValue"];

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
        break;
    }
    case COLUMN_GASSENSOR_CHANGE:
    {
        gasSensorChange((int)jsonMessage["value"]);
        break;
    }
    case COLUMN_BUTTON_CHANGE:
    {
        buttonPress((bool)jsonMessage["value"]);
        break;
    }
    case COLUMN_GASTRESHOLD_CHANGE:
    {
        changeGasTreshold((int)jsonMessage["value"]);
        break;
    }

    default:
        break;
    }
}

/*!
    @brief Getter for the led state
    @returns A boolean to check whether led is on or off
*/
bool Column::IsLedOn()
{
    return ledState;
}

void Column::BuzzerStateOn(bool stateOn)
{
    buzzerState = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_BUZZER_CHANGE));
    jsonMessage["value"] = buzzerState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Function to handle changes to the treshold for the gas sensor
    @param[in] value The value to which the treshold should be changed.
*/
void Column::changeGasTreshold(int value)
{
    gasSensorTreshold = value;

    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_GASTRESHOLD_CHANGE));
    jsonMessage["value"] = gasSensorTreshold;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Column::gasSensorChange(int value)
{
    gasSensorValue = value;
    if (gasSensorValue > gasSensorTreshold)
    {
        BuzzerStateOn(true);
    }

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }

    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_GASSENSOR_CHANGE));
    jsonMessage["value"] = gasSensorValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Column::buttonPress(bool buttonPressed)
{
    if (ledState && buttonPressed && !buzzerState)
    {
        ledStateOn(false);
    }
    else if (!ledState && buttonPressed && !buzzerState)
    {
        ledStateOn(true);
    }

    if (buzzerState && buttonPressed)
    {
        BuzzerStateOn(false);
    }
}

/*!
    @brief Function to turn the led on or off
    @param[in] stateOn Boolean on whether led needs to be on or off
*/
void Column::ledStateOn(bool stateOn)
{
    ledState = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, COLUMN_LED_CHANGE));
    jsonMessage["value"] = ledState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}