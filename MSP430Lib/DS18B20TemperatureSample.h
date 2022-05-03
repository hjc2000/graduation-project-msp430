#ifndef DS18B20TemperatureSample_h
#define DS18B20TemperatureSample_h

#include "DS18B20Lib.h"
#include <functional>

class DS18B20TemperatureSample : public DS18B20BaseOperate
{
protected:
    DS18B20TemperatureSample(uint8_t port, uint16_t pin);
    void StartTemperatureSample(void);
    virtual void OnGetTemperature(uint8_t *buff) = 0;

private:
    void OnResetDone(void) override;

    function<void(void)> m_f_ToDoOnResetDone;

    void SendConvertCommand(void);

    void IsConvertDone(void);

    void SendReadCommandAndRead(void);
};

#endif; // DS18B20TemperatureSample.h