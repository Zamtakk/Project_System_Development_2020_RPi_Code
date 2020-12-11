#include "Websocket/WebsocketConnection.hpp"

WebsocketConnection::WebsocketConnection(websocketpp::connection_hdl hdl, message_ptr msg): Handle(hdl), MessagePointer(msg){

}

WebsocketConnection::~WebsocketConnection(){

}