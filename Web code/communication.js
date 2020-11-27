var pjsdvSocket;

function log(logstr) {   
    document.getElementById("log").innerHTML +=logstr+"\n";
}

async function sendSocket() {
    var text = document.getElementById("texttosend").value;
    pjsdvSocket = new WebSocket("wss://*", "protocolName"); //ToDo: set correct websocket adres + protocol name
    sendText();
}

function sendText() {
    var msg = {
        type: "message",
        text: document.getElementById("texttosend").value
    };

    pjsdvSocket.send(JSON.stringify(msg));

}

pjsdvSocket.onmessage = function (event) {
    log(event.data);
}