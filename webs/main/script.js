$(document).ready(function () {
    socketInit();
    colorInit();
    animsInit();
});

function isJsonString(string) {
    try {JSON.parse(string);
    } catch (e) {return false;}
    return true;
}

var prevScrollpos = window.pageYOffset;
window.onscroll = function() {
    var currentScrollPos = window.pageYOffset;
    if (prevScrollpos > currentScrollPos) {
        $(".floating-container").fadeIn(200);
    } else {
        $(".floating-container").fadeOut(200);
    }
    prevScrollpos = currentScrollPos;
}