#include "Websocket/SocketServer.hpp"

#include <iostream>
#include <thread>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int main()
{
    SocketServer *Socket = SocketServer::Instance();
    string message;

    while (true)
    {
        SocketMessage message = Socket->GetMessage();
        if (message.Message != "")
        {
            cout << "Incoming message: " << message.Message << endl;
            if (message.Message.find("exit") != string::npos)
                break;
        }
        else
        {
            sleep_for(milliseconds(100));
        }
    }
}
