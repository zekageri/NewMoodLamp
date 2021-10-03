#include <headers/includes.h>

void setup() {
    Serial.begin(115200);
    vTaskDelay(100);
    Wire.begin(SDA,SCL,400000);
    LITTLEFS.begin();
    initTasks();
    getUserProgram();
    getConfig();
    initWiFi();
    initServer();
    //scanWiFi();
    //makeConfig();
}

void loop() {
    checkForNewFirmware();
    fileCrawler();
    checkWiFi();
    checkWiFiStrength();
    restart();
    clearSocketClients();
    sendUserProgram();
    dnsServer.processNextRequest(); // <-- for captiveportal?
}