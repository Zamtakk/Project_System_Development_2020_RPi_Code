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
	  ledState(false),
	  switchState(false),
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
		{"ledState", ledState},
		{"switchState", switchState},
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
    case WIB_BUTTON_CHANGE:
        switchStateOn((bool)jsonMessage["value"]);
        break;
    case WIB_LED_CHANGE:
        ledStateOn((bool)jsonMessage["value"]);
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
bool WIB::isLedOn()
{
    return ledState;
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool WIB::getSwitchState()
{
    return switchState;
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
void WIB::switchStateOn(bool stateOn)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::ledStateOn(bool stateOn)
{
}