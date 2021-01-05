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

enum ChairCommands
{
    CHAIR_FORCESENSOR_CHANGE = 4000,
    CHAIR_BUTTON_CHANGE = 4001,
    CHAIR_LED_CHANGE = 4002,
    CHAIR_VIBRATOR_CHANGE = 4003
};

enum WebsiteCommands
{
    WEBSITE_UPDATE = 5000
};

#endif