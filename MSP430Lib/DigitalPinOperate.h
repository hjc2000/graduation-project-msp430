#ifndef DigitalPinOperate_h
#define DigitalPinOperate_h

#include <stdint.h>
#include "driverlib.h"
using namespace std;

/**
 * @brief 在构造的时候选择一个端口的一个引脚，然后就可以调用方法对这个引脚进行
 * 操作了。
 *
 */
class DigitalPinOperater
{
public:
    DigitalPinOperater(uint8_t port, uint16_t pin);
    void ChangePin(uint8_t port, uint16_t pin);
    enum class PinMode
    {
        input,            //输入模式
        output,           //输出模式
        peripheralOutput, //外设输出
        peripheralInput,  //外设输入
    };
    void SetPinMode(PinMode mode);
    PinMode m_currentPinMode = PinMode::input; //当前的引脚模式
    void WriteToPin(bool value);
    void ToggleOutput(void);
    bool ReadPin(void);

private:
    uint8_t m_port;
    uint16_t m_pin;
};

#endif // DigitalPinOperate.h
