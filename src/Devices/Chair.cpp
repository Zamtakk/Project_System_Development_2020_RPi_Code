#include "Devices/Chair.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;
using std::to_string;

Chair::Chair(string uuid, string type, SocketServer *server) : Device(uuid, type, server), ledState(false), vibratorState(false), buttonPressed(false), pressureValue(0)
{
}

Chair::~Chair()
{
}

string Chair::GetDeviceInfo()
{
	json deviceInfo = {
		{"UUID", uuid},
		{"Type", type},
		{"Status", status},
		{"ledState", ledState},
		{"vibratorState", vibratorState},
		{"buttonPressed", buttonPressed},
		{"pressureValue", pressureValue}};

	return deviceInfo.dump();
}

void Chair::HandleMessage(string message)
{
	socketServer->SendMessage(uuid, message);
}

bool Chair::IsLedOn()
{
	return ledState;
}

bool Chair::IsVibratorOn()
{
	return vibratorState;
}

void Chair::PressureSensorChange()
{
	string message = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\": 4000, \"Value\": \"" + to_string(pressureValue) + "\"}";
	socketServer->SendMessage(uuid, message);
}

void Chair::ButtonPressed()
{
	if (buttonPressed)
	{
		buttonPressed = false;
	}
	else
	{
		buttonPressed = true;
	}

	string message = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\": 4001, \"Value\": \"\"}";
	socketServer->SendMessage(uuid, message);
}

bool Chair::LedStateOn()
{
	return true;
}

bool VibratorStateOn()
{
	return true;
}