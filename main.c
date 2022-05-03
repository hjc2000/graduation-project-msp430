#include "MspTar.h"
#include "MSP430Lib/HardwareSerial.h"
#include "DS18B20.h"
#include "myLib/Timer.h"
#include "hardware.h"

/*按照依赖关系按顺序定义*/
Task task;
Timer timer;
Ticker ticker;

HardwareSerial Serial;
MspTar tar;
DS18B20 ds18b20;

int main(void)
{
    init::initSys();
    Serial.begin();
    while (1)
    {
        task.handle();
        timer.handle();
        __bis_SR_register(GIE);
    }
}
