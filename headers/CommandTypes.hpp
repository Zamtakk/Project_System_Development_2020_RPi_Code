#ifndef COMMANDTYPES_HPP
#define COMMANDTYPES_HPP

enum ErrorCodes
{
    NOT_REGISTERED = 0,
    INVALID_FORMAT = 1
};

enum GeneralDeviceCommands
{
    HEARTBEAT = 1000,
    REGISTRATION = 1001,
    DEVICEINFO = 1002
};

enum DeviceStatus
{
    CONNECTED = 2000,
    UNSTABLE = 2001,
    DISCONNECTED = 2002
};

#endif