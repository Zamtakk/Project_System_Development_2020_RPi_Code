var pjsdvSocket = new WebSocket("wss://127.0.0.1:9002"); //change websocket address for address on the pi websocket server

function log(logstr) {   
    document.getElementById("log").innerHTML +=logstr+"\n";
}

async function sendSocket() {
    var msg = {
        type: "message",
        text: document.getElementById("texttosend").value
    };

    pjsdvSocket.send(JSON.stringify(msg));
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