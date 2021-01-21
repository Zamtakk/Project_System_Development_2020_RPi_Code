var socket = new WebSocket("ws://169.254.41.114:9002"); //change the ip address to the ip address of your pi! - also works when using live server
const uuid = "0000000001";
const type = "Website";

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
	@brief Log a string to the notifications screen
	@param[in] logstr the string to be logged to the notifications screen
*/
function log(logstr) {
	document.getElementById("log").innerHTML += logstr + "\n";
}

/*!
	@brief Send a message to the websocket server
	@param[in] elementId The element that should be send to the server
*/
async function sendSocket(elementId) {
	if (elementId == "bed_light") {
		sendSwitchValue("bed_uuid", "Bed", "bed_status", "bed_light", BedCommands.BED_LED_ON);
	}
	else if (elementId == "chair_massage_switch") {
		sendSwitchValue("chair_uuid", "Chair", "chair_status", "chair_massage_switch", ChairCommands.CHAIR_VIBRATOR_ON);
	}
	else if (elementId == "column_smoke_threshold_slider") {
		sendSliderValue("column_uuid", "Column", "colum_status", "column_smoke_threshold_slider", ColumnCommands.COLUMN_SMOKE_TRESHOLD_VALUE);
		updateText("column_smoke_threshold", parseInt(document.getElementById("column_smoke_threshold_slider").value));
	}
	else if (elementId == "door_closeopen_switch") {
		sendSwitchValue("door_uuid", "Door", "door_status", "door_closeopen_switch", DoorCommands.DOOR_DOOR_OPEN);
	}
	else if (elementId == "door_unlocklock_switch") {
		sendSwitchValue("door_uuid", "Door", "door_status", "door_unlocklock_switch", DoorCommands.DOOR_DOOR_LOCKED);
	}
	else if (elementId == "door_ring_button") {
		sendSwitchValue("door_uuid", "Door", "door_status", "door_ring_button", DoorCommands.DOOR_BUTTON_OUTSIDE_PRESSED);
	}
	else if (elementId == "fridge_temperature_slider") {
		sendSliderValue("fridge_uuid", "Fridge", "fridge_status", "fridge_temperature_slider", FridgeCommands.FRIDGE_REQUESTED_FRIDGE_TEMPERATURE);
		updateText("fridge_set_temperature", parseInt(document.getElementById("fridge_temperature_slider").value));
	}
	else if (elementId == "wall_curtains_open_closed") {
		sendSwitchValue("wall_uuid", "Wall", "wall_status", "wall_curtains_open_closed", WallCommands.WALL_CURTAIN_OPEN);
	}
	else if (elementId == "wall_dimmer_strip_enabled") {
		sendSwitchValue("wall_uuid", "Wall", "wall_status", "wall_dimmer_strip_enabled", WallCommands.WALL_LEDSTRIP_ON);
	}
	else if (elementId == "wall_dimmer_light_enabled") {
		sendSwitchValue("wall_uuid", "Wall", "wall_status", "wall_dimmer_light_enabled", WallCommands.WALL_LAMP_ON);
	}
	else if (elementId == "wall_led_slider") {
		sendSliderValue("wall_uuid", "Wall", "wall_status", "wall_led_slider", WallCommands.WALL_LEDSTRIP_VALUE);
	}
	else if (elementId == "lamp_onoff_switch") {
		sendSwitchValue("lamp_uuid", "Lamp", "lamp_status", "lamp_onoff_switch", LampCommands.LAMP_LED_ON);
	}
	else if (elementId == "lamp_dimlevel_slider") {
		sendSliderValue("lamp_uuid", "Lamp", "lamp_status", "lamp_dimlevel_slider", LampCommands.LAMP_LED_VALUE);
	}
	else if (elementId == "simulation_light_1_slider") {
		sendSliderValue("simulation_uuid", "SimulatedDevice", "simulation_status", "simulation_light_1_slider", SimulatedDeviceCommands.SIMULATED_LED1_VALUE);
	}
	else if (elementId == "simulation_light_2_slider") {
		sendSliderValue("simulation_uuid", "SimulatedDevice", "simulation_status", "simulation_light_2_slider", SimulatedDeviceCommands.SIMULATED_LED2_VALUE);
	}
	else if (elementId == "simulation_light_3_slider") {
		sendSliderValue("simulation_uuid", "SimulatedDevice", "simulation_status", "simulation_light_3_slider", SimulatedDeviceCommands.SIMULATED_LED3_VALUE);
	}
	else if (elementId == "wib_led") {
		sendSwitchValue("wib_uuid", "WIB", "wib_status", "wib_led", WIBCommands.WIB_LED_ON);
	}
}

