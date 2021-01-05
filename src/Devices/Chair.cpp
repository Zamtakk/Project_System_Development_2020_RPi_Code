#include "Devices/Chair.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::string;
using std::to_string;

/*!
    @brief Constructor for the Chair object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
Chair::Chair(string uuid, string type, SocketServer *server) : Device(uuid, type, server), vibratorState(false), buttonPressed(false), ledState(false), pressureValue(0)
{
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
		{"vibratorState", vibratorState},
		{"buttonPressed", buttonPressed},
		{"ledState", ledState},
		{"pressureValue", pressureValue}};

	return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Chair::HandleMessage(string message)
{
	json jsonMessage = json::parse(message);
	if (jsonMessage["command"] == CHAIR_FORCESENSOR_CHANGE)
	{
		PressureSensorChange(jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == CHAIR_BUTTON_CHANGE)
	{
		ButtonPressed(jsonMessage["value"]);
	}
	socketServer->SendMessage(uuid, message);
}

/*!
    @brief Getter for the vibrator state
    @returns A boolean to check whether vibrator is on or off
*/
bool Chair::IsVibratorOn()
{
	return vibratorState;
}

/*!
    @brief Function to handle incoming messages concerning changes in the pressure sensor.
    @param[in] pressureValueReceived The pressure value in the message.
*/
void Chair::PressureSensorChange(int pressureValueReceived)
{
	//TODO: use vector with devices in stead of hardcoded website uuid
	string sendmessage = "{\"UUID\":\"0000000001\",\"Type\":\"" + type + "\",\"command\":" + to_string(CHAIR_FORCESENSOR_CHANGE) + ",\"value\":" + to_string(pressureValueReceived) + "}";
	socketServer->SendMessage("0000000001", sendmessage);
	pressureValue = pressureValueReceived;
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Chair::ButtonPressed(bool buttonPressed)
{
	if (buttonPressed)
	{
		buttonPressed = true;
		LedStateOn(true);
		VibratorStateOn(true);
	}
	else
	{
		buttonPressed = false;
		LedStateOn(false);
		VibratorStateOn(false);
	}
}

/*!
    @brief Function to turn the led on or off
    @param[in] stateOn Boolean on whether led needs to be on or off
*/
void Chair::LedStateOn(bool stateOn)
{
	if (stateOn)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(CHAIR_LED_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledState = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(CHAIR_LED_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledState = false;
	}
}

/*!
    @brief Function to turn the vibrator on or off
    @param[in] stateOn Boolean on whether vibrator needs to be on or off
*/
void Chair::VibratorStateOn(bool stateOn)
{
	if (stateOn)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(CHAIR_VIBRATOR_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		vibratorState = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(CHAIR_VIBRATOR_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		vibratorState = false;
	}
}