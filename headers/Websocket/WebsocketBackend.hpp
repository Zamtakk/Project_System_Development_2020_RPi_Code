#ifndef WEBSOCKETBACKEND_HPP
#define WEBSOCKETBACKEND_HPP

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "WebsocketTypes.hpp"
#include "SocketTypes.hpp"

#include <iostream>
#include <thread>
#include <queue>
#include <string>

#ifdef UNUSED
#elif defined(__GNUC__)
#define UNUSED(x) UNUSED_##x __attribute__((unused))
#endif

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

void MessageReceived(server *UNUSED(s), websocketpp::connection_hdl hdl, message_ptr msg);

void SetQueueAndLock(std::queue<WebsocketMessage> *queue, std::mutex *lock);

void SendWebsocketppMessage(WebsocketMessage message);

void StartSocket();

#endif