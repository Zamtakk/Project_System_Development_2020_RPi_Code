#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP
 
#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <thread>
#include <queue>
#include <string>

std::queue<std::string> *_socket_rx;
std::queue<std::string> *_socket_tx;

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

void new_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg){
    // std::cout << "on_message called with hdl: " << hdl.lock().get()
    //           << " and message: " << msg->get_payload()
    //           << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.

    // if (msg->get_payload() == "stop-listening")
    // {
    //     s->stop_listening();
    //     return;
    // }

    _socket_rx->push(msg->get_payload());

    // try
    // {
    //     s->send(hdl, msg->get_payload(), msg->get_opcode());
    // }
    // catch (websocketpp::exception const &e)
    // {
    //     std::cout << "Echo failed because: "
    //               << "(" << e.what() << ")" << std::endl;
    // }
}

void setRxTx(std::queue<std::string> *rx, std::queue<std::string> *tx){
    _socket_rx = rx;
    _socket_tx = tx;
}

void StartSocket(){
    // Create a server endpoint
    server echo_server;

    try
    {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&new_message, &echo_server, ::_1, ::_2));

        // Listen on port 9002
        echo_server.listen(9002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
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
 
#endif