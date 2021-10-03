var lastFirmwareMessageMS = 0;
function processNewFirmware(message){
    var firmwareInfo = JSON.parse(message);
    let msNow = Date.now();
    if( lastFirmwareMessageMS - msNow >= 10000){
        lastFirmwareMessageMS = Date.now();
        console.log( "firmwareInfo: ", firmwareInfo );
    }
}