#include "MspTar.h"

MspTar::MspTar(void)
{
    Init();
}

void MspTar::Init(void)
{
    auto fun = [this]() -> void
    {
        Handle();
    };
    pTask->add(fun);
}

void MspTar::Handle(void)
{
    /*分析m_readList中的数据*/
    while (Serial.available())
    {
        AnalysisReadList(Serial.read());
    }
}

int MspTar::availableForWrite(void)
{
    return Serial.availableForWrite();
}

void MspTar::flush(void)
{
    Serial.flush();
}

void MspTar::write(uint8_t data)
{
    Serial.write(data);
}

/**
 * @brief 收到完整的一个帧后被调用
 *
 * @param data
 */
void MspTar::OnReceive(CircularQueue<uint8_t> &data)
{
    switch (data.pop_front())
    {
    case 1:
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        break;
    }
    }
}
