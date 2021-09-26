
var socket;
var socketIsConnected = false;
var socketConnectionCount = 0;
var socketIsConnecting = false;

function socketInit(){
    connectWebsocket();
    checkSocketConnection();
}

function checkSocketConnection(){
    setInterval(() => {
        if( !socketIsConnected ){
            if(socketConnectionCount < 3){
                socketConnectionCount++;
                connectWebsocket();
            }else if(socketConnectionCount >= 3 && !socketIsConnected && !socketIsConnecting){
                setTimeout(() => {
                    socketConnectionCount = 0;
                }, 10000);
            }
        }
    }, 1000);
}

function connectWebsocket(){
    socket = new WebSocket('ws://' + window.location.hostname + '/moodLamp');
    socketIsConnecting = true;
    socket.onopen = function(event){
        console.log("Socket connected");
        socketIsConnected = true;
        socketIsConnecting = false;
    }
    socket.onclose = function(event){
        console.log("Socket closed");
        socketIsConnected = false;
        socketIsConnecting = false;
    }
    socket.onmessage = function(event){
        processSocketData(event.data);
    }
}

function processSocketData(data){
    let packet = {type: "Not json",content: data};
    if(isJsonString(data)){
        packet = JSON.parse(data);
    }
    switch(packet.type){
        case "message":
            console.log(packet.content);
            break;
        default:
            console.log("Unknown packet type: " + packet.type, "content: ", packet.content);
    }
}

function sendPacket(packetIncoming){
    if(socketIsConnected){
        let packet = JSON.stringify(packetIncoming);
        socket.send( packet );
    }else{
        console.log("Socket is not connected");
    }
}