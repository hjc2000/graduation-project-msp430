#include "DS18B20Lib.h"

void delay(uint16_t us) //延迟，以us为单位
{
    for (volatile uint16_t i = 0; i < us; i++)
    {
        __delay_cycles(24);
    }
}

/**
 * @brief 构造函数。选择DS18B20的数据线连接着的引脚
 *
 * @param port
 * @param pin
 */
DS18B20BaseOperate::DS18B20BaseOperate(uint8_t port, uint16_t pin) : pinOperator(port, pin)
{
}

/**
 * @brief 向引脚写一位数据
 *
 * @param value
 */
void DS18B20BaseOperate::WriteBit(bool value)
{
    if (value)
    {
        /*向总线写1，需要将总线下拉小于15us后上拉，整个过程持续60us*/
        __bic_SR_register(GIE);
        PullDown();
        delay(10);
        Release();
        __bis_SR_register(GIE);
        delay(52); //维持高电平
    }
    else
    {
        /*向总线写0，需要将总线下拉60us*/
        __bic_SR_register(GIE);
        PullDown();
        delay(60); //延迟60us以上
        Release();
        __bis_SR_register(GIE);
        delay(1); //恢复时间1us以上
    }
}

/**
 * @brief 从引脚读一位数据，执行完后引脚停留在输入模式
 *
 * @return true 引脚为高电平
 * @return false 引脚为低电平
 */
bool DS18B20BaseOperate::ReadBit(void)
{
    __bic_SR_register(GIE); //禁止中断
    PullDown();
    delay(2);
    Release();
    delay(3);
    bool bit = pinOperator.ReadPin();
    __bis_SR_register(GIE); //使能中断
    delay(60);              //恢复时间
    return bit;
}

/**
 * @brief 重置DS18B20
 *
 */
void DS18B20BaseOperate::Reset(void)
{
    static uint8_t flag = 0;

    auto fun = [this](void) -> void
    {
        Reset();
    };

    switch (flag)
    {
    case 0: //开始重置
    {
        PullDown();
        ticker.addTask(5, fun); // 500us后再次进入该函数
        flag++;
    }
    case 1:
    {
        //设置为输入，让外接上拉电阻上拉
        Release();
        delay(65); //延迟一段时间后检测“存在脉冲”
        if (!pinOperator.ReadPin())
        {
            //检测到存在脉冲
            flag++;
            ticker.addTask(2, fun); //等待存在脉冲结束
        }
        else
        {
            //检测不到存在脉冲
            flag = 0; //复位
            return;
        }
    }
    case 2:
    {
        flag = 0;
        OnResetDone();
    }
    }
}

void DS18B20BaseOperate::WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        WriteBit(data & uint8_t(0x01));
        data = data >> 1;
    }
}

uint8_t DS18B20BaseOperate::ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t bit = ReadBit();
        bit = bit << i; //移位
        data = data | bit;
    }
    return data;
}
