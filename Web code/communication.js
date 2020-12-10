var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.display === "block") {
      content.style.display = "none";
    } else {
      content.style.display = "block";
    }
  });
}

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