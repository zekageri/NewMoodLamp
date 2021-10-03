function colorInit(){
    makePicker();
    setSavedColor();
    colorToolOpenEvent();
    saveCurrentColorEvent();
    deleteCurrentColorEvent();
}

var RGB_Strip;

function makePicker(){
    RGB_Strip = new iro.ColorPicker(".colorPicker", {
        color: "rgb(255, 0, 0)",
        borderWidth: 1,
        borderColor: "#fff",
        id:"MainPicker",
        layout: [
            {
                component: iro.ui.Box,
            },
            {
                component: iro.ui.Slider,
                options: {
                    id: 'hue-slider',
                    sliderType: 'hue'
                }
            },
            {
                component: iro.ui.Slider,
                options: {
                    id: 'saturation-slider',
                    sliderType: 'saturation'
                }
            },
            {
                component: iro.ui.Slider,
                options: {
                    id: 'value-slider',
                    sliderType: 'value'
                }
            },
            {
                component: iro.ui.Slider,
                options: {
                    id: 'kelvin-slider',
                    sliderType: 'kelvin'
                }
            }
        ]
    });
    RGB_Strip.on('input:move', getColorFromEvent);
    RGB_Strip.on('input:end', getColorFromEvent);
}

function colorToolOpenEvent(){
    $(".colorToolOpenBtn").on("click", function () {
        if( !$(".colorTools").hasClass("active") ){
            $(".colorTools").addClass("active");
            $(".colorToolOpenBtn i").removeClass("open");
            setTimeout(() => {
                $(".toolBtns").fadeIn(200);
                $(".colorToolOpenBtn i").addClass("open");
            }, 500);
        }else{
            $(".colorTools").removeClass("active");
            $(".colorToolOpenBtn i").addClass("open");
            $(".toolBtns").hide();
            setTimeout(() => {
                $(".colorToolOpenBtn i").removeClass("open");
            }, 500);
        }
    });
}

var selectedSavedColorID;

function deleteCurrentColorEvent(){
    $(".colorToolBtn.delete").on("click", function () {
        if( !selectedSavedColorID ){
            console.warn("Válassz egy színt a törléshez!");
            return;
        }
        for (const colorObj of userProgram.savedColors ) {
            if( colorObj.id == selectedSavedColorID ){
                userProgram.savedColors.splice(userProgram.savedColors.indexOf(colorObj), 1);
                addSavedColors();
                saveUserProg();
                console.log("Szín kitörölve!");
                break;
            }
        }
    });
}

function saveCurrentColorEvent(){
    $(".colorToolBtn.copy").on("click", function () {
        if( !("savedColors" in userProgram) ){userProgram.savedColors = [];}
        if(userProgram.savedColors.length >= 20){
            console.warn("Nem tudok több színt eltárolni. :(");
            return;
        }
        userProgram.savedColors.push({
            color:RGB_Strip.color.hexString,
            id: uniqueId()
        });
        addSavedColors();
        saveUserProg();
    });
}

function addSavedColors(){
    if( "savedColors" in userProgram ){
        $(".colorOptions").empty();
        for (const colorObj of userProgram.savedColors ) {
            $(".colorOptions").append(`<div id="${colorObj.id}" class="savedColor" style="background-color:${colorObj.color}"></div>`);
        }
        setSavedColor();
    }
}

function setSavedColor(){
    $(".savedColor").off("click").on("click", function () {
        $(".savedColor").removeClass("activeColor");
        $(this).addClass("activeColor");
        let savedC = $(this).css("background-color").match(/\d+/g);
        RGB_Strip.color.rgb = {r:savedC[0],g:savedC[1],b:savedC[2]};
        getColorFromEvent();
        selectedSavedColorID = $(this).attr("id");
    });
}

function getColorFromEvent(){
    $(".checkbox-input").prop("checked", false);
    sendPacket({
        type:"rgb",
        r: RGB_Strip.color.rgb.r,
        g: RGB_Strip.color.rgb.g,
        b: RGB_Strip.color.rgb.b
    });
}