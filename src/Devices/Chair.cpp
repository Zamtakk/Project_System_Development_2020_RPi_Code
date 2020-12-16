#include "Devices/Chair.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;



Chair::Chair(string uuid, string type, SocketServer *server) : Device(uuid, type, server), ledState(false), vibratorState(false), buttonPressed(false), pressureValue(int)
{
int ledPin = D5;      // LED connected to digital pin 9
int vibrator = D5;   // potentiometer connected to analog pin 3
int pressureValue = 0;         // variable to store the read value

void setup() {
  pinMode(ledPin, OUTPUT);  // sets the pin as output
}

void loop() {
  pressureValue = analogRead(vibrator);  // read the input pin
  analogWrite(ledPin, pressureValue / 4); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}
}



Chair::~Chair()
{
}

string Chair::getDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"ledState", ledState},
        {"vibratorState", vibratorState},
        {"buttonPressed", buttonPressed},
        {"pressureValue", pressureValue}};

    return deviceInfo.dump();
}

void Chair::HandleMessage(string message){
    socketServer->SendMessage(uuid, message);
}