#include <headers/includes.h>

void setup() {
    Serial.begin(115200);
    vTaskDelay(100);
    Wire.begin(SDA,SCL,400000);
    LITTLEFS.begin();
    initTasks();
    getConfig();
    initWiFi();
    initServer();
    //scanWiFi();
    //makeConfig();
}

void loop() {
    fileCrawler();
    checkWiFi();
    checkWiFiStrength();
    restart();
    dnsServer.processNextRequest(); // <-- for captiveportal?
}