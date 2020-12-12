#include "Websocket/SocketServer.hpp"
#include "Websocket/WebsocketTypes.hpp"
#include "json.hpp"

#include "CommandTypes.hpp"

using json = nlohmann::json;

using std::lock_guard;
using std::mutex;
using std::string;
using std::thread;
using std::to_string;
using std::vector;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

// Global static pointer used to ensure a single instance of the class.
SocketServer *SocketServer::socketServer = NULL;

// Public methods
SocketServer *SocketServer::Instance()
{
    if (!socketServer)
        socketServer = new SocketServer();
    return socketServer;
}

string SocketServer::GetMessage()
{
    const lock_guard<mutex> lock(messageLock);
    if (!messageQueue.empty())
    {
        string newMessage = messageQueue.front();
        messageQueue.pop();
        return newMessage;
    }
    else
    {
        return "";
    }
}

bool SocketServer::SendMessage(string uuid, string jsonStringMessage)
{
    DeviceRegistration *devicePtr = getRegisteredDevice(uuid);
    if (devicePtr != nullptr)
    {
        WebsocketMessage packet;
        packet.Handle = devicePtr->ConnectionHandle;
        packet.MessagePointer = devicePtr->MessagePointer;
        packet.MessagePointer->set_payload(jsonStringMessage);
        SendWebsocketppMessage(packet);
        return true;
    }
    return false;
}

// Private Methods
SocketServer::SocketServer()
{
    SetQueueAndLock(&websocketppRxQueue, &websocketppRxLock);

    websocketppThread = new thread(StartSocket);
    processRxThread = new thread(&SocketServer::parseIncommingMessages, this);
}

SocketServer::~SocketServer()
{
    delete websocketppThread;
    delete processRxThread;
}

void SocketServer::parseIncommingMessages()
{
    WebsocketMessage websocketppMessage;

    while (1)
    {
        // Wait before parsing a new message to not overload the system
        sleep_for(milliseconds(250));

        // Aquire lock and get a new message if one is available
        const lock_guard<mutex> _websocketppLock(websocketppRxLock);
        if (!websocketppRxQueue.empty())
        {
            websocketppMessage = websocketppRxQueue.front();
            websocketppRxQueue.pop();
        }
        else
        {
            _websocketppLock.~lock_guard();
            continue;
        }
        _websocketppLock.~lock_guard();

        // Check if the message stucture is valid JSON and if it contains the required fields
        if (!isMessageValid(websocketppMessage.MessagePointer->get_payload()))
        {
            sendIncorrectMessageFormat(websocketppMessage);
            continue;
        }

        // Parse the message for easy use
        json jsonMessage = json::parse(websocketppMessage.MessagePointer->get_payload());

        // Check if the message comes from a registered device
        // If it doesn't, register it if it is a registration message
        if (!isDeviceRegistered(jsonMessage["UUID"]))
        {
            if (jsonMessage["command"] == REGISTRATION)
            {
                registerDevice(websocketppMessage);
            }
            else
            {
                sendDeviceNotRegistered(jsonMessage["UUID"]);
            }
            continue;
        }

        // Handle message
        if (jsonMessage["command"] == HEARTBEAT)
        {
            // Todo: implement heartbeat system
        }
        else
        {
            const lock_guard<mutex> _messageLock(messageLock);
            messageQueue.push(websocketppMessage.MessagePointer->get_payload());
            _messageLock.~lock_guard();
        }
    }
}

/*!
    @brief Checks to see if the message is valid JSON and if it contains the required fields
    @param[in] message The raw string message passed on from the websocket connection
    @return Returns true if message is proper JSON and contains the required fields
*/
bool SocketServer::isMessageValid(string message)
{
    json jsonMessage = json::parse(message);

    if (!jsonMessage.contains("UUID") || !jsonMessage.contains("Type") || !jsonMessage.contains("Commands"))
        return false;
    return true;
}

bool SocketServer::isDeviceRegistered(string uuid)
{
    for (vector<DeviceRegistration>::iterator i = registeredDevices.begin(); i < registeredDevices.end(); i++)
    {
        if (i->UUID.find(uuid) != string::npos)
        {
            return true;
        }
    }
    return false;
}

void SocketServer::sendDeviceNotRegistered(WebsocketMessage websocketppMessage)
{
    websocketppMessage.MessagePointer->set_payload("{\"error\":" + to_string(NOT_REGISTERED) + ",\"description\":\"Device not registered!\"}");
    SendWebsocketppMessage(websocketppMessage);
}

void SocketServer::sendIncorrectMessageFormat(WebsocketMessage websocketppMessage)
{
    websocketppMessage.MessagePointer->set_payload("{\"error\":" + to_string(INVALID_FORMAT) + ",\"description\":\"Message format is not correct.\nIt needs at least UUID, Type and Commands\"}");
    SendWebsocketppMessage(websocketppMessage);
}

DeviceRegistration *SocketServer::getRegisteredDevice(string uuid)
{
    for (vector<DeviceRegistration>::iterator i = registeredDevices.begin(); i < registeredDevices.end(); i++)
    {
        if (i->UUID.find(uuid) != string::npos)
        {
            return &(*i);
        }
    }
    return nullptr;
}

void SocketServer::registerDevice(WebsocketMessage websocketppMessage)
{
    json jsonMessage = json::parse(websocketppMessage.MessagePointer->get_payload());

    DeviceRegistration newDevice{
        .UUID = jsonMessage["UUID"],
        .ConnectionHandle = websocketppMessage.Handle,
        .MessagePointer = websocketppMessage.MessagePointer};

    registeredDevices.push_back(newDevice);
}