/*!
	@brief Trigger an event when receiving a message
*/
socket.onmessage = function (event) {
	var jsonMessage = JSON.parse(event.data);
	var type = jsonMessage["Type"];
	var command = jsonMessage["command"];
	var value = jsonMessage["value"];

	if (command == WebsiteCommands.WEBSITE_UPDATE) {
		updateDeviceInformation(jsonMessage);
	}
	else if (type == "Bed" && command == BedCommands.BED_LED_ON) {
		updateSwitch("bed_light", value);
	}
	else if (type == "Bed" && command == BedCommands.BED_PRESSURE_SENSOR_VALUE) {
		updateText("bed_measured_weight", value);
	}
	else if (type == "Chair" && command == ChairCommands.CHAIR_VIBRATOR_ON) {
		updateSwitch("chair_massage_switch", value);
	}
	else if (type == "Chair" && command == ChairCommands.CHAIR_PRESSURE_SENSOR_VALUE) {
		updateText("chair_measured_weight", value);
	}
	else if (type == "Column" && command == ColumnCommands.COLUMN_LED_ON) {
		updateText("column_light", value);
	}
	else if (type == "Column" && command == ColumnCommands.COLUMN_SMOKE_SENSOR_VALUE) {
		updateText("column_smoke_level", value);
	}
	else if (type == "Door" && command == DoorCommands.DOOR_DOOR_OPEN) {
		updateSwitch("door_closeopen_switch", value);
	}
	else if (type == "Door" && command == DoorCommands.DOOR_DOOR_LOCKED) {
		updateSwitch("door_unlocklock_switch", value);
	}
	else if (type == "Fridge" && command == FridgeCommands.FRIDGE_REQUESTED_FRIDGE_TEMPERATURE) {
		updateText("fridge_set_temperature", value);
		updateSlider("fridge_temperature_slider", value);
	}
	else if (type == "Fridge" && command == FridgeCommands.FRIDGE_TEMPERATURE_INSIDE_VALUE) {
		updateText("fridge_temperature", value);
	}
	else if (type == "Fridge" && command == FridgeCommands.FRIDGE_DOOR_OPEN_COUNTER) {
		updateText("fridge_opened", value);
	}
	else if (type == "Lamp" && command == LampCommands.LAMP_LED_ON) {
		updateSwitch("lamp_onoff_switch", value);
	}
	else if (type == "Lamp" && command == LampCommands.LAMP_LED_VALUE) {
		updateSlider("lamp_dimlevel_slider", value);
	}
	// else if (type == "Lamp" && command == LampCommands.LAMP_MOVEMENT_DETECTED) {
	// 	updateText("lamp_last_movement", value);
	// }
	else if (type == "SimulatedDevice" && command == SimulatedDeviceCommands.SIMULATED_LED1_VALUE) {
		updateSlider("simulation_light_1_slider", value);
	}
	else if (type == "SimulatedDevice" && command == SimulatedDeviceCommands.SIMULATED_LED2_VALUE) {
		updateSlider("simulation_light_2_slider", value);
	}
	else if (type == "SimulatedDevice" && command == SimulatedDeviceCommands.SIMULATED_LED3_VALUE) {
		updateSlider("simulation_light_3_slider", value);
	}
	else if (type == "Wall" && command == WallCommands.WALL_LDR_VALUE) {
		updateText("wall_light_level", value);
	}
	else if (type == "Wall" && command == WallCommands.WALL_LEDSTRIP_VALUE) {
		updateSlider("wall_led_slider", value);
	}
	else if (type == "WIB" && command == WIBCommands.WIB_SWITCH_ON) {
		if (value) {
			updateText("wib_switch", "on");
		} else {
			updateText("wib_switch", "off");
		}
	}
	else if (type == "WIB" && command == WIBCommands.WIB_LED_ON) {
		updateSwitch("wib_led", value);
	}
	else if (type == "WIB" && command == WIBCommands.WIB_DIMMER_VALUE) {
		updateText("wib_potmeter", value);
	}
}

