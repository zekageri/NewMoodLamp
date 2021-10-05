
var countDownDate = new Date("Sep 25, 2025 15:00:00").getTime();
var countDownTimer;


function stopCountDown(){
    clearInterval(countDownTimer);
}

function startCountDown(time){
    stopCountDown();
    let ms = hmsToSecondsOnly(time)*1000;
    sendPacket({"type":"countDown","time":ms});
    setCountDownTime(ms);
    countdownTimeStart();
    $(".swOffBtn").addClass("runningCountDown");
}

function setCountDownTime(ms){
    countDownDate = new Date().getTime() + ms;
}

function countdownTimeStart(){
    countDownTimer = setInterval(function() {
        var now = new Date().getTime();
        var distance = countDownDate - now;
        var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
        var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
        var seconds = Math.floor((distance % (1000 * 60)) / 1000);
        if( hours < 10 ){hours = "0" + hours;}
        if( minutes < 10 ){minutes = "0" + minutes;}
        if( seconds < 10 ){seconds = "0" + seconds;}
        $("#hourTimer").text(hours);
        $("#minTimer").text(minutes);
        $("#secTimer").text(seconds);
        if (distance < 0) {
            clearInterval(countDownTimer);
            $(".swOffBtn").removeCLass("runningCountDown");
        }
    }, 1000);
}