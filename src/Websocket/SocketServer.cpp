#include "Websocket/SocketServer.hpp"
#include "Websocket/WebsocketTypes.hpp"

// Global static pointer used to ensure a single instance of the class.
SocketServer *SocketServer::socketServer = NULL;

// Public methods
SocketServer *SocketServer::Instance()
{
    if (!socketServer)
        socketServer = new SocketServer();
    return socketServer;
}

SocketMessage SocketServer::GetMessage()
{
    const std::lock_guard<std::mutex> lock(*messageLock);
    if (!messageQueue->empty())
    {
        SocketMessage newMessage = messageQueue->front();
        messageQueue->pop();
        return newMessage;
    }
    else
    {
        SocketMessage msg = {
            .UUID = "",
            .Message = ""
        };
        return msg;
    }
}

bool SocketServer::SendMessage(SocketMessage message)
{
    DeviceRegistration *device = getRegisteredDevice(message.UUID);
    if (device != nullptr){
        device->Connection->MessagePointer->set_payload(message.Message);
        SendWebsocketppMessage(device->Connection);
        return true;
    }
    return false;
}

// Private Methods
SocketServer::SocketServer()
{
    websocketppRxQueue = new std::queue<WebsocketConnection *>();
    messageQueue = new std::queue<SocketMessage>();

    SetQueueAndLock(websocketppRxQueue, websocketppRxLock);

    websocketppThread = new std::thread(StartSocket);
    processRxThread = new std::thread(parseIncommingMessages);
}

SocketServer::~SocketServer()
{
    delete websocketppRxQueue;
    delete messageQueue;
    delete websocketppThread;
    delete processRxThread;
}

void SocketServer::parseIncommingMessages()
{
    WebsocketConnection *websocketppMessage;

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        const std::lock_guard<std::mutex> _websocketppLock(*websocketppRxLock);
        if(!websocketppRxQueue->empty()){
            websocketppMessage = websocketppRxQueue->front();
            websocketppRxQueue->pop();
        }else{
            _websocketppLock.~lock_guard();
            continue;
        }
        _websocketppLock.~lock_guard();

        SendWebsocketppMessage(websocketppMessage);

        // switch ( message.command ) {
        //     case "registration":

        //     break;
        //     case "heatbeat":

        //     break;
        //     default:
        //         const std::lock_guard<std::mutex> _messageLock(*messageLock);
        //         SocketMessage newSocketMessage;
        //         newSocketMessage.UUID = message.uuid;
        //         newSocketMessage.Message = message;
        //         messageQueue->push(newSocketMessage);
        //         _messageLock.~lock_guard();
        //     break;
        // }

        delete websocketppMessage;
    }
}