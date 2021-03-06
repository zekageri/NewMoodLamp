#ifndef wifiHandling_h
#define wifiHandling_h

static const inline String getMeaningFullRSSI(int wifiStrength);

#define wifiTryConnections 3
#define wifiTry_MS 3000
byte wifiConnTryCount = 0;

static const inline void printWifiDetails(){
    Serial.println("************** WIFI *********************");
    Serial.print("\t-WiFi Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.print("\t-Current IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("\t-WiFi Strength: ");
    Serial.println(WiFi.RSSI());
    Serial.println("***********************************");
}

static const inline void startMDNS(){
    if( !MDNS.begin( config.hostname ) ) {
        Serial.println("Error starting mDNS\n");
        errorLog("Error starting mDNS");
        return;
    }else{
        Serial.print("mDNS started with hostname: ");
        Serial.println(config.hostname);
        Serial.println("");
    }
}

static const inline void configWiFi(){
    WiFi.setHostname(config.hostname);
    if( !config.dhcpIsOn ){
        if (!WiFi.config(config.ip, config.gw, config.sub, config.dns1, config.dns2)) {
            Serial.println("STA Failed to configure");
        }
    }
}

static const inline void scanWiFi(){
    Serial.println("\nScanning for networks...");
    light.setLoading(true);
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        File f = LITTLEFS.open(networks, "w");
        StaticJsonDocument<1500> doc;
        JsonArray array = doc.to<JsonArray>();
        for (int i = 0; i < n; ++i) {
            JsonObject network = array.createNestedObject();
            network["SSID"] = WiFi.SSID(i);
            network["RSSI"] = WiFi.RSSI(i);
            network["strength"]     = getMeaningFullRSSI(WiFi.RSSI(i));
            network["encryption"]   = WiFi.encryptionType(i);
        }
        serializeJson(doc, f);
        serializeJsonPretty(doc, Serial);
        Serial.println("");
        light.setLoading(false);
    }
}

static const inline boolean connectToWiFi(){
    configWiFi();
    WiFi.begin(config.wifiSSID, config.wifiPass);
    long wifiConnStart = millis();
    while ( WiFi.status() != WL_CONNECTED ) {
        if( millis() - wifiConnStart >= wifiTry_MS ){
            vTaskDelay(500);
            Serial.println("WiFi connection failed. Try again...");
            break;
        }
    }
    if( WiFi.status() != WL_CONNECTED ){
        wifiConnTryCount++;
        if(wifiConnTryCount >= wifiTryConnections){return false;}
        connectToWiFi();
    }else{
        startMDNS();
        printWifiDetails();
        wifiConnTryCount = 0;
        isAPMode         = false;
        wifiConnected    = true;
        light.success();
        return true;
    }
}

static const inline String getMeaningFullRSSI(int wifiStrength){
        if( wifiStrength <= -90 ){
            return "Very bad";
        }else if( wifiStrength <= -65 && wifiStrength >= -90 ){
            return "Bad";
        }else if ( wifiStrength <= -41 && wifiStrength >= -65 ){
            return "Good";
        }else if( wifiStrength >= -41 ){
            return "Excellent";
        }
        return String(wifiStrength);
}

static const inline void checkWiFiStrength(){
    EVERY_N_MINUTES(1){
        if( !isAPMode ){
            config.lastWiFiStrength = getMeaningFullRSSI(WiFi.RSSI());
            Serial.print("\nWiFi strength is ");
            Serial.println(config.lastWiFiStrength);
        }
    }
}

IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);

static const inline void makeAP(){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(config.APssid);
    delay(500);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    isAPMode        = true;
    wifiConnected   = false;
    light.warning();
}

static const inline void initWiFi(){
    WiFi.mode(WIFI_STA);
    if( !connectToWiFi() ){ makeAP(); scanWiFi(); }
}

static const inline void checkWiFi(){
    EVERY_N_SECONDS(5){
        if( WiFi.status() != WL_CONNECTED && !isAPMode ){
            WiFi.disconnect();
            WiFi.reconnect();
        }
    }
}
#endif

