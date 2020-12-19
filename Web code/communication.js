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
async function sendSocket(UUID, type, command, value) {
	var msg = {
		UUID: UUID,
		Type: type,
		command: command,
		value: value
	};
	socket.send(JSON.stringify(msg, null, 2));
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
	socket.send(JSON.stringify(registration, null, 2));
};

/*!
	@brief Trigger an event when receiving a message
*/
socket.onmessage = function (event) {
	log(event.data);
}

async function sendString() {
	socket.send(document.getElementById("demoinput").value);
}