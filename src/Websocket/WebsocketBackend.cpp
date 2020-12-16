#include "Websocket/WebsocketBackend.hpp"

using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::queue;

// Create a server endpoint
server Server;

queue<WebsocketMessage> *RxQueue;
mutex *RxQueueLock;

void MessageReceived(server *UNUSED(s), websocketpp::connection_hdl hdl, message_ptr msg)
{
    WebsocketMessage newMessage = {
        .Handle = hdl,
        .MessagePointer = msg};
    const lock_guard<mutex> lock(*RxQueueLock);
    RxQueue->push(newMessage);
}

void SetQueueAndLock(queue<WebsocketMessage> *queue, mutex *lock)
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
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "other exception" << endl;
    }
}