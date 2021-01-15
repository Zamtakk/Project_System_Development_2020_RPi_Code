#include "Websocket/SocketServer.hpp"
#include "Devices/Device.hpp"
#include "Devices/ExampleDevice.hpp"
#include "Devices/Chair.hpp"
#include "Devices/Website.hpp"
#include "Devices/SimulatedDevice.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Door.hpp"
#include "Devices/Wall.hpp"
#include "Devices/WIB.hpp"

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
            sleep_for(milliseconds(10));
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
                ExampleDevice *newDevice = new ExampleDevice(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Chair")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Chair *newDevice = new Chair(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Website")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Website *newDevice = new Website(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "SimulatedDevice")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                SimulatedDevice *newDevice = new SimulatedDevice(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Bed")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Bed *newDevice = new Bed(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Door")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Door *newDevice = new Door(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "Wall")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                Wall *newDevice = new Wall(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
            }
            else if (jsonMessage["Type"] == "WIB")
            {
                newUUID = jsonMessage["UUID"];
                newType = jsonMessage["Type"];
                WIB *newDevice = new WIB(newUUID, newType, Socket, &devices);
                devices.insert(pair<string, Device *>(newUUID, newDevice));
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
