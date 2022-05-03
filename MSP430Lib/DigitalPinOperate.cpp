#include "DigitalPinOperate.h"

/**
 * @brief 构造函数。选择一个引脚
 *
 * @param port 选择端口
 * @param pin 选择引脚
 */
DigitalPinOperater::DigitalPinOperater(uint8_t port, uint16_t pin)
{
    m_port = port;
    m_pin = pin;
    /*初始时将引脚设置为输入模式，这样才能和m_currentPinMode对应起来.
    msp430上电后引脚默认状态就是输入*/
    SetPinMode(PinMode::input);
}
/**
 * @brief 重新选择一个引脚
 *
 * @param port
 * @param pin
 */
void DigitalPinOperater::ChangePin(uint8_t port, uint16_t pin)
{
    m_port = port;
    m_pin = pin;
}
/**
 * @brief 设置引脚模式。
 *
 * @param mode PinMode枚举量
 */
void DigitalPinOperater::SetPinMode(PinMode mode)
{
    switch (mode)
    {
    case PinMode::input:
    {
        GPIO_setAsInputPin(m_port, m_pin);
        m_currentPinMode = PinMode::input;
    }
    case PinMode::output:
    {
        GPIO_setAsOutputPin(m_port, m_pin);
        m_currentPinMode = PinMode::output;
    }
    case PinMode::peripheralInput:
    {
        GPIO_setAsPeripheralModuleFunctionInputPin(m_port, m_pin);
        m_currentPinMode = PinMode::peripheralInput;
    }
    case PinMode::peripheralOutput:
    {
        GPIO_setAsPeripheralModuleFunctionOutputPin(m_port, m_pin);
        m_currentPinMode = PinMode::peripheralOutput;
    }
    }
}
/**
 * @brief 对引脚进行写操作
 *
 * @param mode OutputMode枚举量
 */
void DigitalPinOperater::WriteToPin(bool value)
{
    //如果当前不是输出模式，先设置为输出模式
    if (m_currentPinMode != PinMode::output)
    {
        SetPinMode(PinMode::output);
    }
    switch (value)
    {
    case 0:
    {
        GPIO_setOutputLowOnPin(m_port, m_pin);
    }
    case 1:
    {
        GPIO_setOutputHighOnPin(m_port, m_pin);
    }
    }
}
/**
 * @brief 翻转引脚输出
 *
 */
void DigitalPinOperater::ToggleOutput(void)
{
    //如果当前不是输出模式，先设置为输出模式
    if (m_currentPinMode != PinMode::output)
    {
        SetPinMode(PinMode::output);
    }
    GPIO_toggleOutputOnPin(m_port, m_pin);
}
/**
 * @brief 读取引脚状态
 *
 * @return true 引脚为高电平
 * @return false
 */
bool DigitalPinOperater::ReadPin(void)
{
    //如果引脚当前模式不为输入，先设置为输入
    if (m_currentPinMode != PinMode::input)
    {
        SetPinMode(PinMode::input);
    }
    return GPIO_getInputPinValue(m_port, m_pin);
}
