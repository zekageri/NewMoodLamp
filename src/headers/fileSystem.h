#ifndef fileSystem_h
#define fileSystem_h

static const inline void makeConfig();

/* FILES */
const char *Config      = "/Config.json";
const char *ERRORLOG    = "/ErrorLog.json";
const char *fsLog       = "/fsLog.json";
/* FILES */

String filesToCheck[] = {ERRORLOG,fsLog};
#define MAX_LOG_SIZE 50000

static const inline void errorLog(String msg){
    File f = LITTLEFS.open(ERRORLOG, "a");
    if(f){
        String objHolder = "";
        StaticJsonDocument<1000> doc;
        doc["msg"]      = msg;
        doc["millis"]   = millis();
        doc["time"]     = Time.GetCurrentTime();
        serializeJson(doc, objHolder);
        objHolder += ",";
        f.print(objHolder);
    }
    f.close();
}

static const inline void addToLog(String msg){
    File f = LITTLEFS.open(fsLog, "a");
    if(f){
        String objHolder = "";
        StaticJsonDocument<1000> doc;
        doc["msg"]      = msg;
        doc["millis"]   = millis();
        doc["time"]     = Time.GetCurrentTime();
        serializeJson(doc, objHolder);
        objHolder += ",";
        f.print(objHolder);
    }
    f.close();
}

static const inline void fileCrawler(){
    EVERY_N_HOURS(1){
        int arrLength = (sizeof(filesToCheck) / sizeof(filesToCheck[0]));
        for(byte i = 0; i < arrLength;i++){
            if(LITTLEFS.exists(filesToCheck[i])){
                File FILE = LITTLEFS.open(filesToCheck[i]);
                if(FILE.size() >= MAX_LOG_SIZE){
                    FILE.close();
                    LITTLEFS.remove(filesToCheck[i]);
                    addToLog("\""+filesToCheck[i]+"\" file törölve");
                }else{
                    FILE.close();
                }
            }
        }
    }
}

static const inline void printConfigInfo(){
    Serial.println("\n********************* CONFIG *************************");
    Serial.print("\t-Hostname: ");
    Serial.println(config.hostname);
    Serial.print("\t-AP ssid: ");
    Serial.println(config.APssid);
    Serial.print("\t-AP password: ");
    Serial.println(config.APpassword);
    Serial.print("\t-wifi Pass: ");
    Serial.println(config.wifiPass);
    Serial.print("\t-wifi SSID: ");
    Serial.println(config.wifiSSID);
    Serial.print("\t-ip:");
    Serial.println(config.ip);
    Serial.print("\t-gateway:");
    Serial.println(config.gw);
    Serial.print("\t-subnet:");
    Serial.println(config.sub);
    Serial.print("\t-firmware Version: ");
    Serial.println(config.firmwareVersion);
    Serial.println("**********************************************\n");
}

static const inline void getConfig(){
    if(LITTLEFS.exists(Config)){
        File file = LITTLEFS.open(Config,"r");
        StaticJsonDocument<1000> doc;
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            Serial.print("Config deserialization error. Code: ");
            Serial.println(error.c_str());
        }else{
            strlcpy(config.hostname,doc["hostname"] | config.hostname,sizeof(config.hostname));
            strlcpy(config.APssid,doc["APssid"]     | config.APssid,sizeof(config.APssid));
            strlcpy(config.APpassword,doc["APpassword"] | config.APpassword,sizeof(config.APpassword));
            strlcpy(config.wifiPass,doc["wifiPass"] | config.wifiPass,sizeof(config.wifiPass));
            strlcpy(config.wifiSSID,doc["wifiSSID"] | config.wifiSSID,sizeof(config.wifiSSID));

            String ip  = doc["ip"].as<String>();  config.ip.fromString(ip);
            String sub = doc["sub"].as<String>(); config.sub.fromString(sub);
            String gw  = doc["gw"].as<String>();  config.gw.fromString(gw);

            config.firmwareVersion = doc["firmwareVersion"] | 1;
            config.dhcpIsOn = doc["dhcpIsOn"] | true;

            printConfigInfo();
        }
        file.close();
    }else{
        makeConfig();
    }
}

static const inline void makeConfig(){
    File file = LITTLEFS.open(Config,"w");
    Serial.println("Making new config...");
    if (file) {
        StaticJsonDocument<1000> doc;

        doc["hostname"]     = config.hostname;
        doc["APssid"]       = config.APssid;
        doc["APpassword"]   = config.APpassword;
        doc["wifiPass"]     = config.wifiPass;
        doc["wifiSSID"]     = config.wifiSSID;
        doc["dhcpIsOn"]     = config.dhcpIsOn;
        doc["ip"]           = config.ip.toString();
        doc["sub"]          = config.sub.toString();
        doc["gw"]           = config.gw.toString();
        doc["firmwareVersion"] = config.firmwareVersion;

        if (serializeJson(doc, file) == 0) {
            Serial.println(F("Failed to write to file"));
            errorLog("Failed to write to cofnig file");
        }
    }else{
        Serial.println("Can't open config file for write...");
        errorLog("Can't open config file for write...");
    }
    file.close();
}

static const inline void checkForNewFirmware(){

}

static const inline void handleNewFirmware(){
    if( LITTLEFS.exists("/firmware.bin") ){
        File file = LITTLEFS.open("/firmware.bin");
        boolean hasError = false;
        if(!file){
            Serial.println("Failed to open file for reading");
            hasError = true;
        }
        if( !hasError ){
            Serial.println("Starting update..");
            size_t fileSize = file.size();
            if( !Update.begin(fileSize) ){
                Serial.println("Cannot do the update");
                errorLog("Cannot do the update. Wrong file size");
                hasError = true;
            };
            if( !hasError ){
                Update.writeStream(file);
                if( Update.end() ){
                    Serial.println("Successful update");
                    errorLog("Firmware update successful");
                }else {
                    Serial.println("Error Occurred: " + String(Update.getError()));
                    errorLog("Error Occurred: " + String(Update.getError()));
                    hasError = true;
                }
            }
        }
        file.close();
        if(!hasError){
            config.firmwareVersion = config.firmwareVersion++;
            makeConfig();
            vTaskDelay(500);
            ESP.restart();
        }
    }
}

#endif