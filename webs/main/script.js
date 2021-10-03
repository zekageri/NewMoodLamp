$(document).ready(function () {
    socketInit();
    colorInit();
    animsInit();
    initNav();
    countDownInit();
});

var userProgram = {};
var userProgSaver;

function saveUserProg(){
    clearTimeout(userProgSaver);
    userProgSaver = setTimeout(() => {
        sendPacket({
            type: "saveUserProg",
            prog: JSON.stringify(userProgram)
        });
    }, 1000);
}

function uniqueId() {
    var idStrLen = 32;
    var idStr = (Math.floor((Math.random() * 25)) + 10).toString(36) + "_";
    idStr += (new Date()).getTime().toString(36) + "_";
    do {idStr += (Math.floor((Math.random() * 35))).toString(36);
    } while (idStr.length < idStrLen);
    return (idStr);
}

function initNav(){
    $(".float-element").on("click", function () {
        let currentPage = $(".activePage").attr("href");
        let pageID = $(this).attr("href");
        if( pageID != currentPage ){
            $(".page").hide();
            $(pageID).fadeIn(200);
            $(".activePage").removeClass("activePage");
            $(this).addClass("activePage");
        }
    });
}

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