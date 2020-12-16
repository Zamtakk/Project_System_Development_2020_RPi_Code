#ifndef WEBSOCKETTYPES_HPP
#define WEBSOCKETTYPES_HPP

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;
typedef websocketpp::connection_hdl connection_hdl;

#endif