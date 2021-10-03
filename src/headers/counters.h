

static const inline void getCounters(){

}

static const inline void countersLoop(){

}


void counterTask( void * parameter ){
    getCounters();
    for ever{
        countersLoop();
        vTaskDelay(1);
    }
}