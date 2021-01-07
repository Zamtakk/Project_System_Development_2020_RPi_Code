#include "Devices/WIB.hpp"
#include "Devices/Website.hpp"
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
    case DEVICEINFO:
        ledState = (bool)jsonMessage["ledState"];
        switchState = (bool)jsonMessage["switchState"];
        potValue = (int)jsonMessage["potValue"];
        
        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website*>(website)->updateWebsite();
        break;
    case WIB_SWITCH_CHANGE:
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
    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, WIB_POTMETER_CHANGE));
    jsonMessage["value"] = potValue;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::switchStateOn(bool stateOn)
{
    ledStateOn(stateOn);

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, WIB_SWITCH_CHANGE));
    jsonMessage["value"] = stateOn;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief 
    @param[in] 
    @return 
*/
void WIB::ledStateOn(bool stateOn)
{
    json jsonMessage = json::parse(newMessage(uuid, type, WIB_LED_CHANGE));
    jsonMessage["value"] = stateOn;

    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}