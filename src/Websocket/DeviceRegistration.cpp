#include "Websocket/DeviceRegistration.hpp"

DeviceRegistration::DeviceRegistration(std::string uuid, WebsocketConnection *connection): Connection(connection), UUID(uuid){

}

DeviceRegistration::~DeviceRegistration(){
    
}