$(document).ready(function () {
    socketInit();
    colorInit();
});

function isJsonString(string) {
    try {JSON.parse(string);
    } catch (e) {return false;}
    return true;
}