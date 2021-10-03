
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
        socketIsConnected = true;
        socketIsConnecting = false;
        $(".connectionIndicator").addClass("connected");
    }
    socket.onclose = function(event){
        socketIsConnected = false;
        socketIsConnecting = false;
        $(".connectionIndicator").removeClass("connected");
    }
    socket.onmessage = function(event){
        processSocketData(event.data);
    }
}

function processSocketData(data){
    let packet = {type: "Not json",msg: data};
    if(isJsonString(data)){ packet = JSON.parse(data); }
    switch(packet.type){
        case "firmware":
            processNewFirmware(packet.msg);
            break;
        case "userProg":
            processUserProgram(packet.msg);
            break;
        case "progInfo":
            processProgramInfo(packet.msg);
            break;
        default:
            console.log("Unknown packet type: " + packet.type, "msg: ", packet.content);
    }
}

function processProgramInfo(msg){
    if(msg == "saved"){
        console.log("Program saved");
    }else{
        console.log("Program not saved");
    }
}

function processUserProgram(data){
    if( isJsonString(data) ){
        userProgram = JSON.parse(data);
        addSavedColors();
    }else{
        console.warn("Nem tudtam betölteni a felhasználói programot. :(");
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