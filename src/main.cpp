#include "SocketServer.hpp"

#include <iostream>
#include <thread>
#include <string>

using namespace std;

int main()
{
    SocketServer* Socket = SocketServer::Instance();
    string message;

    while(true){
        message = Socket->GetMessage();
        if(message != ""){
            cout << "Incoming message: " << message << endl;
            if (message.find("exit") != string::npos) break;
        }else{
            this_thread::sleep_for (chrono::milliseconds(100));
        }
    }
}
