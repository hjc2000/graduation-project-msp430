#include "MspTar.h"
#include "HardwareSerial.h"
#include "Timer.h"
#include "hardware.h"
#include "DS18B20TemperatureSample.h"
#include "Task.h"

int main(void)
{
    // new Task();
    // new Timer();
    // new Ticker();
    // new HardwareSerial();
    // new MspTar();
    initSys();
    Serial.begin();
    while (1)
    {
        pTask->handle();
        timer.handle();
        __bis_SR_register(GIE);
    }
}
