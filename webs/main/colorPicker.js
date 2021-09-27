function colorInit(){
    makePicker();
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

function getColorFromEvent(){
    $(".checkbox-input").prop("checked", false);
    sendPacket({
        type:"rgb",
        r: RGB_Strip.color.rgb.r,
        g: RGB_Strip.color.rgb.g,
        b: RGB_Strip.color.rgb.b
    });
}