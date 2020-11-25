#include "socket_server.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <queue>

using namespace std;

int main()
{
    queue<string> *socket_rx = new queue<string>();
    queue<string> *socket_tx = new queue<string>();

    setRxTx(socket_rx, socket_tx);

    // Constructs the new thread and runs it. Does not block execution.
    thread t1(StartSocket);

    while(true){
        if(socket_rx->size() > 0){
            string message = socket_rx->front();
            socket_rx->pop();
            cout << "Incoming message: " << message << endl;
            if (message.find("exit") != string::npos) break;
        }else{
            this_thread::sleep_for (chrono::milliseconds(100));
        }
    }
}
