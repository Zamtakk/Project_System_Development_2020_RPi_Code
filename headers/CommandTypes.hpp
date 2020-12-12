#ifndef COMMANDTYPES_HPP
#define COMMANDTYPES_HPP

enum ErrorCodes{
    NOT_REGISTERED = 0,
    INVALID_FORMAT
};

enum GeneralDeviceCommands{
    HEARTBEAT = 1000,
    REGISTRATION
};

#endif