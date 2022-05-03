#ifndef DS18B20Lib_h
#define DS18B20Lib_h
#include <stdint.h>
#include "DigitalPinOperate.h"
#include "Timer.h"

class DS18B20BaseOperate
{
protected:
    DS18B20BaseOperate(uint8_t port, uint16_t pin);
    void delay(uint16_t us);
    void Reset(void);
    void WriteByte(uint8_t data);
    uint8_t ReadByte(void);
    void WriteBit(bool value);
    bool ReadBit(void);
    /**
     * @brief 重置完成时被回调
     *
     */
    virtual void OnResetDone(void) = 0;

private:
    //下拉总线
    void PullDown(void)
    {
        pinOperator.WriteToPin(0);
    }
    //释放总线
    void Release(void)
    {
        pinOperator.SetPinMode(DigitalPinOperater::PinMode::input);
    }

private:
    DigitalPinOperater pinOperator;
};

#endif // DS18B20Lib.h
