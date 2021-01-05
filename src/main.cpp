#include "Websocket/SocketServer.hpp"
#include "Devices/Device.hpp"
#include "Devices/ExampleDevice.hpp"
#include "Devices/Chair.hpp"
#include "Devices/Website.hpp"

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
using std::to_string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int main()
{
    SocketServer *Socket = SocketServer::Instance();
    string message;
    map<string, Device *> devices;

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

            if (jsonMessage["Type"] == "ExampleDevice")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                ExampleDevice *newDevice = new ExampleDevice(newUUID, newType, Socket);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Chair")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Chair *newDevice = new Chair(newUUID, newType, Socket);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Website")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Website *newDevice = new Website(newUUID, newType, Socket);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
        
                map<string, Device *>::iterator it = devices.begin();

                while(it != devices.end()) {
                    string deviceInfo = it -> second -> GetDeviceInfo();
                    string sendmessage = "{\"UUID\": \"" + newUUID + "\", \"Type\": \"" + newType + "\", \"command\": " + to_string(WEBSITE_UPDATE) + ", \"value\":[" + deviceInfo + "]}";
                    newDevice->HandleMessage(sendmessage);
                    it++;
                }
            }
        }
        else
        {
            if (devices.count(jsonMessage["UUID"]) != 0)
            {
                devices.find(jsonMessage["UUID"])->second->HandleMessage(message);
            }
            else
            {
                Socket->SendMessage(jsonMessage["UUID"], "{\"error\":" + to_string(NOT_REGISTERED) + ",\"description\":\"Device object was not yet created!\"}");
            }
        }
    }
}
