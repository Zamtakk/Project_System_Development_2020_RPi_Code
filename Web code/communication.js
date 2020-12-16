var pjsdvSocket = new WebSocket("ws://169.254.41.114:9002"); //change the ip address to the ip address of your pi! - also works when using live server

function log(logstr) {
	document.getElementById("log").innerHTML += logstr + "\n";
}

async function sendSocketTest() {
	var msg = document.getElementById("texttosend").value;

	pjsdvSocket.send(msg);

	document.getElementById("texttosend").value = "";
}

pjsdvSocket.onopen = function (event) {
	pjsdvSocket.send("Here's some text that the server is urgently awaiting!");
};

pjsdvSocket.onmessage = function (event) {
	log(event.data);
}

function setStatus() {
	var toggleswitch = document.getElementById("switchstatus");
	var doortoggleswitch = document.getElementById("switchdoorstatus");
	if (toggleswitch.checked == true) {
		document.getElementById("statusvalue").innerHTML = "open";
	} else {
		document.getElementById("statusvalue").innerHTML = "dicht";
	}

	if (doortoggleswitch.checked == true) {
		document.getElementById("doorstatusvalue").innerHTML = "open";
	} else {
		document.getElementById("doorstatusvalue").innerHTML = "dicht";
	}
}