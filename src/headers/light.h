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
            int fadeAmount = 10;
            int pulsateColorBrightness = 255;
            inline void pulsateColorChange(){
                EVERY_N_MILLIS(1000){
                    CRGB randomcolor  = CHSV(random(192), 255, 255);
                    for(int i = 0; i < NUM_LEDS; i++ ){
                        leds[i] = randomcolor;
                        leds[i].fadeLightBy(pulsateColorBrightness);
                    }
                    FastLED.show();
                    pulsateColorBrightness = pulsateColorBrightness + fadeAmount;
                    if(pulsateColorBrightness == 0 || pulsateColorBrightness == 255){
                        fadeAmount = -fadeAmount ; 
                    }
                }
            }

            float inverse_smoothstep(float x) {
                return 0.5 - sin(asin(1 - 2 * x) / 3);}

            float a = 0;
            inline void breathe(){
                float b = (sin (a)+1)/2;
                //float v = SMOOTHERSTEP(b);
                float v = inverse_smoothstep(b);
                int color = (50 * v) + (255 * (1 - v));
                fill_solid(leds, NUM_LEDS, CHSV(0, 255, color));
                a+=0.03;
                FastLED.delay(15);
            }

            /* PULSATE ANIMATION */
            uint8_t bloodHue = 96;  // Blood color [hue from 0-255]
            uint8_t bloodSat = 255;  // Blood staturation [0-255]
            int flowDirection = -1;   // Use either 1 or -1 to set flow direction
            uint16_t cycleLength = 1500;  // Lover values = continuous flow, higher values = distinct pulses.
            uint16_t pulseLength = 150;  // How long the pulse takes to fade out.  Higher value is longer.
            uint16_t pulseOffset = 200;  // Delay before second pulse.  Higher value is more delay.
            uint8_t baseBrightness = 10;  // Brightness of LEDs when not pulsing. Set to 0 for off.

            int sumPulse(int time_shift) {
                //time_shift = 0;  //Uncomment to heart beat/pulse all LEDs together
                int pulse1 = pulseWave8( millis() + time_shift, cycleLength, pulseLength );
                int pulse2 = pulseWave8( millis() + time_shift + pulseOffset, cycleLength, pulseLength );
                return qadd8( pulse1, pulse2 );  // Add pulses together without overflow
            }

            uint8_t pulseWave8(uint32_t ms, uint16_t cycleLength, uint16_t pulseLength) {
                uint16_t T = ms % cycleLength;
                if ( T > pulseLength) return baseBrightness;
                uint16_t halfPulse = pulseLength / 2;
                if (T <= halfPulse ) {
                    return (T * 255) / halfPulse;  //first half = going up
                } else {
                    return((pulseLength - T) * 255) / halfPulse;  //second half = going down
                }
            }

            inline void pulsate(){
                for (int i = 0; i < NUM_LEDS ; i++) {
                    uint8_t bloodVal = sumPulse( (5/NUM_LEDS/2) + (NUM_LEDS/2) * i * flowDirection );
                    leds[i] = CHSV( bloodHue, bloodSat, bloodVal );
                }
            }

            /* PULSATE ANIMATION END */

            inline void randomColors(){
                EVERY_N_SECONDS(5){
                    CRGB randomcolor  = CHSV(random(192), 255, 255);
                    for (int i = 0; i < NUM_LEDS ; i++) {
                        leds[i] = randomcolor;
                    }
                }
            }

            inline void rainbow(){
                int rainBowSpeed = 10;
                int rainBowHue = 10;
                uint8_t thisHue = beat8(rainBowSpeed,255);
                fill_rainbow(leds, NUM_LEDS, thisHue, rainBowHue);
            }

            #define COOLING  20
            // Default 120, suggested range 50-200.
            #define SPARKING 50
            void Fire2012(){
                static byte heat[NUM_LEDS];
                for( int i = 0; i < NUM_LEDS; i++) {
                    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
                }
                for( int k= NUM_LEDS - 1; k >= 2; k--) {
                    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
                }
                if( random8() < SPARKING ) {
                    int y = random8(7);
                    heat[y] = qadd8( heat[y], random8(160,255) );
                }
                for( int j = 0; j < NUM_LEDS; j++) {
                    leds[j] = HeatColor( heat[j]);
                }
            }
            inline void fire(){
                Fire2012();
                FastLED.delay(1000 / 60);
            }

            /* water */
            void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
                setAll(red,green,blue);
                
                int Pixel = random(NUM_LEDS);
                setPixel(Pixel,0xff,0xff,0xff);
                FastLED.show();
                delay(SparkleDelay);
                setPixel(Pixel,red,green,blue);
                FastLED.show();
                delay(SpeedDelay);
            }

            void setPixel(int Pixel, byte red, byte green, byte blue) {
                leds[Pixel].r = red;
                leds[Pixel].g = green;
                leds[Pixel].b = blue;
            }

            void setAll(byte red, byte green, byte blue) {
                for(int i = 0; i < NUM_LEDS; i++ ) {
                    setPixel(i, red, green, blue); 
                }
                FastLED.show();
            }

            inline void water(){
                SnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
            }
            /* water */

            inline void nature(){}

            boolean cycle = false;
            inline void allEffectRotate(){
                EVERY_N_SECONDS(20){
                    animMode++;
                    if(animMode > 8){animMode = 1;}
                }
            }

            boolean loadingIsOn = false;
            void fadeall() {
                for (int i = 0; i < NUM_LEDS; i++) {
                    leds[i].nscale8(250);
                }
            }

            int loading_delay = 30;
            int loading_dir = 5;
            inline void loadingAnim(){
                if(loadingIsOn){
                    for (int i = 0; i < NUM_LEDS; i++) {
                        leds[i] = CHSV(0, 255, 255);
                        FastLED.show();
                        for(int j = 0; j < random(2,20); j++){
                            fadeall();
                        }
                        delay(loading_delay);
                        if (loading_dir == 1) {
                            loading_delay++;
                        }else if (loading_dir == 2) {
                            loading_delay--;
                        }
                        if (loading_delay == 40) {
                            loading_dir = 2;
                        }else if (loading_delay == 10) {
                            loading_dir = 1;
                        }
                    }
                }
            }

    public:
        rgbLight(){};

        boolean delayOffBool = false;
        long delayOffStartMS = 0;
        inline void delayOff(){
            delayOffBool = true;
            delayOffStartMS = millis();
        }

        inline void checkDelayOff(){
            if(delayOffBool){
                if(millis() - delayOffStartMS >= 5000){
                    delayOffBool = false;
                    off();
                }
            }
        }

        inline void success(){
            animMode = 0;
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            delayOff();
        }
        inline void warning(){
            animMode = 0;
            fill_solid(leds, NUM_LEDS, CRGB::Yellow);
            delayOff();
        }
        inline void error(){
            animMode = 0;
            fill_solid(leds, NUM_LEDS, CRGB::Red);
            delayOff();
        }

        inline void setLoading(boolean isOn){
            animMode = 0;
            for (int i = 0; i < NUM_LEDS; i++){leds[i] = CRGB::Black;}
            loadingIsOn = isOn;
        }

        inline void setAnimation(byte mode){
            FastLED.setBrightness(255);
            loadingIsOn = false;
            animMode = mode;
            if(mode == 9){cycle = true;animMode = 1;}else{cycle = false;}
        }

        inline void on(){
            isOn = true;
            setRGB(lastRed,lastGreen,lastBlue);
        }

        inline void off(){
            isOn = false;
            animMode = 0;
            for (int i = 0; i < NUM_LEDS; i++){
                leds[i] = CRGB::Black;
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
                    breathe();
                    break;
                case 2:
                    pulsateColorChange();
                    break;
                case 3:
                    pulsate();
                    break;
                case 4:
                    randomColors();
                    break;
                case 5:
                    rainbow();
                    break;
                case 6:
                    fire();
                    break;
                case 7:
                    water();
                    break;
                case 8:
                    nature();
                    break;
                default:
                    break;
            }
        }

        inline void setup(){
            FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
            off();
        }

        inline void loop(){
            if(cycle){allEffectRotate();}
            runAnimations();
            loadingAnim();
            checkDelayOff();
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