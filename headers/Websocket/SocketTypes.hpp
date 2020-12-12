#ifndef SOCKETTYPES_HPP
#define SOCKETTYPES_HPP

#include <string>

#include "WebsocketTypes.hpp"

using std::string;

struct DeviceRegistration
{
    string UUID;
    connection_hdl ConnectionHandle;
    message_ptr MessagePointer;
};

struct WebsocketMessage
{
    connection_hdl Handle;
    message_ptr MessagePointer;
};

#endif