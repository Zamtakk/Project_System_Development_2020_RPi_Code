// Includes

#include "CommandTypes.hpp"

#include "Devices/Bed.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;
using std::to_string;

// Function definitions

/*!
    @brief Constructor for the Bed object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Bed::Bed(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledOn(false),
      pressureValue(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
	jsonMessage["value"] = "";
	socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Bed objects
*/
Bed::~Bed()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Bed::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"BED_LED_ON", ledOn},
        {"BED_PRESSURE_SENSOR_VALUE", pressureValue}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Bed::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

	switch ((int)jsonMessage["command"])
	{
	case DEVICE_INFO:
	{
		ledOn = (bool)jsonMessage["BED_LED_ON"];
		pressureValue = (int)jsonMessage["BED_PRESSURE_SENSOR_VALUE"];

		updateWebsite();
		break;
	}
	case BED_BUTTON_PRESSED:
	{
		buttonWasPressed((bool)jsonMessage["value"]);
		break;
	}
	case BED_LED_ON:
	{
		turnLedOn((bool)jsonMessage["value"]);
		break;
	}
	case BED_PRESSURE_SENSOR_VALUE:
	{
		newPressureSensorValue((uint16_t)jsonMessage["value"]);
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
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Bed::buttonWasPressed(bool buttonPressed)
{
    if (ledOn && buttonPressed)
    {
        turnLedOn(false);
    }
    else if (!ledOn && buttonPressed)
    {
        turnLedOn(true);
    }
}

/*!
    @brief Function to turn the led on or off
    @param[in] stateOn Boolean on whether led needs to be on or off
*/
void Bed::turnLedOn(bool p_ledOn)
{
    ledOn = p_ledOn;
    json jsonMessage = json::parse(newMessage(uuid, type, BED_LED_ON));
    jsonMessage["value"] = ledOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages concerning changes in the pressure sensor.
    @param[in] pressureValueReceived The pressure value in the message.
*/
void Bed::newPressureSensorValue(uint16_t p_pressureValue)
{
    pressureValue = p_pressureValue;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }

    json jsonMessage = json::parse(newMessage(uuid, type, BED_PRESSURE_SENSOR_VALUE));
    jsonMessage["value"] = pressureValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}
