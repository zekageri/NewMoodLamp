#ifndef serverHandling_h
#define serverHandling_h

static const inline void endPoints();

const char* gzippedFiles[] = {
    "/index.html",
    "/index-ap.html",
    "/mainScript.bundle.min.js",
    "/main.css",
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

static const inline void initServer(){
    endPoints();
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

    server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath("/main.css"), "text/css");
        if( isGzipped ("/main.css") ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
    });

    server.on("/mainScript.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, checkPath("/mainScript.bundle.min.js"), "text/javascript");
        if( isGzipped ("/mainScript.bundle.min.js") ){ response->addHeader("Content-Encoding", "gzip"); }
        request->send(response);
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