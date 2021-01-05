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
async function sendSocket(type, command) {
	var msg;
	var value = "";
	if (type == "Chair") {
		if (document.getElementById("chair_status").innerHTML === "Disconnected") return;
		if (command == 4001) {
			check = document.getElementById("chair_massage_switch");
			if (check.checked) {
				value = 1;
			}
			else {
				value = 0;
			}
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
		command: 1001,
		value: ""
	}
	socket.send(JSON.stringify(registration));
};

/*!
	@brief Trigger an event when receiving a message
*/
socket.onmessage = function (event) {
	var jsonMessage = JSON.parse(event.data);
	if (jsonMessage["Type"] == "Chair" && jsonMessage["command"] == 4000) {
		document.getElementById("chair_measured_weight").innerHTML = jsonMessage["value"];
	}
	else if (jsonMessage["command"] == 5000) {
		updateDeviceInformation(jsonMessage);
	}

	if (jsonMessage["command"] < 4000) {
		log(event.data);
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
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("fridge_status").innerHTML = "connected";
				break;
			case "Lamp":
				document.getElementById("lamp_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("lamp_status").innerHTML = "connected";
				break;
			case "Door":
				document.getElementById("door_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("door_status").innerHTML = "connected";
				break;
			case "Chair":
				document.getElementById("chair_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("chair_status").innerHTML = "connected";
				break;
			case "Bed":
				document.getElementById("bed_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("bed_status").innerHTML = "connected";
				break;
			case "Column":
				document.getElementById("column_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("column_status").innerHTML = "connected";
				break;
			case "Wall":
				document.getElementById("wall_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("wall_status").innerHTML = "connected";
				break;
			case "Simulation":
				document.getElementById("simulation_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("simulation_status").innerHTML = "connected";
				break;
			case "WIB":
				document.getElementById("wib_uuid").innerHTML = deviceInformation["value"][0]["UUID"];
				if (deviceInformation["value"][0]["Status"] == 2000)
					document.getElementById("wib_status").innerHTML = "connected";
				break;
			default:
				break;
		}
	}
}