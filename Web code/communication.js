var pjsdvSocket = new WebSocket("ws://127.0.0.1:9002", "protocolOne");

function log(logstr) {   
    document.getElementById("log").innerHTML +=logstr+"\n";
}

async function sendSocket() {
    var text = document.getElementById("texttosend").value;
    sendText();
}

function sendText() {
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