#ifndef WEBSOCKETCONNECTION_HPP
#define WEBSOCKETCONNECTION_HPP

#include "WebsocketTypes.hpp"

class WebsocketConnection{
public:
    WebsocketConnection(websocketpp::connection_hdl hdl, message_ptr msg);
    ~WebsocketConnection();
    websocketpp::connection_hdl Handle;
    message_ptr MessagePointer;
};

#endif