/*!
	@brief Function to handle incoming messages containing the device information
	@param[in] JSON message with the device information as value
*/
async function updateDeviceInformation(deviceInformation) {
	console.log(deviceInformation);
	for (var i = 0; i < deviceInformation["value"].length; i++) {
		var device = deviceInformation["value"][i];
		switch (device["Type"]) {
			case "Bed":
				updateText("bed_uuid", device["UUID"]);
				updateStatus("bed_status", device["Status"]);
				updateText("bed_light", device["BED_LED_ON"]);
				break;
			case "Chair":
				updateText("chair_uuid", device["UUID"]);
				updateStatus("chair_status", device["Status"]);
				updateText("chair_massage_switch", device["CHAIR_VIBRATOR_ON"]);
				break;
			case "Column":
				updateText("column_uuid", device["UUID"]);
				updateStatus("column_status", device["Status"]);
				updateText("column_light"), parseBool(device["COLUMN_LED_ON"]);
				updateText("column_smoke_threshold"), parseInt(device["COLUMN_SMOKE_TRESHOLD_VALUE"]);
				updateSlider("column_smoke_threshold_slider"), device["COLUMN_SMOKE_TRESHOLD_VALUE"];
				break;
			case "Door":
				updateText("door_uuid", device["UUID"]);
				updateStatus("door_status", device["Status"]);
				updateSwitch("door_closeopen_switch", device["DOOR_DOOR_OPEN"]);
				updateSwitch("door_unlocklock_switch", device["DOOR_DOOR_LOCKED"]);
				break;
			case "Fridge":
				updateText("fridge_uuid", device["UUID"]);
				updateStatus("fridge_status", device["Status"]);
				updateText("fridge_temperature", device["FRIDGE_TEMPERATURE_INSIDE_VALUE"]);
				updateText("fridge_set_temperature", device["FRIDGE_REQUESTED_FRIDGE_TEMPERATURE"]);
				updateSlider("fridge_temperature_slider", device["FRIDGE_REQUESTED_FRIDGE_TEMPERATURE"]);
				updateText("fridge_opened", device["FRIDGE_DOOR_OPEN_COUNTER"]);
				break;
			case "Lamp":
				updateText("lamp_uuid", device["UUID"]);
				updateStatus("lamp_status", device["Status"]);
				// updateText("lamp_last_movement", device["movementSensorValue"]);
				updateSwitch("lamp_onoff_switch", device["LAMP_LED_ON"]);
				updateSlider("lamp_dimlevel_slider", device["LAMP_LED_VALUE"]);
				break;
			case "SimulatedDevice":
				updateText("simulation_uuid", device["UUID"]);
				updateStatus("simulation_status", device["Status"]);
				updateSlider("simulation_light_1_slider", device["SIMULATED_LED1_VALUE"]);
				updateSlider("simulation_light_2_slider", device["SIMULATED_LED2_VALUE"]);
				updateSlider("simulation_light_3_slider", device["SIMULATED_LED3_VALUE"]);
				break;
			case "Wall":
				updateText("wall_uuid", device["UUID"]);
				updateStatus("wall_status", device["Status"]);
				updateText("wall_light_level", device["WALL_LDR_VALUE"]);
				updateSwitch("wall_curtains_open_closed", device["WALL_CURTAIN_OPEN"]);
				updateSlider("wall_led_slider", device["WALL_LEDSTRIP_VALUE"]);
				updateSwitch("wall_dimmer_light_enabled", device["WALL_CURTAIN_OPEN"]);
				updateSwitch("wall_dimmer_strip_enabled", device["WALL_LAMP_ON"]);
				break;
			case "WIB":
				updateText("wib_uuid", device["UUID"]);
				updateStatus("wib_status", device["Status"]);
				updateText("wib_switch", device["WIB_SWITCH_ON"]);
				updateSwitch("wib_led", device["WIB_LED_ON"]);
				updateText("wib_potmeter", device["WIB_DIMMER_VALUE"]);
				break;
			default:
				break;
		}
	}
}

function updateStatus(elementId, newStatus) {
	if (newStatus == DeviceStatus.CONNECTED) {
		document.getElementById(elementId).innerHTML = "Connected";
		document.getElementById(elementId).className = "status_connected";
	} else if (newStatus == DeviceStatus.UNSTABLE) {
		document.getElementById(elementId).innerHTML = "Lost";
		document.getElementById(elementId).className = "status_lost";
	} else if (newStatus == DeviceStatus.DISCONNECTED) {
		document.getElementById(elementId).innerHTML = "Disconnected";
		document.getElementById(elementId).className = "status_disconnected";
	}
}

function updateText(elementId, newText) {
	document.getElementById(elementId).innerHTML = newText;
}

function updateSlider(elementId, newValue) {
	document.getElementById(elementId).value = newValue;
}

function updateSwitch(elementId, newState) {
	document.getElementById(elementId).checked = newState;
}

