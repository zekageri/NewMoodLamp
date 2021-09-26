#ifndef light_h
#define light_h

#define SMOOTHERSTEP(x) ((x) * (x) * (x) * ((x) * ((x) * 6 - 15) + 10))

class rgbLight{
    CRGB leds[NUM_LEDS];

    boolean isOn = false;
    int lastRed     = 0;
    int lastGreen   = 0;
    int lastBlue    = 0;
    int brightness  = 0;
    byte animMode   = 0;

    private:
            float pulseSpeed = 0.5;  // Larger value gives faster pulse.
            uint8_t hueA = 15;
            uint8_t satA = 230;
            float valueMin = 120.0;
            uint8_t hueB = 95;
            uint8_t satB = 255;
            float valueMax = 255.0;
            uint8_t hue = hueA;
            uint8_t sat = satA;
            float val   = valueMin;

            inline void pulsateColorChange(){
                float delta = (valueMax - valueMin) / 2.35040238;
                float dV = ((exp(sin(pulseSpeed * millis()/2000.0*PI)) -0.36787944) * delta);
                val = valueMin + dV;
                hue = map(val, valueMin, valueMax, hueA, hueB);
                sat = map(val, valueMin, valueMax, satA, satB);
                for (int i = 0; i < NUM_LEDS; i++) {
                    leds[i] = CHSV(hue, sat, val);
                    leds[i].r = dim8_video(leds[i].r);
                    leds[i].g = dim8_video(leds[i].g);
                    leds[i].b = dim8_video(leds[i].b);
                }
            }

            float inverse_smoothstep(float x) {
                return 0.5 - sin(asin(1 - 2 * x) / 3);}

            float a = 0;
            inline void pulsate(){
                float b = (sin (a)+1)/2;
                //float v = SMOOTHERSTEP(b);
                float v = inverse_smoothstep(b);
                int color = (50 * v) + (255 * (1 - v));
                fill_solid(leds, NUM_LEDS, CHSV(0, 255, color));
                a+=0.03;
                FastLED.delay(15);
            }
    public:
        rgbLight(){};
        
        inline void on(){
            if( !isOn ){
                isOn = true;
                setRGB(lastRed,lastGreen,lastBlue);
            }
        }

        inline void off(){
            if( isOn ){
                isOn = false;
                animMode = 0;
                for (int i = 0; i < NUM_LEDS; i++){
                    leds[i] = CRGB::Black;
                }
            }
        }

        inline void setRGB(byte r, byte g, byte b){
            animMode = 0;
            lastRed = r;lastGreen = g;lastBlue = b;
            if(r == 0 && g == 0 && b == 0){off();}else{
                for(int i = 0; i < NUM_LEDS;i++){
                    leds[i] = CRGB( lastRed, lastGreen, lastBlue );
                }
            }
        }

        inline void runAnimations(){
            switch(animMode){
                case 1:
                    pulsate();
                    break;
                case 2:
                    pulsateColorChange();
                break;
                default:
                    break;
            }
        }

        inline void setup(){
            FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
            animMode = 1;
        }

        inline void loop(){
            runAnimations();
            FastLED.show();
        }
};
rgbLight light;

void lightTask( void * parameter ){
    light.setup();
    for ever{
        light.loop();
        vTaskDelay(1);
    }
}

#endif