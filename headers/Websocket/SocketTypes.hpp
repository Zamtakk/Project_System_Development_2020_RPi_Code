#ifndef SOCKETTYPES_HPP
#define SOCKETTYPES_HPP

#include <string>

#include "WebsocketTypes.hpp"

struct DeviceRegistration
{
    std::string UUID;
    websocketpp::connection_hdl ConnectionHandle;
    message_ptr MessagePointer;
};

struct WebsocketMessage
{
    websocketpp::connection_hdl Handle;
    message_ptr MessagePointer;
};

struct SocketMessage
{
    std::string UUID;
    std::string Message;
};

#endif