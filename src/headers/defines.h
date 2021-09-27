#ifndef defines_h
#define defines_h

#define ever (;;)
#define FS_NO_GLOBALS
#define U_LITTLEFS 100
#define SERVER_PORT 80
boolean isAPMode    = false;
boolean canRestart  = false;

int ADMIN_CODE = 0;

/* captive portal dns? */
DNSServer dnsServer;
/* captive portal dns? */

/* rtc things */
RTC_DS3231 rtc;
#define SCL 22
#define SDA 21
/* rtc things */

/* led things */
#define LED_PIN     27
#define NUM_LEDS    26
#define CHIPSET     WS2812
#define COLOR_ORDER GRB
/* led things */

boolean canCalculateSunTimes = false;
boolean wifiConnected = false;

/* server things */
AsyncWebServer server(SERVER_PORT);
AsyncWebServerRequest *request;
AsyncWebSocket ws("/moodLamp");
AsyncWebSocketClient * globalClient = NULL;
File uploadFile;

struct Config {
    char hostname[32]   = "myLamp";
    char APssid[32]     = "Moodlamp";
    char APpassword[32] = "";

    char wifiSSID[32]   = "Emelet";
    char wifiPass[32]   = "Administrator";

    int firmwareVersion = 1;
    boolean dhcpIsOn = true;
    
    String lastWiFiStrength = "Good";
    
    IPAddress ip    = IPAddress(192, 168, 0, 33);
    IPAddress sub   = IPAddress(255, 255, 248, 0);
    IPAddress gw    = IPAddress(192, 168, 0, 1);
    IPAddress dns1  = IPAddress(8,8,8,8);
    IPAddress dns2  = IPAddress(192,168,0,1);

};
Config config;


/* server things */

xTaskHandle Time_System_Task_Handle,lightTask_Handle;
void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);

/* server things */

/* File system defines */
static const inline void errorLog(String msg);
static const inline void addToLog(String msg);
static const inline void scanWiFi();
/* File system defines */

#endif 
