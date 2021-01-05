#include "Devices/Chair.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::to_string;

/*!
    @brief Constructor for the Chair object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
Chair::Chair(string uuid, string type, SocketServer *server) : Device(uuid, type, server), vibratorState(false), buttonPressed(false), pressureValue(0)
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
		PressureSensorChange(message);
	}
	else if (jsonMessage["command"] == CHAIR_BUTTON_CHANGE)
	{
		ButtonPressed(message);
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
    @param[in] message The JSON message in string format.
*/
void Chair::PressureSensorChange(string message)
{
	json jsonMessage = json::parse(message);
	//TODO: use vector with devices in stead of hardcoded website uuid
	socketServer->SendMessage("0000000001", message);
	string value = jsonMessage["value"];
	pressureValue = stoi(value, nullptr, 10);
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] message The JSON message in string format.
*/
void Chair::ButtonPressed(string message)
{
	json jsonMessage = json::parse(message);
	if (jsonMessage["value"] == 0)
	{
		buttonPressed = false;
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\": 4002, \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		vibratorState = false;
	}
	else if (jsonMessage["value"] == 1)
	{
		buttonPressed = true;
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":4002,\"value\": \"1\"}";
		cout << sendmessage << endl;
		socketServer->SendMessage(uuid, sendmessage);
		vibratorState = true;
	}
}