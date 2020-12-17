var socket = new WebSocket("ws://169.254.41.114:9002"); //change the ip address to the ip address of your pi! - also works when using live server

function log(logstr) {
	document.getElementById("log").innerHTML += logstr + "\n";
}

async function sendSocket(UUID, type, command, value) {
	var msg = {
		UUID: 		UUID,
		Type: 		type,
		command:	command,
		value: 		value
	};
	socket.send(JSON.stringify(msg, null, 2));
}

socket.onopen = function (event) {
	socket.send("Here's some text that the server is urgently awaiting!");
};

socket.onmessage = function (event) {
	log(event.data);
}