
var countDownDate = new Date("Sep 25, 2025 15:00:00").getTime();

function countDownInit(){
    setCountDownTime(4600000);
    countdownTimeStart();
}

function setCountDownTime(setSec){
    countDownDate = new Date().getTime() + (setSec*1000);
}

function countdownTimeStart(){
    var x = setInterval(function() {
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
            clearInterval(x);
            console.log("Timer hit!");
        }
    }, 1000);
}