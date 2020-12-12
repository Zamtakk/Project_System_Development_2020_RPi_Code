#include "Websocket/SocketServer.hpp"
#include "Devices/Device.hpp"
#include "Devices/ExampleDevice.hpp"
#include "Devices/DeviceTypes.hpp"

#include "json.hpp"

#include <iostream>
#include <thread>
#include <string>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int main()
{
    SocketServer *Socket = SocketServer::Instance();
    string message;
    map<string, Device *> devices;
    vector<ExampleDevice *> exampleDevices;

    while (true)
    {
        message = Socket->GetMessage();
        if (message == "")
        {
            // When no messages are available, wait a bit to prevent overloading the system.
            sleep_for(milliseconds(50));
            continue;
        }

        json jsonMessage = json::parse(message);

        if (jsonMessage["command"] == REGISTRATION)
        {
            string newUUID, newType;

            if (jsonMessage["Type"] == EXAMPLEDEVICE)
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                ExampleDevice *newDevice = new ExampleDevice(newUUID, newType, Socket);
                exampleDevices.push_back(newDevice);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
        }
        else
        {
            devices.find(jsonMessage["UUID"])->second->handleMessage(message);
        }
    }
}
