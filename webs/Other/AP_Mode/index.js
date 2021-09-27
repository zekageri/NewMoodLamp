$(document).ready(function () {
    wifiScanInit();
});

function isJsonString(string) {
    try {JSON.parse(string);
    } catch (e) {return false;}
    return true;
}