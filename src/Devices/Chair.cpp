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
Chair::Chair(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices), vibratorState(false), ledState(false), pressureValue(0)
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
		pressureSensorChange((int)jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == CHAIR_BUTTON_CHANGE)
	{
		buttonPress((bool)jsonMessage["value"]);
	}
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
void Chair::pressureSensorChange(int pressureValueReceived)
{
	pressureValue = pressureValueReceived;

	map<string, Device *>::iterator it;

    for (it = devices->begin(); it != devices->end(); it++)
    {
        if (it->second->GetType() == "Website")
        {
            string senduuid = it->second->GetUUID();
			json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_FORCESENSOR_CHANGE));
			jsonMessage["value"] = pressureValueReceived;
			socketServer->SendMessage(senduuid, jsonMessage.dump());
			break;
        }
    }
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Chair::buttonPress(bool buttonPressed)
{
	ledStateOn(buttonPressed);
	vibratorStateOn(buttonPressed);
}

/*!
    @brief Function to turn the led on or off
    @param[in] stateOn Boolean on whether led needs to be on or off
*/
void Chair::ledStateOn(bool stateOn)
{
	if (stateOn)
	{
		if (ledState)
		{
			ledState = false;
		}
		else
		{
			ledState = true;
		}
		json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_LED_CHANGE));
		jsonMessage["value"] = ledState;

		socketServer->SendMessage(uuid, jsonMessage.dump());
	}
}

/*!
    @brief Function to turn the vibrator on or off
    @param[in] stateOn Boolean on whether vibrator needs to be on or off
*/
void Chair::vibratorStateOn(bool stateOn)
{
	if (stateOn)
	{
		if (vibratorState)
		{
			vibratorState = false;
		}
		else
		{
			vibratorState = true;
		}
		json jsonMessage = json::parse(newMessage(uuid, type, CHAIR_VIBRATOR_CHANGE));
		jsonMessage["value"] = vibratorState;

		socketServer->SendMessage(uuid, jsonMessage.dump());
	}
}