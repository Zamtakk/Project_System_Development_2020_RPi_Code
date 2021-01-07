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
	  ledState0(false),
	  ledState1(false),
	  ledState2(false),
	  ledState3(false),
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
		{"ledState0", ledState0},
		{"ledState1", ledState1},
		{"ledState2", ledState2},
		{"ledState3", ledState3},
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
}

/*!
    @brief 
    @param[in] 
    @return 
*/
bool WIB::isLedOn(int ledNr)
{
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