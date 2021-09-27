
function wifiScanInit(){
    getNetworks();
    rescanIcoHolderEvent();
}

var networks = [];

function showScan(){
    $(".scanningDone").hide();
    $(".scanning").show();
}

function hideScan(){
    $(".netCount").html(networks.length);
    $(".scanningDone").show();
    $(".scanning").hide();
}

function getNetworks(){
    showScan();
    $.get("/downFile",{"filename":"/networks.json"},
        function (data, textStatus, jqXHR) {
            if( isJsonString(data) ){
                $(".networks").empty();
                networks = JSON.parse(data);
                hideScan();
                assembleNetworks();
            }else{
                alert(data);
            }
        }
    );
}

function getWiFiColor(strength){
    if( strength == "Excellent" ){
        return "green";
    }else if(strength == "Good"){
        return "#FFCC00";
    }else if(strength == "Bad"){
        return "orange";
    }else if( strength == "Very bad"){
        return "red";
    }
}

function addLockIfEnc(network){
    if(network.encryption > 0){
        return `
        <div class="network-item wifiConnBtn">
            <svg fill="currentColor" class="wifiConn" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 64 64" >
                <g id="Layer_14" data-name="Layer 14"><path d="M26,46V38a3,3,0,0,0-6,0v8a5,5,0,0,0,1.81,8.84A5.37,5.37,0,0,0,23,55a5,5,0,0,0,3-9Zm-1.14,6.33a3,3,0,1,1-3.36-4.93,1,1,0,0,0,.5-.87V38a1,1,0,0,1,2,0v8.55a1,1,0,0,0,.5.87,3,3,0,0,1,.36,4.93Z"/>
                    <path d="M61.22,38l-3.31-.73a12.63,12.63,0,0,1-8-5.8,1,1,0,0,0-1.72,0A12.55,12.55,0,0,1,43,36.23V32a5,5,0,0,0-3-4.58V18A17,17,0,0,0,6,18v9.42A5,5,0,0,0,3,32v3H2a1,1,0,0,0-1,1V54a1,1,0,0,0,1,1H3v3a5,5,0,0,0,5,5H38a5,5,0,0,0,5-5v-.13a29.08,29.08,0,0,0,4.6,4.33l.8.6a1,1,0,0,0,1.2,0l.8-.6A29.15,29.15,0,0,0,62,39,1,1,0,0,0,61.22,38ZM8,18a15,15,0,0,1,30,0v9H34V18a11,11,0,0,0-22,0v9H8Zm24,9H14V18a9,9,0,0,1,18,0ZM13,37v4H3V37ZM3,53V49H13v4Zm10-6H3V43H13ZM41,58a3,3,0,0,1-3,3H8a3,3,0,0,1-3-3V55h9a1,1,0,0,0,1-1V36a1,1,0,0,0-1-1H5V32a3,3,0,0,1,3-3H38a3,3,0,0,1,3,3v5c-.3.09-.6.18-.91.25L36.78,38A1,1,0,0,0,36,39a29.14,29.14,0,0,0,5,16.24Zm8.2,2.6-.2.15-.2-.15A27.15,27.15,0,0,1,38,39.8l2.51-.56A14.63,14.63,0,0,0,49,33.79a14.63,14.63,0,0,0,8.48,5.45L60,39.8A27.15,27.15,0,0,1,49.2,60.6Z"/>
                    <path d="M54.89,40.45l-1.78-.9-.68,1.35A7.09,7.09,0,1,0,54,42.16ZM54,47a5.05,5.05,0,1,1-2.47-4.29l-2.7,5.4-2-2.71-1.6,1.2,3,4a1,1,0,0,0,.8.4h.09a1,1,0,0,0,.8-.55l3.17-6.34A4.92,4.92,0,0,1,54,47Z"/></g>
            </svg>
        </div>
        `;
    }
    return "";
}
function assembleNetwork(network){
    return `
    <div class="networkContainer" ssid="${network.SSID}">
        <div class="network-item wifiIconHolder" style="color:${getWiFiColor(network.strength)}">
            <svg fill="currentColor" class="wifiIco" enable-background="new 0 0 512.004 512.004" viewBox="0 0 512.004 512.004" xmlns="http://www.w3.org/2000/svg"><g><path d="m351.745 279.814c-57.416-39.197-133.618-39.506-191.486 0-17.297 11.807-19.546 36.542-4.685 51.402 11.358 11.36 29.359 13.013 42.809 3.901 34.664-23.489 80.445-23.575 115.237 0 23.752 16.095 55.153-2.656 52.518-30.836-.928-9.938-6.175-18.856-14.393-24.467zm-6.629 40.089c-5.951 5.951-15.423 6.778-22.52 1.969-40.071-27.152-92.959-27.259-133.187 0-7.098 4.811-16.569 3.983-22.52-1.969-7.777-7.777-6.621-20.723 2.392-26.874 52.103-35.572 121.144-35.706 173.444 0 9.029 6.164 10.152 19.113 2.391 26.874z"/><path d="m426.923 208.297c-99.579-80.373-242.1-80.505-341.843 0-15.489 12.502-16.677 35.751-2.591 49.836 12.158 12.157 31.44 13.172 44.855 2.358 74.693-60.212 182.119-60.617 257.315 0 22.558 18.186 56.201 1.119 54.655-27.923-.503-9.475-5.02-18.322-12.391-24.271zm-8.722 38.521c-6.363 6.364-16.466 6.888-23.5 1.216-80.533-64.918-196.326-65.353-277.398 0-7.035 5.672-17.137 5.147-23.5-1.216-7.21-7.21-6.958-19.385 1.327-26.07 93.388-75.375 227.724-75.886 321.745 0 8.288 6.689 8.532 18.865 1.326 26.07z"/><path d="m511.979 160.475c-.348-9.309-4.596-18.132-11.654-24.207-140.425-120.885-348.482-120.659-488.646-.001-7.058 6.076-11.306 14.899-11.654 24.208-1.095 29.323 33.404 45.696 55.533 26.657 115.21-99.139 285.892-98.953 400.887 0 22.163 19.067 56.628 2.633 55.534-26.657zm-21.13 13.695c-6.467 6.469-16.995 6.833-23.967.833-121.201-104.291-300.757-104.121-421.759 0-6.973 6.002-17.501 5.635-23.968-.833-7.201-7.2-6.846-19.054.963-25.777 134.42-115.714 333.599-115.499 467.769.001 7.717 6.642 8.248 18.491.962 25.776z"/><path d="m256.002 466.315c-30.878 0-56-25.121-56-56s25.122-56 56-56 56 25.121 56 56-25.122 56-56 56zm0-96c-22.056 0-40 17.944-40 40s17.944 40 40 40 40-17.944 40-40-17.944-40-40-40z"/></g></svg>
        </div>
        <div class="network-item wifiName">
            ${network.SSID}
        </div>
        ${addLockIfEnc(network)}
    </div>`;
}

function rescanIcoHolderEvent(){
    $(".rescanIcoHolder").on("click", function () {
        showScan();
        $.get("/scanNetworks",
            function (data, textStatus, jqXHR) {
                if(data == "scanned"){
                    getNetworks();
                }
            }
        );
    });
}

function assembleNetworks(){
    for (const network of networks) {
        $(".networks").append( assembleNetwork(network) );
    }
    connectBtnEvent();
}


function connectBtnEvent(){
    $(".networkContainer").off("click").on("click", function () {
        $(".networkContainer").removeClass("boxClick");
        $(this).addClass("boxClick");
        let SSID = $(this).attr("ssid");
        setTimeout(() => {
            let pass = window.prompt("Enter network password");
            if( pass ){
                console.log("Connecting to: ", SSID, "with password: ", pass);
                $.get("/newNetworkData",{SSID:SSID,pass:pass},
                    function (data, textStatus, jqXHR) {
                        if(data == "saved"){
                            console.log("Network creds saved. Lamp restarting...");
                        }
                    }
                );
            }
        }, 150);
    });
}