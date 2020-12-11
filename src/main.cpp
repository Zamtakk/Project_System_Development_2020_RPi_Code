#include "Websocket/SocketServer.hpp"

#include <iostream>
#include <thread>
#include <string>

using namespace std;

int main()
{
    SocketServer* Socket = SocketServer::Instance();
    string message;

    while(true){
        SocketMessage message = Socket->GetMessage();
        if(message.Message != ""){
            cout << "Incoming message: " << message.Message << endl;
            if (message.Message.find("exit") != string::npos) break;
        }else{
            this_thread::sleep_for (chrono::milliseconds(100));
        }
    }
}
