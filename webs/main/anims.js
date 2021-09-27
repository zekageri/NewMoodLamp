function animsInit(){
    animSelect();
}

function animSelect(){
    $(".checkbox-input").on("click", function (e) {
        e.preventDefault();
        $(".checkbox-input").prop("checked", false);
        setTimeout(() => {
            let mode = $(this).attr("animMode");
            if($(this).prop("checked")){
                mode = 0;
                $(this).prop("checked", false);
            }else{
                $(this).prop("checked", true);
            }
            setAnimation(mode);
        }, 10);
    });
}

function setAnimation(mode){
    sendPacket({
        "type": "anim",
        "mode": mode
    });
}