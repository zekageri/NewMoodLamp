#ifndef webSocket_h
#define webSocket_h

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println("A socket client connected");
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
        }else{
            Serial.print("Unknown packet type: ");
            Serial.println(type);
            Serial.print("Message: ");
            serializeJsonPretty(packet, Serial);
            Serial.println("");
        }
    }
}

#endif