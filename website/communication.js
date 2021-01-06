var socket = new WebSocket("ws://169.254.41.114:9002"); //change the ip address to the ip address of your pi! - also works when using live server

/*!
	@brief Log a string to the notifications screen
	@param[in] logstr the string to be logged to the notifications screen
*/
function log(logstr) {
	document.getElementById("log").innerHTML += logstr + "\n";
}

/*!
	@brief Send a message to the websocket server
	@param[in] UUID the device UUID to be sent
	@param[in] type the device type 
	@param[in] command the command number
	@param[in] value an optional value to be added to the message
*/
async function sendSocket(type) {
	var msg;
	var value = "";
	if (type == "Chair") {
		if (document.getElementById("chair_status").innerHTML === "Disconnected") return;
		if (command == ChairCommands.CHAIR_BUTTON_CHANGE) {
			check = document.getElementById("chair_massage_switch");
			value = check.checked;
		}
		msg = {
			UUID: document.getElementById("chair_uuid").innerHTML,
			Type: type,
			command: command,
			value: value
		};
		socket.send(JSON.stringify(msg));
	}
	
}

/*!
	@brief Send registration message when the connection with server gets established
*/
socket.onopen = function (event) {
	var registration = {
		UUID: "0000000001",
		Type: "Website",
		command: GeneralDeviceCommands.REGISTRATION,
		value: ""
	}
	var update = {
		UUID: "0000000001",
		Type: "Website",
		command: GeneralDeviceCommands.WebsiteCommands.WEBSITE_UPDATE,
		value: ""
	}
	socket.send(JSON.stringify(registration));
	socket.send(JSON.stringify(update));
};

/*!
	@brief Trigger an event when receiving a message
*/
socket.onmessage = function (event) {
	var jsonMessage = JSON.parse(event.data);
	if (jsonMessage["Type"] == "Chair" && jsonMessage["command"] == ChairCommands.CHAIR_FORCESENSOR_CHANGE) {
		document.getElementById("chair_measured_weight").innerHTML = jsonMessage["value"];
	}
	else if (jsonMessage["command"] == WebsiteCommands.WEBSITE_UPDATE) {
		updateDeviceInformation(jsonMessage);
	}
}

/*!
	@brief Function to handle incoming messages containing the device information
	@param[in] JSON message with the device information as value
*/
async function updateDeviceInformation(deviceInformation) {
	console.log(deviceInformation);
	for (var i = 0; i < deviceInformation["value"].length; i++) {
		switch (deviceInformation["value"][i]["Type"]) {
			case "Fridge":
				document.getElementById("fridge_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("fridge_status").innerHTML = "connected";
				break;
			case "Lamp":
				document.getElementById("lamp_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("lamp_status").innerHTML = "connected";
				break;
			case "Door":
				document.getElementById("door_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("door_status").innerHTML = "connected";
				break;
			case "Chair":
				document.getElementById("chair_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("chair_status").innerHTML = "connected";
				break;
			case "Bed":
				document.getElementById("bed_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("bed_status").innerHTML = "connected";
				break;
			case "Column":
				document.getElementById("column_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("column_status").innerHTML = "connected";
				break;
			case "Wall":
				document.getElementById("wall_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("wall_status").innerHTML = "connected";
				break;
			case "Simulation":
				document.getElementById("simulation_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("simulation_status").innerHTML = "connected";
				break;
			case "WIB":
				document.getElementById("wib_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == DeviceStatus.CONNECTED)
					document.getElementById("wib_status").innerHTML = "connected";
				break;
			default:
				break;
		}
	}
}

const ErrorCodes =
{
    NOT_REGISTERED : 0,
    INVALID_FORMAT : 1
};

const GeneralDeviceCommands =
{
    HEARTBEAT : 1000,
    REGISTRATION : 1001,
    DEVICEINFO : 1002
};

const DeviceStatus =
{
    CONNECTED : 2000,
    UNSTABLE : 2001,
    DISCONNECTED : 2002
};

const WebsiteCommands =
{
    WEBSITE_UPDATE : 3000,
	WEBSITE_FORWARD : 3001
};

const ChairCommands =
{
    CHAIR_FORCESENSOR_CHANGE : 4000,
    CHAIR_BUTTON_CHANGE : 4001,
    CHAIR_LED_CHANGE : 4002,
    CHAIR_VIBRATOR_CHANGE : 4003
};

const FridgeCommands =
{
	FRIDGE_SWITCH_CHANGE : 5000,
    FRIDGE_FAN_CHANGE : 5001,
    FRIDGE_TEMPERATURESENSOR_CHANGE : 5002
};

const ColumnCommands =
{
	COLUMN_BUTTON_CHANGE : 6000,
    COLUMN_BUZZER_CHANGE : 6001,
    COLUMN_LED_CHANGE : 6002,
    COLUMN_GASSENSOR_CHANGE : 6003
};

const BedCommands =
{
	BED_BUTTON_CHANGE : 7000,
    BED_LED_CHANGE : 7001,
    BED_FORCESENSOR_CHANGE : 7002
};

const LampCommands =
{
	LAMP_MOVEMENTSENSOR_CHANGE : 8000,
    LAMP_LED_CHANGE : 8001
};

const DoorCommands =
{
	DOOR_BUTTON1_CHANGE : 9000,
    DOOR_BUTTON2_CHANGE : 9001,
    DOOR_LED1_CHANGE : 9002,
    DOOR_LED2_CHANGE : 9003,
    DOOR_SERVO_CHANGE : 9004
};

const WallCommands =
{
	WALL_SCREEN_CHANGE : 10000,
    WALL_LDR_CHANGE : 10001,
    WALL_POTMETER_CHANGE : 10002,
    WALL_LEDSTRIP_CHANGE : 10003
};

const SimulatedDeviceCommands =
{
	SIMULATED_BUTTON1_CHANGE : 11000,
    SIMULATED_BUTTON2_CHANGE : 11001,
    SIMULATED_LED1_CHANGE : 11002,
    SIMULATED_LED2_CHANGE : 11003,
    SIMULATED_LED3_CHANGE : 11003,
    SIMULATED_POTMETER_CHANGE : 11004
};

const WibCommands =
{
	WIB_BUTTON1_CHANGE : 12000,
    WIB_BUTTON2_CHANGE : 12001,
    WIB_BUTTON3_CHANGE : 12002,
    WIB_BUTTON4_CHANGE : 12003,
    WIB_LED1_CHANGE : 12004,
    WIB_LED2_CHANGE : 12005,
    WIB_LED3_CHANGE : 12006,
    WIB_LED4_CHANGE : 12007,
    WIB_POTMETER_CHANGE : 12008
};
