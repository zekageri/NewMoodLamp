#ifndef serverHandling_h
#define serverHandling_h

static const inline void endPoints();

const char* gzippedFiles[] = {
    "/index.html",
    "/index-ap.html",
    "/mainScript.bundle.min.js",
    "/mainStyle.bundle.min.css",
    "/apStyle.bundle.min.css",
    "/apScript.bundle.min.js",
    "/jquery3.6.0.min.js",
    "/NotFound.html"
};

static const inline boolean isGzipped(const char fileName[]) {
    for (int i = 0; i < sizeof(gzippedFiles) / sizeof(gzippedFiles[0]); i++) {
        if (strcmp(fileName, gzippedFiles[i]) == 0) { return true; }
    }
    return false;
}

static const inline String checkPath(String path) {
    if( !path.startsWith("/") ) {path = "/" + path;}
    if (LITTLEFS.exists(path)) { return path; }
    return "/NotFound.html";
}

static const inline String getCookie(String name, String cookie) {
    if (cookie.indexOf(name) == -1) { return ""; }
    name += "=";
    int length = name.length();
    int startPos = cookie.indexOf(name) + length;
    int stopPos = cookie.indexOf(";", startPos);

    return cookie.substring(startPos, stopPos);
}

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {
    endPoints();
  }
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    String mainPath = "/index-ap.html";
    AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(mainPath), "text/html");
    if( isGzipped (mainPath.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
    request->send(response);
  }
};

static const inline void setupCaptiveDNS(){
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", WiFi.softAPIP());
}
static const inline void initServer(){
    if( isAPMode ){
        Serial.println("Init captive portal and dns...");
        setupCaptiveDNS();
        server.addHandler(new CaptiveRequestHandler()); //.setFilter(ON_AP_FILTER)
    }else{
        endPoints();
    }
    server.begin();
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
}

static const inline void endPoints(){

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String mainPath = "/index.html";
        if(isAPMode){ mainPath = "/index-ap.html"; }
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(mainPath), "text/html");
        if( isGzipped (mainPath.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/mainStyle.bundle.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/mainStyle.bundle.min.css";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/css");
        if( isGzipped (path.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/mainScript.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/mainScript.bundle.min.js";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/javascript");
        if( isGzipped (path.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/jquery3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/jquery3.6.0.min.js";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/javascript");
        if( isGzipped (path.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });
    
    server.on("/apStyle.bundle.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/apStyle.bundle.min.css";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/css");
        if( isGzipped (path.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/apScript.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/apScript.bundle.min.js";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/javascript");
        if( isGzipped (path.c_str()) ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/networks.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/networks.json";
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath(path), "text/json");
        request->send(response);
    });
    
    server.on("/scanNetworks", HTTP_GET, [](AsyncWebServerRequest *request) {
        scanWiFi();
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "scanned");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    server.on("/newNetworkData", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebParameter* p = request->getParam("SSID");
        String SSID = p->value();
        p = request->getParam("pass");
        String password = p->value();
        strlcpy(config.wifiPass,password.c_str(),sizeof(config.wifiPass));
        strlcpy(config.wifiSSID,SSID.c_str(),sizeof(config.wifiSSID));
        makeConfig();
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "saved");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
        canRestart = true;
    });

    server.on("/downFile", HTTP_GET, [](AsyncWebServerRequest* request) {
        AsyncWebParameter* p = request->getParam(0);
        AsyncWebServerResponse* response;
        const char* fileBuff = p->value().c_str();
        size_t fileBuff_Size = strlen(fileBuff);
        if (fileBuff_Size > 1) {
            boolean isExists = LITTLEFS.exists(fileBuff);
            if ( isExists ) {
                response = request->beginResponse(LITTLEFS, fileBuff, "text/plain", true);
                if (isGzipped(fileBuff)) {
                    response->addHeader("Content-Encoding", "gzip");
                    response->addHeader("Cache-Control", "max-age=31536000");
                }
                request->send(response);
            } else {
                response = request->beginResponse(204, "text/plain", "Nincs ilyen file");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
            }
        } else {
            request->send(500);
        }
    });

    server.serveStatic("/", LITTLEFS, "/").setCacheControl("max-age=30536000");

    server.onNotFound([](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/NotFound.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=31536000");
        request->send(response);
    });
}

boolean uploadError = false;
int httpStatus = 200;
String webResponse = "File Uploaded ";
inline void resetVariables() {
    uploadError = false;
    httpStatus = 200;
    webResponse = "File Uploaded ";
}

void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
        resetVariables();
        webResponse += filename;
        if (!filename.startsWith("/"))
            filename = "/" + filename;
        if (LITTLEFS.exists(filename)) {
            LITTLEFS.remove(filename);
        }
        uploadFile = LITTLEFS.open(filename, "w");
        if (!uploadFile) {
            httpStatus = 500;
            webResponse = "File NOT Uploaded " + filename;
        }
    }
    for (size_t i = 0; i < len; i++) {
        uploadFile.write(data[i]);
    }
    if (final) {
        uploadFile.close();
        AsyncWebServerResponse *response = request->beginResponse(httpStatus, "text/plain", webResponse);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }
}
#endif