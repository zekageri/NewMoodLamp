#ifndef ESP_Time_h
#define ESP_Time_h

    class ESP_Time{
        long lastSettedTime = 0;
        long lastNTP_Try    = 0;
        boolean gotTimeFrom_NTP = false;
        boolean gotTimeFrom_RTC = false;
        boolean gmt_Offset_Set  = true;
        boolean timeIsOk        = false;
        // FOR NTP SYNC 
        const char* ntpServer     = "pool.ntp.org";
        //const char* ntpServers[3] = {"pool.ntp.org","0.hu.pool.ntp.org","1.hu.pool.ntp.org"};
        int gmtOffset_sec         = 3600;
        int daylightOffset_sec    = 3600;
        // FOR NTP SYNC
        String CurrentDay = "mon";
        int Year    = 2020;
        int Up_Day  = 0,Up_Hour = 0,Up_Minute   = 0,Up_Second   = 0,HighMillis  = 0,Rollover        = 0,
            Month   = 0,Day     = 0,Hour        = 0,Min         = 0,Sec         = 0,currentDayInt   = 0;
        char timestring[25];
        
        int months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        char daysOfTheWeek[7][13] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

        private:

            void setInternalTime(int year,int mon,int mDay,int hour,int min,int sec,int wDay){
                Year            = year;         Month   = mon;
                Day             = mDay;         Hour    = hour;
                Min             = min;          Sec     = sec;
                CurrentDay      = daysOfTheWeek[wDay];
                currentDayInt   = wDay;
                timeIsOk        = true;
            }

            void rtcBegin(){
                if ( !rtc.begin() ) {
                    Serial.println("Couldn't find RTC");
                    gotTimeFrom_RTC = false;
                }else{
                    if ( rtc.lostPower() ) {
                        errorLog("RTC Lost Power, check the battery!");
                        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
                        gotTimeFrom_RTC = false;
                    }else{
                        gotTimeFrom_RTC = true;
                        Serial.println("Got rtc time");
                        DateTime now = rtc.now();
                        setInternalTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second(),now.dayOfTheWeek());
                        generateAdminCode();
                    }
                }
            }
            
            byte maxNTP_Try_Count = 0;
            void ntpBegin(){
                if( gmt_Offset_Set && wifiConnected && !gotTimeFrom_NTP && millis() - lastNTP_Try >= 20000 && maxNTP_Try_Count < 3 ){
                    maxNTP_Try_Count++;
                    lastNTP_Try = millis();
                    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
                    struct tm timeinfo;
                    if( !getLocalTime(&timeinfo) ){
                        if( maxNTP_Try_Count < 2 ){
                            errorLog("No ntp time. Getting time from RTC.");
                        }
                        Serial.println("Ntp time config was not successfull");
                        gotTimeFrom_NTP = false;
                    }else{
                        Serial.println("Got ntp time");
                        gotTimeFrom_NTP = true;
                        setInternalTime(timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,timeinfo.tm_wday);
                        rtc.adjust(DateTime(timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec));
                        generateAdminCode();
                        canCalculateSunTimes = true;
                    }
                }else if( maxNTP_Try_Count >= 3 && millis() - lastNTP_Try >= 60000 ){
                    maxNTP_Try_Count = 2;
                }
            }

            void generateAdminCode(){
                ADMIN_CODE = 951008;
                String adminCode = String(ADMIN_CODE) + String(Day);
                ADMIN_CODE = adminCode.toInt();
                Serial.print("Generated admin Code: ");
                Serial.println(ADMIN_CODE);
            }

            String Assemble_Time(){
                String ReadableMonth = "0",ReadableDay = "0",ReadableHour = "0",ReadableMin = "0",ReadableSec = "0";
                if(Hour < 10)   {ReadableHour   += Hour;}   else{ReadableHour   = Hour;}
                if(Min < 10)    {ReadableMin    += Min;}    else{ReadableMin    = Min;}
                if(Sec < 10)    {ReadableSec    += Sec;}    else{ReadableSec    = Sec;}
                if(Month < 10)  {ReadableMonth  += Month;}  else{ReadableMonth  = Month;}
                if(Day < 10)    {ReadableDay    += Day;}    else{ReadableDay    = Day;}
                return String(Year) + " " + ReadableMonth + "/"+ ReadableDay + " " + ReadableHour += ":" + ReadableMin += ":" + ReadableSec;
            }

            void upTime_Counter(){
                if (millis() >= 3000000000) {
                    HighMillis = 1;
                }
                if (millis() <= 100000 && HighMillis == 1) {
                    Rollover++;
                    HighMillis = 0;
                }
                long secsUp = millis() / 1000;
                Up_Second = secsUp % 60;
                Up_Minute = (secsUp / 60) % 60;
                Up_Hour = (secsUp / (60 * 60)) % 24;
                Up_Day = (Rollover * 50) + (secsUp / (60 * 60 * 24));
                sprintf(timestring, "%d Nap %02d:%02d:%02d", Up_Day, Up_Hour, Up_Minute, Up_Second);
            }
            
            void fetchTime(){
                EVERY_N_SECONDS(1){
                    upTime_Counter();
                    if( gotTimeFrom_NTP ){
                        struct tm timeinfo;
                        if( getLocalTime(&timeinfo) ){
                            setInternalTime(timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,timeinfo.tm_wday);
                        }else{
                            gotTimeFrom_NTP = false;
                        }
                    }else if( gotTimeFrom_RTC ){
                        DateTime now = rtc.now();
                        setInternalTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second(),now.dayOfTheWeek());
                    }else{
                        internalTick();
                    }
                }
            }

            void internalTick(){
                Sec++;
                if(Sec > 59){
                    Sec = 0;Min++;
                    if(Min > 59){
                        Min = 0;Hour++;
                        if(Hour > 23){
                            Hour = 0;Day++;
                            if( Day > (months[Month]-1) ){
                                Day = 0;Month++;
                                if(Month > 11){
                                    Month = 0;Day = 0;Hour = 0;Min = 0;Sec = 0;Year++;
                                }
                            }
                        }
                    }
                }
            }

        public:
            ESP_Time(){};

            uint8_t     Get_Hour()      {return Hour;}
            uint8_t     Get_Min()       {return Min;}
            uint8_t     Get_Sec()       {return Sec;}
            uint8_t     Get_Month()     {return Month;}
            uint16_t    Get_Year()      {return Year;}
            String      Get_Day()       {return CurrentDay;}
            uint8_t     Get_DayOfMonth(){return Day;}
            uint8_t     Get_DayOfWeek() {return currentDayInt;}
            String      GetCurrentTime(){return Assemble_Time();}
            String      GetUptime()     {return timestring;}
            uint32_t    getTimeStamp()  {DateTime now = rtc.now(); return now.unixtime();}
            boolean     isOk()          {if( Year < 2021 ){ return false; }return timeIsOk;}
            boolean     gotNTP()        {return gotTimeFrom_NTP;}
            boolean     isWeekend()     {if( currentDayInt == 6 || currentDayInt == 0 ){return true;}else{return false;}}
            boolean     isWorkDay()     {if( currentDayInt != 6 && currentDayInt != 0 ){return true;}else{return false;}}
            boolean     justRTC_Time()  {if( gotTimeFrom_RTC && !gotTimeFrom_NTP ){return true;}return false;}

            void setGmtOffset_sec(int gmtOffset){
                gmtOffset_sec       = gmtOffset;
                gmt_Offset_Set      = true;
                maxNTP_Try_Count    = 0;
                Serial.print("Setting GMT offset sec to: ");
                Serial.println(gmtOffset_sec);
            }
            void Set_Time(int CurrYear, int CurrMonth, int CurrDay, int CurrHour, int CurrMin, int CurrSec){
                if(millis() - lastSettedTime >= 60000){
                    lastSettedTime = millis();
                    Year = CurrYear;Month = CurrMonth;Day = CurrDay;Hour = CurrHour;Min = CurrMin;Sec = CurrSec;
                    rtc.adjust(DateTime(Year, Month, Day, Hour, Min, Sec+3));
                }
            }
            void setup(){
                rtcBegin();
            }

            void loop(){
                ntpBegin();
                fetchTime();
            }
    };
    ESP_Time Time;

    void Time_System( void * parameter ){
        Time.setup();
        for ever{
            Time.loop();
            vTaskDelay(100);
        }
    }

#endif