function isConnected(statusID) {
	if (document.getElementById(statusID).innerHTML === "Disconnected") {
		return false;
	}
	return true;
}

function forwardMessage(message) {
	var msg = {
		UUID: uuid,
		Type: type,
		command: WebsiteCommands.WEBSITE_FORWARD,
		value: message
	};
	socket.send(JSON.stringify(msg));
}

function sendSwitchValue(uuidID, type, statusID, switchID, command) {
	if (!isConnected(statusID)) return;

	switchElement = document.getElementById(switchID);
	var message = {
		UUID: document.getElementById(uuidID).innerHTML,
		Type: type,
		command: command,
		value: switchElement.checked
	};

	forwardMessage(message);
}

function sendSliderValue(uuidID, type, statusID, sliderID, command) {
	if (!isConnected(statusID)) return;

	sliderElement = document.getElementById(sliderID);
	var message = {
		UUID: document.getElementById(uuidID).innerHTML,
		Type: type,
		command: command,
		value: parseInt(sliderElement.value)
	};

	forwardMessage(message);
}

//Command types

const ErrorCodes =
{
	NOT_REGISTERED: 10,
	INVALID_FORMAT: 11
};

const GeneralDeviceCommands =
{
	HEARTBEAT: 20,
	REGISTRATION: 21,
	DEVICE_INFO: 22
};

const DeviceStatus =
{
	CONNECTED: 30,
	UNSTABLE: 31,
	DISCONNECTED: 32
};

const BedCommands =
{
	BED_BUTTON_PRESSED: 40,
	BED_LED_ON: 41,
	BED_PRESSURE_SENSOR_VALUE: 42
};

const ChairCommands =
{
	CHAIR_BUTTON_PRESSED: 50,
	CHAIR_LED_ON: 51,
	CHAIR_VIBRATOR_ON: 52,
	CHAIR_PRESSURE_SENSOR_VALUE: 53
};

const ColumnCommands =
{
	COLUMN_BUTTON_PRESSED : 60,
    COLUMN_LED_ON : 61,
    COLUMN_BUZZER_ON : 62,
    COLUMN_SMOKE_SENSOR_VALUE : 63,
    COLUMN_SMOKE_TRESHOLD_VALUE : 64
};

const DoorCommands =
{
	DOOR_BUTTON_INSIDE_PRESSED: 70,
	DOOR_BUTTON_OUTSIDE_PRESSED: 71,
	DOOR_LED_INSIDE_ON: 72,
	DOOR_LED_OUTSIDE_ON: 73,
	DOOR_DOOR_OPEN: 74,
	DOOR_DOOR_LOCKED: 75
};

const FridgeCommands =
{
	FRIDGE_DOOR_CLOSED: 80,
	FRIDGE_FAN_ON: 81,
	FRIDGE_RAW_TEMPERATURE_SENSOR_INSIDE_VALUE: 82,
	FRIDGE_RAW_TEMPERATURE_SENSOR_OUTSIDE_VALUE: 83,
	FRIDGE_COOLER_ON: 84,
	FRIDGE_REQUESTED_FRIDGE_TEMPERATURE: 85,
	FRIDGE_DOOR_OPEN_COUNTER: 86,
	FRIDGE_TEMPERATURE_INSIDE_VALUE: 87,
	FRIDGE_TEMPERATURE_OUTSIDE_VALUE: 88
};

const LampCommands =
{
	LAMP_MOVEMENT_DETECTED: 90,
	LAMP_LED_VALUE: 91,
	LAMP_LED_ON: 92
};

const SimulatedDeviceCommands =
{
	SIMULATED_BUTTON1_PRESSED: 100,
	SIMULATED_BUTTON2_PRESSED: 101,
	SIMULATED_LED1_VALUE: 102,
	SIMULATED_LED2_VALUE: 103,
	SIMULATED_LED3_VALUE: 104,
	SIMULATED_DIMMER_VALUE: 105
};

const WallCommands =
{
	WALL_CURTAIN_OPEN: 110,
	WALL_LEDSTRIP_ON: 111,
	WALL_LAMP_ON: 112,
	WALL_DIMMER_VALUE: 113,
	WALL_LDR_VALUE: 114,
	WALL_LEDSTRIP_VALUE: 115
};

const WebsiteCommands =
{
	WEBSITE_UPDATE: 120,
	WEBSITE_FORWARD: 121
};

const WIBCommands =
{
	WIB_SWITCH_ON: 130,
	WIB_LED_ON: 131,
	WIB_DIMMER_VALUE: 132
};