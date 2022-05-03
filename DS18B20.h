#ifndef DS18B20_H
#define DS18B20_H

// #include "MspTar.h"
// #include "DS18B20TemperatureSample.h"

// class DS18B20 : public virtual DS18B20TemperatureSample
// {
// public:
//     DS18B20(void) : DS18B20TemperatureSample(GPIO_PORT_P1, GPIO_PIN4)
//     {
//         auto fun = [this](void) -> void
//         {
//             StartTemperatureSample();
//         };
//         timer.addTask(50000, fun); //添加到定时器
//     }

//     void OnGetTemperature(uint8_t *buff) override
//     {
//         uint8_t sendBuff[3] = {1, buff[0], buff[1]};
//         tar.sendData(sendBuff, 3);
//     }
// };

#include <stdint.h>
#include "driverlib.h"
#include "Timer.h"
#include "MspTar.h"
#include "DigitalPinOperate.h"
#include "Delegate.h"

class BasicGpioOperation
{
private:
    static const uint8_t m_port = GPIO_PORT_P1;
    static const uint16_t m_pin = GPIO_PIN4;

protected:
    static inline bool readPin(void)
    {
        return GPIO_getInputPinValue(m_port, m_pin);
    }
    static inline void setAsOutput(void)
    {
        GPIO_setAsOutputPin(m_port, m_pin);
    }
    static inline void setAsInput(void)
    {
        GPIO_setAsInputPin(m_port, m_pin);
    }
    static inline void outputHigh(void)
    {
        GPIO_setOutputHighOnPin(m_port, m_pin);
    }
    static inline void outputLow(void)
    {
        GPIO_setOutputLowOnPin(m_port, m_pin);
    }
    static inline uint8_t test(void)
    {
        //检测当前总线的电平，用来检测存在脉冲
        setAsInput();
        return GPIO_getInputPinValue(m_port, m_pin);
    }

protected:
    static inline void delay(uint16_t us) //延迟，以us为单位
    {
        for (volatile uint16_t i = 0; i < us; i++)
        {
            __delay_cycles(24);
        }
    }
};

class ResetDS18B20 : public virtual BasicGpioOperation
{
protected:
    //重置完成事件
    function<void(void)> EndResetEvent = nullptr;
    void startReset(void)
    {
        setAsOutput();
        outputLow();
        auto fun = [this](void) -> void
        {
            pullUp();
        };
        ticker.addTask(5, fun); // 500us后上拉
    }
    void pullUp(void)
    {
        outputHigh();
        delay(65);   //延迟一段时间后检测“存在脉冲”
        if (!test()) //检测存在脉冲
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
            auto fun = [this](void) -> void
            {
                endReset();
            };
            ticker.addTask(2, fun); //等待存在脉冲结束
        }
    }
    //执行重置结束事件委托
    void endReset(void)
    {
        if (EndResetEvent != nullptr)
        {
            EndResetEvent();
        }
    }
};

class DataExchange : public virtual BasicGpioOperation
{
protected:
    static void write0(void)
    {
        /*向总线写0，需要将总线下拉60us*/
        __bic_SR_register(GIE);
        setAsOutput();
        outputLow();
        delay(60); //延迟60us以上
        outputHigh();
        __bis_SR_register(GIE);
        delay(1); //恢复时间1us以上
    }
    static void write1(void) //最后总线处于强上拉状态
    {
        /*向总线写1，需要将总线下拉小于15us后上拉，整个过程持续60us*/
        __bic_SR_register(GIE);
        setAsOutput();
        outputLow();
        delay(10);
        outputHigh();
        __bis_SR_register(GIE);
        delay(52); //维持高电平
    }
    static void write(uint8_t data)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            if (data & uint8_t(0x01)) //检测最低位是0还是1
            {
                write1();
            }
            else
            {
                write0();
            }
            data = data >> 1;
        }
    }
    static bool readABit(void)
    {
        __bic_SR_register(GIE); //禁止中断
        setAsOutput();
        outputLow();
        delay(1);
        setAsInput();
        delay(2);
        bool bit = readPin();
        __bis_SR_register(GIE); //使能中断
        delay(60);              //恢复时间
        return bit;
    }
    static uint8_t read(void)
    {
        uint8_t data = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            uint8_t bit = readABit();
            bit = bit << i; //移位
            data = data | bit;
        }
        return data;
    }
};

/****************************************************/
/*以下是实现具体功能的类*/
class DS18B20TemperatureSample : public virtual ResetDS18B20, public virtual DataExchange
{
public:
    Delegate getTempEvent;

protected:
    void startTemperatureSample(void)
    {
        startReset();
        //订阅重置完成事件
        EndResetEvent = [this](void) -> void
        {
            sendConvertCommand();
        };
    }
    void sendConvertCommand(void)
    {
        write(0xcc); //跳过ROM
        write(0x44); //转化
        function<void(void)> fun = [this](void) -> void
        {
            isConvertDone();
        };
        ticker.addTask(10 * 1000, fun); // 1s后检查是否转换完毕
    }
    void isConvertDone(void)
    {
        if (readABit())
        {
            //转换完成
            startReset();
            EndResetEvent = [this](void) -> void
            {
                sendReadCommandAndRead();
            };
        }
        else
        {
            function<void(void)> fun = [this](void) -> void
            {
                isConvertDone();
            };
            ticker.addTask(100, fun); //没有转换完，继续检查
        }
    }
    void sendReadCommandAndRead(void)
    {
        write(0xcc);
        write(0xbe);
        uint8_t low = read();
        uint8_t high = read();
        //触发“获取到温度”事件
        uint8_t buff[] = {low, high};
        // getTempEvent(buff);
        uint8_t sendBuff[3] = {1, buff[0], buff[1]};
        tar.sendData(sendBuff, 3);
    }
};

class DS18B20 : public virtual DS18B20TemperatureSample
{
public:
    DS18B20(void)
    {
        auto fun = [this](void) -> void
        {
            startTemperatureSample();
        };
        timer.addTask(50000, fun); //添加到定时器
    }
};
extern DS18B20 ds18b20; //引用声明

#endif // DS18B20_H
