#ifndef DS18B20Lib_h
#define DS18B20Lib_h
#include <stdint.h>
#include "DigitalPinOperate.h"
#include "Timer.h"

class DS18B20BaseOperate
{
protected:
    DS18B20BaseOperate(uint8_t port, uint16_t pin);
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
        pinOperator.WriteToPin(pinOperator.low);
    }
    //释放总线
    void Release(void)
    {
        pinOperator.SetPinMode(pinOperator.input);
    }

private:
    DigitalPinOperate pinOperator;
};

#endif // DS18B20Lib.h