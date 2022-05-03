// #ifndef DS18B20TemperatureSample_h
// #define DS18B20TemperatureSample_h

// #include "DS18B20Lib.h"
// #include <functional>
// #include "Timer.h"

// class DS18B20TemperatureSample : public DS18B20BaseOperate
// {
// public:
//     DS18B20TemperatureSample(void);
//     void StartTemperatureSample(void);
//     virtual void OnGetTemperature(uint8_t *buff);

// private:
//     void OnResetDone(void) override;

//     function<void(void)> m_f_ToDoOnResetDone;

//     void SendConvertCommand(void);

//     void IsConvertDone(void);

//     void SendReadCommandAndRead(void);
// };

// #endif; // DS18B20TemperatureSample.h
