#include "Devices/Bed.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;



Bed::Bed(string uuid, string type, SocketServer *server) : Device(uuid, type, server), ledState(false), buttonPressed(false), pressureValue(0)
{
}



Bed::~Bed()
{
}

string Bed::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"ledState", ledState},
        {"buttonPressed", buttonPressed},
        {"pressureValue", pressureValue}};

    return deviceInfo.dump();
}

void Bed::HandleMessage(string message){
    socketServer->SendMessage(uuid, message);
}