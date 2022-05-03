#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <list>
#include "driverlib.h"
#include "CircularQueue.h"
#include "Task.h"

using namespace std;

class HardwareSerial
{
private:
    CircularQueue<uint8_t> m_readBuff, m_txBuff; //使用循环队列实现的接收、发送缓冲区

public: //环境配置
    HardwareSerial(void);
    void readDataInRxISR(void); //放在接收中断服务程序
    void sendDataInTxISR(void); //放到发送中断服务程序

private: //重新启动发送
    /* 串口的发送中断是上升沿触发，硬件的发送缓冲寄存器 TXBUFF 为空后会触发中断，
    进入中断后如果没有往 TXBUFF 写数据，退出中断后，便无法再次进入中断，除非
    再次向 TXBUFF 写入数据 */
    bool m_btxDone = true; //这个标志位为真表示发生过进入中断但没有往TXBUFF写数据
    void startSend(void);

public:                              //提供给用户的方法
    void begin(void);                //初始化UART模块
    void write(uint8_t data);        //向发送队列中写一个数据
    uint8_t read(void);              //从接收队列中读取一个数，读取后数据会从队列中删除
    uint8_t availableForWrite(void); //发送队列剩余空间
    uint8_t available(void);         //接收队列长度
    void flush(void);                //等待直到发送缓冲区清空
};
extern HardwareSerial Serial;

#endif // SERIAL_H
