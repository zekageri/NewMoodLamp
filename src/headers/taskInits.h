#ifndef taskInits_h
#define taskInits_h

static const inline void espTimeTaskInit(int Stack_Depth, int Core, int Priority) {
    xTaskCreatePinnedToCore(Time_System, "Time_System", Stack_Depth, NULL, Priority, &Time_System_Task_Handle, Core);
}

static const inline void lightTaskInit(int Stack_Depth, int Core, int Priority) {
    xTaskCreatePinnedToCore(lightTask, "lightTask", Stack_Depth, NULL, Priority, &lightTask_Handle, Core);
}

static const inline void counterTaskInit(int Stack_Depth, int Core, int Priority) {
    xTaskCreatePinnedToCore(counterTask, "counterTask", Stack_Depth, NULL, Priority, &counterTask_Handle, Core);
}

static const inline void initTasks(){
    espTimeTaskInit(3048, 1, 1);
    lightTaskInit(5000, 1, 1);
    counterTaskInit(3000,1,1);
}

#endif
