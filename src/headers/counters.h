boolean canCountDown    = false;
long countDownMS        = 0;
long countDownStartMS   = 0;

static const inline void startCountDown(long ms){
    countDownStartMS = millis();
    countDownMS = ms;
    canCountDown = true;
}

static const inline void countDown(){
    if( canCountDown ){
        long diff = millis() - countDownStartMS;
        if( diff >= countDownMS){
            canCountDown    = false;
            countDownMS     = 0;
            light.off();
        }
    }
}

static const inline void getCounters(){

}

static const inline void countersLoop(){
    countDown();
}


void counterTask( void * parameter ){
    getCounters();
    for ever{
        countersLoop();
        vTaskDelay(1);
    }
}