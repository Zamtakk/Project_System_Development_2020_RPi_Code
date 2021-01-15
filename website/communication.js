var socket = new WebSocket("ws://169.254.181.129:9002"); //change the ip address to the ip address of your pi! - also works when using live server
const uuid = "0000000001";
const type = "Website";

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
async function sendSocket(elementId) {
	var value = "";
	if (elementId == "chair_massage_switch") {
		if (document.getElementById("chair_status").innerHTML === "Disconnected") return;

		check = document.getElementById("chair_massage_switch");
		value = {
			UUID: document.getElementById("chair_uuid").innerHTML,
			Type: "Chair",
			command: ChairCommands.CHAIR_VIBRATOR_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "bed_light") {
		if (document.getElementById("bed_status").innerHTML === "Disconnected") return;

		check = document.getElementById("bed_light");
		value = {
			UUID: document.getElementById("bed_uuid").innerHTML,
			Type: "Bed",
			command: BedCommands.BED_LED_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "simulation_light_1_slider") {
		if (document.getElementById("simulation_status").innerHTML === "Disconnected") return;

		check = document.getElementById("simulation_light_1_slider");
		value = {
			UUID: document.getElementById("simulation_uuid").innerHTML,
			Type: "SimulatedDevice",
			command: SimulatedDeviceCommands.SIMULATED_LED1_CHANGE,
			value: parseInt(check.value)
		};
	}
	else if (elementId == "simulation_light_2_slider") {
		if (document.getElementById("simulation_status").innerHTML === "Disconnected") return;

		check = document.getElementById("simulation_light_2_slider");
		value = {
			UUID: document.getElementById("simulation_uuid").innerHTML,
			Type: "SimulatedDevice",
			command: SimulatedDeviceCommands.SIMULATED_LED2_CHANGE,
			value: parseInt(check.value)
		};
	}
	else if (elementId == "simulation_light_3_slider") {
		if (document.getElementById("simulation_status").innerHTML === "Disconnected") return;

		check = document.getElementById("simulation_light_3_slider");
		value = {
			UUID: document.getElementById("simulation_uuid").innerHTML,
			Type: "SimulatedDevice",
			command: SimulatedDeviceCommands.SIMULATED_LED3_CHANGE,
			value: parseInt(check.value)
		};
	}
	else if (elementId == "door_closeopen_switch") {
		if (document.getElementById("door_status").innerHTML === "Disconnected") return;

		check = document.getElementById("door_closeopen_switch");
		value = {
			UUID: document.getElementById("door_uuid").innerHTML,
			Type: "Door",
			command: DoorCommands.DOOR_SERVO_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "door_unlocklock_switch") {
		if (document.getElementById("door_status").innerHTML === "Disconnected") return;

		check = document.getElementById("door_unlocklock_switch");
		value = {
			UUID: document.getElementById("door_uuid").innerHTML,
			Type: "Door",
			command: DoorCommands.DOOR_LOCK_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "door_ring_button") {
		if (document.getElementById("door_status").innerHTML === "Disconnected") return;

		check = document.getElementById("door_ring_button");
		value = {
			UUID: document.getElementById("door_uuid").innerHTML,
			Type: "Door",
			command: DoorCommands.DOOR_BUTTON2_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "wib_led") {
		if (document.getElementById("wib_status").innerHTML === "Disconnected") return;

		check = document.getElementById("wib_led");
		value = {
			UUID: document.getElementById("wib_uuid").innerHTML,
			Type: "WIB",
			command: WibCommands.WIB_LED_CHANGE,
			value: check.checked
		};
	}
	else if (elementId == "fridge_temperature_slider") {
		if (document.getElementById("fridge_status").innerHTML === "Disconnected") return;

		check = document.getElementById("fridge_temperature_slider");
		value = {
			UUID: document.getElementById("fridge_uuid").innerHTML,
			Type: "Fridge",
			command: FridgeCommands.FRIDGE_COOLINGVALUE_CHANGE,
			value: parseInt(check.value)
		};
		document.getElementById("fridge_set_temperature").innerHTML = parseInt(check.value);
	}
	else {
		return;
	}
	var msg = {
		UUID: uuid,
		Type: type,
		command: WebsiteCommands.WEBSITE_FORWARD,
		value: value
	};
	socket.send(JSON.stringify(msg));
}

/*!
	@brief Send registration message when the connection with server gets established
*/
socket.onopen = function (event) {
	var registration = {
		UUID: uuid,
		Type: type,
		command: GeneralDeviceCommands.REGISTRATION,
		value: ""
	}
	socket.send(JSON.stringify(registration));

	var update = {
		UUID: uuid,
		Type: type,
		command: WebsiteCommands.WEBSITE_UPDATE,
		value: ""
	}
	socket.send(JSON.stringify(update));
};

/*!
	@brief Trigger an event when receiving a message
*/
socket.onmessage = function (event) {
	var jsonMessage = JSON.parse(event.data);
	if (jsonMessage["command"] == WebsiteCommands.WEBSITE_UPDATE) {
		updateDeviceInformation(jsonMessage);
	}
	else if (jsonMessage["Type"] == "Chair" && jsonMessage["command"] == ChairCommands.CHAIR_FORCESENSOR_CHANGE) {
		document.getElementById("chair_measured_weight").innerHTML = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Chair" && jsonMessage["command"] == ChairCommands.CHAIR_VIBRATOR_CHANGE) {
		document.getElementById("chair_massage_switch").checked = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Bed" && jsonMessage["command"] == BedCommands.BED_FORCESENSOR_CHANGE) {
		document.getElementById("bed_measured_weight").innerHTML = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Bed" && jsonMessage["command"] == BedCommands.BED_LED_CHANGE) {
		document.getElementById("bed_light").checked = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "SimulatedDevice" && jsonMessage["command"] == SimulatedDeviceCommands.SIMULATED_LED1_CHANGE) {
		document.getElementById("simulation_light_1_slider").value = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "SimulatedDevice" && jsonMessage["command"] == SimulatedDeviceCommands.SIMULATED_LED2_CHANGE) {
		document.getElementById("simulation_light_2_slider").value = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "SimulatedDevice" && jsonMessage["command"] == SimulatedDeviceCommands.SIMULATED_LED3_CHANGE) {
		document.getElementById("simulation_light_3_slider").value = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "WIB" && jsonMessage["command"] == WibCommands.WIB_SWITCH_CHANGE) {
		if (jsonMessage["value"]) {
			document.getElementById("wib_switch").innerHTML = "on";
		} else {
			document.getElementById("wib_switch").innerHTML = "off";
		}
	}
	else if (jsonMessage["Type"] == "WIB" && jsonMessage["command"] == WibCommands.WIB_LED_CHANGE) {
		document.getElementById("wib_led").checked = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "WIB" && jsonMessage["command"] == WibCommands.WIB_POTMETER_CHANGE) {
		document.getElementById("wib_potmeter").innerHTML = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Fridge" && jsonMessage["command"] == FridgeCommands.FRIDGE_COOLINGVALUE_CHANGE) {
		document.getElementById("fridge_set_temperature").innerHTML = jsonMessage["value"];
		document.getElementById("fridge_temperature_slider").value = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Fridge" && jsonMessage["command"] == FridgeCommands.FRIDGE_TEMPERATURESENSORINSIDE_CHANGE) {
		document.getElementById("fridge_temperature").innerHTML = jsonMessage["temperatureValueInside"];
	}
	else if (jsonMessage["Type"] == "Fridge" && jsonMessage["command"] == FridgeCommands.FRIDGE_SWITCH_CHANGE) {
		var value = parseInt(document.getElementById("fridge_opened").innerHTML);
		if (jsonMessage["value"]) {
			value += 1;
		}
		document.getElementById("fridge_opened").innerHTML = value;
	}
	else if (jsonMessage["Type"] == "Door" && jsonMessage["command"] == DoorCommands.DOOR_SERVO_CHANGE) {
		document.getElementById("door_closeopen_switch").checked = jsonMessage["value"];
	}
	else if (jsonMessage["Type"] == "Door" && jsonMessage["command"] == DoorCommands.DOOR_LOCK_CHANGE) {
		document.getElementById("door_unlocklock_switch").checked = jsonMessage["value"];
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
				document.getElementById("fridge_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "fridge_status");
				document.getElementById("fridge_set_temperature").innerHTML = jsonMessage["value"][i]["coolingValue"];
				document.getElementById("fridge_temperature_slider").value = jsonMessage["value"][i]["coolingValue"];
				break;
			case "Lamp":
				document.getElementById("lamp_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "lamp_status");
				break;
			case "Door":
				document.getElementById("door_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "door_status");
				document.getElementById("door_closeopen_switch").checked = deviceInformation["value"][i]["doorOpen"];
				document.getElementById("door_unlocklock_switch").checked = deviceInformation["value"][i]["doorLocked"];
				break;
			case "Chair":
				document.getElementById("chair_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "chair_status");
				document.getElementById("chair_massage_switch").innerHTML = deviceInformation["value"][i]["vibratorState"];
				break;
			case "Bed":
				document.getElementById("bed_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "bed_status");
				document.getElementById("bed_light").innerHTML = deviceInformation["value"][i]["ledState"];
				break;
			case "Column":
				document.getElementById("column_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "column_status");
				break;
			case "Wall":
				document.getElementById("wall_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "wall_status");
				break;
			case "SimulatedDevice":
				document.getElementById("simulation_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "simulation_status");
				document.getElementById("simulation_light_1_slider").value = deviceInformation["value"][i]["led1Value"];
				document.getElementById("simulation_light_2_slider").value = deviceInformation["value"][i]["led2Value"];
				document.getElementById("simulation_light_3_slider").value = deviceInformation["value"][i]["led3Value"];
				break;
			case "WIB":
				document.getElementById("wib_uuid").innerHTML = deviceInformation["value"][i]["UUID"];
				updateStatus(deviceInformation["value"][i]["Status"], "wib_status");
				document.getElementById("wib_switch").innerHTML = deviceInformation["value"][i]["switchState"];
				document.getElementById("wib_led").checked = deviceInformation["value"][i]["ledState"];
				document.getElementById("wib_potmeter").innerHTML = deviceInformation["value"][i]["potValue"];
				break;
			default:
				break;
		}
	}
}

function updateStatus(newStatus, statusElementName){
	if (newStatus == DeviceStatus.CONNECTED) {
		document.getElementById(statusElementName).innerHTML = "Connected";
		document.getElementById(statusElementName).className = "status_connected";
	}else if (newStatus == DeviceStatus.UNSTABLE) {
		document.getElementById(statusElementName).innerHTML = "Lost";
		document.getElementById(statusElementName).className = "status_lost";
	}else if (newStatus == DeviceStatus.DISCONNECTED) {
		document.getElementById(statusElementName).innerHTML = "Disconnected";
		document.getElementById(statusElementName).className = "status_disconnected";
	}
}

//Command types

const ErrorCodes =
{
	NOT_REGISTERED: 0,
	INVALID_FORMAT: 1
};

const GeneralDeviceCommands =
{
	HEARTBEAT: 1000,
	REGISTRATION: 1001,
	DEVICEINFO: 1002
};

const DeviceStatus =
{
	CONNECTED: 2000,
	UNSTABLE: 2001,
	DISCONNECTED: 2002
};

const WebsiteCommands =
{
	WEBSITE_UPDATE: 3000,
	WEBSITE_FORWARD: 3001
};

const ChairCommands =
{
	CHAIR_FORCESENSOR_CHANGE: 4000,
	CHAIR_BUTTON_CHANGE: 4001,
	CHAIR_LED_CHANGE: 4002,
	CHAIR_VIBRATOR_CHANGE: 4003
};

const FridgeCommands =
{
	FRIDGE_SWITCH_CHANGE : 5000,
    FRIDGE_FAN_CHANGE : 5001,
    FRIDGE_TEC_CHANGE : 5002,
    FRIDGE_TEMPERATURESENSORINSIDE_CHANGE : 5003,
    FRIDGE_TEMPERATURESENSOROUTSIDE_CHANGE : 5004,
    FRIDGE_COOLINGVALUE_CHANGE : 5005
};

const ColumnCommands =
{
	COLUMN_BUTTON_CHANGE: 6000,
	COLUMN_BUZZER_CHANGE: 6001,
	COLUMN_LED_CHANGE: 6002,
	COLUMN_GASSENSOR_CHANGE: 6003
};

const BedCommands =
{
	BED_BUTTON_CHANGE: 7000,
	BED_LED_CHANGE: 7001,
	BED_FORCESENSOR_CHANGE: 7002
};

const LampCommands =
{
	LAMP_MOVEMENTSENSOR_CHANGE: 8000,
	LAMP_LED_CHANGE: 8001
};

const DoorCommands =
{
	DOOR_BUTTON1_CHANGE: 9000,
	DOOR_BUTTON2_CHANGE: 9001,
	DOOR_LED1_CHANGE: 9002,
	DOOR_LED2_CHANGE: 9003,
	DOOR_SERVO_CHANGE: 9004,
	DOOR_LOCK_CHANGE: 9005
};

const WallCommands =
{
	WALL_SCREEN_CHANGE: 10000,
	WALL_LDR_CHANGE: 10001,
	WALL_POTMETER_CHANGE: 10002,
	WALL_LEDSTRIP_CHANGE: 10003
};

const SimulatedDeviceCommands =
{
	SIMULATED_BUTTON1_CHANGE: 11000,
	SIMULATED_BUTTON2_CHANGE: 11001,
	SIMULATED_LED1_CHANGE: 11002,
	SIMULATED_LED2_CHANGE: 11003,
	SIMULATED_LED3_CHANGE: 11004,
	SIMULATED_POTMETER_CHANGE: 11005
};

const WibCommands =
{
	WIB_SWITCH_CHANGE: 12000,
	WIB_LED_CHANGE: 12001,
	WIB_POTMETER_CHANGE: 12002
};
