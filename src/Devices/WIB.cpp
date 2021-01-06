#include "Devices/WIB.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::string;
using std::to_string;

/*!
    @brief Constructor for the WIB object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
WIB::WIB(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices), vibratorState(false), buttonPressed(false), ledState(false), pressureValue(0)
{
}

/*!
    @brief Deconstructor for WIB objects
*/
WIB::~WIB()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string WIB::GetDeviceInfo()
{
	json deviceInfo = {
		{"UUID", uuid},
		{"Type", type},
		{"Status", status},
		{"ledState0", ledState0},
		{"ledState1", ledState1},
		{"ledState2", ledState2},
		{"ledState3", ledState3},
		{"switchState0", switchState0},
		{"switchState1", switchState1},
		{"switchState2", switchState2},
		{"switchState3", switchState3},
		{"potValue", potValue},

	return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void WIB::HandleMessage(string message)
{
	json jsonMessage = json::parse(message);
	if (jsonMessage["command"] == WIB_POTSENSOR_CHANGE)
	{
		potValueChange(jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == WIB_BUTTON0_CHANGE)
	{
		switchState0(jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == WIB_BUTTON1_CHANGE)
	{
		switchState1(jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == WIB_BUTTON2_CHANGE)
	{
		switchState2(jsonMessage["value"]);
	}
	else if (jsonMessage["command"] == WIB_BUTTON3_CHANGE)
	{
		switchState3(jsonMessage["value"]);
	}
	socketServer->SendMessage(uuid, message);
}

/*!
    @brief Getter for the led states
    @returns A boolean to check whether leds are on or off
*/
bool WIB::isLedOn0()
{
	return isLedOn0;
}

bool WIB::isLedOn1()
{
	return isLedOn1;
}

bool WIB::isLedOn2()
{
	return isLedOn2;
}

bool WIB::isLedOn3()
{
	return isLedOn3;
}

/*!
    @brief Function to handle incoming messages concerning changes in the pot meter.
    @param[in] potValueReceived The pot value in the message.
*/
void WIB::potValueChange(int potValueReceived)
{
	//TODO: use vector with devices in stead of hardcoded website uuid
	string sendmessage = "{\"UUID\":\"0000000001\",\"Type\":\"" + type + "\",\"command\":" + to_string(WIB_POTSENSOR_CHANGE) + ",\"value\":" + to_string(potValueReceived) + "}";
	socketServer->SendMessage("0000000001", sendmessage);
	potValue = potValueReceived;
}

/*!
    @brief Function to handle incoming messages when the switch is pressed or released
    @param[in] switchStateX Boolean on whether switch is used
*/
void WIB::switchState0(bool switchState0)
{
	if (switchState0)
	{
		switchState0 = true;
	}
	else
	{
		switchState0 = false;
	}
}

void WIB::switchState1(bool switchState1)
{
	if (switchState1)
	{
		switchState1 = true;
	}
	else
	{
		switchState1 = false;
	}
}

void WIB::switchState2(bool switchState2)
{
	if (switchState2)
	{
		switchState2 = true;
	}
	else
	{
		switchState2 = false;
	}
}

void WIB::switchState3(bool switchState3)
{
	if (switchState3)
	{
		switchState3 = true;
	}
	else
	{
		switchState3 = false;
	}
}

/*!
    @brief Functions to turn the leds on or off
    @param[in] ledStateOnX Boolean on whether led needs to be on or off
*/
void WIB::LedStateOn0(bool ledStateOn0)
{
	if (ledStateOn0)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(WIB_LED0_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn0 = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(WIB_LED0_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn0 = false;
	}
}

void WIB::LedStateOn1(bool ledStateOn1)
{
	if (ledStateOn1)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(WIB_LED1_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn1 = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(WIB_LED0_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn0 = false;
	}
}

void WIB::LedStateOn2(bool ledStateOn2)
{
	if (ledStateOn2)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(WIB_LED2_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn2 = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(WIB_LED2_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn2 = false;
	}
}

void WIB::LedStateOn3(bool ledStateOn3)
{
	if (ledStateOn3)
	{
		string sendmessage = "{\"UUID\":\"" + uuid + "\",\"Type\":\"" + type + "\",\"command\":" + to_string(WIB_LED3_CHANGE) + ",\"value\": \"1\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn3 = true;
	}
	else
	{
		string sendmessage = "{\"UUID\": \"" + uuid + "\", \"Type\": \"" + type + "\", \"command\":" + to_string(WIB_LED3_CHANGE) + ", \"value\": \"0\"}";
		socketServer->SendMessage(uuid, sendmessage);
		ledStateOn3 = false;
	}
}

