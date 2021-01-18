// Includes

#include "CommandTypes.hpp"

#include "Devices/Chair.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;
using std::to_string;

// Function definitions

/*!
    @brief Constructor for the Chair object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Chair::Chair(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
	: Device(uuid, type, server, devices),
	  ledOn(false),
	  vibratorOn(false),
	  pressureValue(0)
{
	json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
	jsonMessage["value"] = "";
	socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Chair objects
*/
Chair::~Chair()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Chair::GetDeviceInfo()
{
	json deviceInfo = {
		{"UUID", uuid},
		{"Type", type},
		{"Status", status},
		{"CHAIR_LED_ON", ledOn},
		{"CHAIR_VIBRATOR_ON", vibratorOn},
		{"CHAIR_PRESSURE_SENSOR_VALUE", pressureValue}};

	return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Chair::HandleMessage(string message)
{
	json jsonMessage = json::parse(message);

	switch ((int)jsonMessage["command"])
	{
	case DEVICE_INFO:
	{
		ledOn = (bool)jsonMessage["CHAIR_LED_ON"];
		vibratorOn = (bool)jsonMessage["CHAIR_VIBRATOR_ON"];
		pressureValue = (uint8_t)jsonMessage["CHAIR_PRESSURE_SENSOR_VALUE"];

		updateWebsite();
		break;
	}
	case CHAIR_BUTTON_PRESSED:
	{
		buttonWasPressed((bool)jsonMessage["value"]);
		break;
	}
	case CHAIR_VIBRATOR_ON:
	{
		turnVibratorOn((bool)jsonMessage["value"]);
		break;
	}
	case CHAIR_PRESSURE_SENSOR_VALUE:
	{
		newPressureSensorValue((uint8_t)jsonMessage["value"]);
		break;
	}
    case HEARTBEAT:
    {
        status = (DeviceStatus)jsonMessage["heartbeat"]["status"];

        updateWebsite();
    }
	default:
		break;
	}
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Chair::buttonWasPressed(bool buttonPressed)
{
	if (vibratorOn && buttonPressed)
	{
		turnVibratorOn(false);
	}
	else if (!vibratorOn && buttonPressed)
	{
		turnVibratorOn(true);
	}
}

/*!
    @brief Turns the LED on or off
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
void Chair::turnLedOn(bool p_ledOn)
{
	ledOn = p_ledOn;
	json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_LED_ON));
	jsonMessage["value"] = ledOn;
	socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Turns the vibrator on or off
    @param[in] p_vibratorOn A boolean stating if the vibrator should turn on (true) or off (false)
*/
void Chair::turnVibratorOn(bool p_vibratorOn)
{
	turnLedOn(p_vibratorOn);
	vibratorOn = p_vibratorOn;

	json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_VIBRATOR_ON));
	jsonMessage["value"] = vibratorOn;

	socketServer->SendMessage(uuid, jsonMessage.dump());

	Device *website = getDeviceByType("Website");
	if (website == nullptr)
		return;

	socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages concerning changes in the pressure sensor.
    @param[in] p_pressureValue The new pressure value.
*/
void Chair::newPressureSensorValue(uint8_t p_pressureValue)
{
	pressureValue = p_pressureValue;

	Device *website = getDeviceByType("Website");
	if (website == nullptr)
	{
		return;
	}

	json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_PRESSURE_SENSOR_VALUE));
	jsonMessage["value"] = pressureValue;
	socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}
