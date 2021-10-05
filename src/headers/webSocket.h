#ifndef webSocket_h
#define webSocket_h

static const inline void clearSocketClients(){
    EVERY_N_SECONDS(30){
        ws.cleanupClients();
    }
}

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("A socket client connected");
        if(newFirmwareAvailable){
            sendSocket("firmware",newFirmwareInfo);
        }
        lastClientID = client->id();
        canSendUserProg = true;
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("A socket client disconnected");
    }else if (type == WS_EVT_DATA) {
        StaticJsonDocument<1000> packet;
        deserializeJson(packet, data, len);
        String type = packet["type"];
        if (type == "rgb") {
            light.setRGB(
                packet["r"],
                packet["g"],
                packet["b"]
            );
        }else if( type == "anim" ){
            light.setAnimation( packet["mode"] ) ;
        }else if( type == "saveUserProg" ){
            saveUserProg( packet["prog"] );
        }else if( type == "countDown"){
            startCountDown( packet["time"] );
        }else{
            Serial.print("Unknown packet type: ");
            Serial.println(type);
            Serial.print("Message: ");
            serializeJsonPretty(packet, Serial);
            Serial.println("");
        }
    }
}

static const inline void sendSocket(String type, String msg, uint32_t clientID){
    if( ws.count() > 0 ){
        StaticJsonDocument<5000> packet;
        packet["type"] = type;
        packet["msg"] = msg;
        String json;
        serializeJson(packet, json);
        if(clientID != 0){
            ws.text(clientID, json);
        }else{
            ws.textAll(json);
        }
    }
}

#endif