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
    @brief 
    @param[in] 
    @return 
*/
WIB::WIB(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
	: Device(uuid, type, server, devices),
	  ledState1(false),
	  ledState2(false),
	  ledState3(false),
	  ledState4(false),
	  potValue(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICEINFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief 
    @param[in] 
    @return 
*/
WIB::~WIB()
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
string WIB::GetDeviceInfo()
{
	json deviceInfo = {
		{"UUID", uuid},
		{"Type", type},
		{"Status", status},
		{"ledState0", ledState1},
		{"ledState1", ledState2},
		{"ledState2", ledState3},
		{"ledState3", ledState4},
		{"potValue", potValue}};

	return deviceInfo.dump();
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((WibCommands)jsonMessage["command"])
    {
    case WIB_BUTTON1_CHANGE:
        swtichStateOn(1, (bool)jsonMessage["value"]);
        break;
    case WIB_BUTTON2_CHANGE:
        swtichStateOn(2, (bool)jsonMessage["value"]);
        break;
    case WIB_BUTTON3_CHANGE:
        swtichStateOn(3, (bool)jsonMessage["value"]);
        break;
    case WIB_BUTTON4_CHANGE:
        swtichStateOn(4, (bool)jsonMessage["value"]);
        break;
    case WIB_LED1_CHANGE:
        ledStateOn(1, (bool)jsonMessage["value"]);
        break;
    case WIB_LED2_CHANGE:
        ledStateOn(2, (bool)jsonMessage["value"]);
        break;
    case WIB_LED3_CHANGE:
        ledStateOn(3, (bool)jsonMessage["value"]);
        break;
    case WIB_LED4_CHANGE:
        ledStateOn(4, (bool)jsonMessage["value"]);
        break;
    case WIB_POTMETER_CHANGE:
        potValueChange((int)jsonMessage["value"]);
        break;
    default:
        break;
    }
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool WIB::isLedOn(int ledNr)
{
    if (ledNr == 1)
    {
        return ledState1;
    }
    else if (ledNr == 2)
    {
        return ledState2;
    }
    else if (ledNr == 3)
    {
        return ledState3;
    }
    else if (ledNr == 4)
    {
        return ledState4;
    }
    else
    {
        return false;
    }
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool WIB::getSwitchState(int switchNr)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::potValueChange(int potValue)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::swtichStateOn(int switchNr, bool stateOn)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::ledStateOn(int LedNr, bool stateOn)
{
}