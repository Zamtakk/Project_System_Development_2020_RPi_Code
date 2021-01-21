// Includes

#include "Websocket/SocketServer.hpp"

#include "Devices/Bed.hpp"
#include "Devices/Chair.hpp"
#include "Devices/Column.hpp"
#include "Devices/Device.hpp"
#include "Devices/Door.hpp"
#include "Devices/Fridge.hpp"
#include "Devices/Lamp.hpp"
#include "Devices/SimulatedDevice.hpp"
#include "Devices/Wall.hpp"
#include "Devices/Website.hpp"
#include "Devices/WIB.hpp"

#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>

// Define namespace functions

using nlohmann::json;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

// Main loop

int main()
{
    SocketServer *Socket = SocketServer::Instance();
    string message;
    map<string, Device *> devices;

    while (true)
    {
        // Get new messages from socket server
        message = Socket->GetMessage();
        if (message == "")
        {
            // When no messages are available, wait a bit to prevent overloading the system.
            sleep_for(milliseconds(10));
            continue;
        }

        // Parse the message to JSON so it is easier to read.
        json jsonMessage = json::parse(message);

        // If it is a registration message, create a new device of that type
        if (jsonMessage["command"] == REGISTRATION)
        {
            string newUUID = jsonMessage["UUID"];
            string newType = jsonMessage["Type"];

            if (jsonMessage["Type"] == "Bed")
            {
                devices.insert(pair<string, Device *>(newUUID, new Bed(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Chair")
            {
                devices.insert(pair<string, Device *>(newUUID, new Chair(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Column")
            {
                devices.insert(pair<string, Device *>(newUUID, new Column(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Door")
            {
                devices.insert(pair<string, Device *>(newUUID, new Door(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Fridge")
            {
                devices.insert(pair<string, Device *>(newUUID, new Fridge(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Lamp")
            {
                devices.insert(pair<string, Device *>(newUUID, new Lamp(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "SimulatedDevice")
            {
                devices.insert(pair<string, Device *>(newUUID, new SimulatedDevice(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Wall")
            {
                devices.insert(pair<string, Device *>(newUUID, new Wall(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "Website")
            {
                devices.insert(pair<string, Device *>(newUUID, new Website(newUUID, newType, Socket, &devices)));
            }
            else if (jsonMessage["Type"] == "WIB")
            {
                devices.insert(pair<string, Device *>(newUUID, new WIB(newUUID, newType, Socket, &devices)));
            }
        }
        else
        {
            // If it is a normal message, check if the device exists and pass on the message
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
