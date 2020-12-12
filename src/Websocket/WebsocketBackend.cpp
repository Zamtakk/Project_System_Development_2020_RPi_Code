#include "Websocket/WebsocketBackend.hpp"

// Create a server endpoint
server Server;

std::queue<WebsocketMessage> *RxQueue;
std::mutex *RxQueueLock;

void MessageReceived(server *UNUSED(s), websocketpp::connection_hdl hdl, message_ptr msg)
{
    WebsocketMessage newMessage = {
        .Handle = hdl,
        .MessagePointer = msg
    };
    const std::lock_guard<std::mutex> lock(*RxQueueLock);
    RxQueue->push(newMessage);
}

void SetQueueAndLock(std::queue<WebsocketMessage> *queue, std::mutex *lock)
{
    RxQueue = queue;
    RxQueueLock = lock;
}

void SendWebsocketppMessage(WebsocketMessage message)
{
    Server.send(message.Handle, message.MessagePointer);
}

void StartSocket()
{
    try
    {
        // Set logging settings
        Server.set_access_channels(websocketpp::log::alevel::all);
        Server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        Server.init_asio();

        // Register our message handler
        Server.set_message_handler(bind(&MessageReceived, &Server, ::_1, ::_2));

        // Listen on port 9002
        Server.listen(9002);

        // Start the server accept loop
        Server.start_accept();

        // Start the ASIO io_service run loop
        Server.run();
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "other exception" << std::endl;
    }
}