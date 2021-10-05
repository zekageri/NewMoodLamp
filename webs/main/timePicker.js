function initTimePickers(){
    initInlinePicker();
}

function hmsToSecondsOnly(str) {
    var p = str.split(':'),
        s = 0, m = 1;
    while (p.length > 0) {
        s += m * parseInt(p.pop(), 10);
        m *= 60;
    }
    return s;
}

var swOffPicker;
function initInlinePicker(){

    swOffPicker = new Picker(document.querySelector('.swOffBtn'), {
        format: 'HH:mm',
        date: new Date(0, 20),
        controls: true,
        text:{
            title: 'Visszaszámláló',
            cancel: 'Mégsem',
            confirm: 'OK',
            year: 'Év',
            month: 'Hónap',
            day: 'Nap',
            hour: 'Óra',
            minute: 'Perc',
            second: 'Másodperc',
            millisecond: 'Ms',
        },
        pick: function(e){
            startCountDown(swOffPicker.getDate(true)+":00");
        }
    });

}
