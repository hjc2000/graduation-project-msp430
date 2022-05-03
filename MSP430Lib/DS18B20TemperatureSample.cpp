// #include "DS18B20TemperatureSample.h"

// DS18B20TemperatureSample::DS18B20TemperatureSample(void) : DS18B20BaseOperate(GPIO_PORT_P1, GPIO_PIN4)
// {
//     task.add([this](void) -> void
//              { StartTemperatureSample(); });
// }

// void DS18B20TemperatureSample::OnResetDone(void)
// {
//     m_f_ToDoOnResetDone();
//     GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7); //绿色LED
// }

// void DS18B20TemperatureSample::StartTemperatureSample(void)
// {
//     Reset();
//     m_f_ToDoOnResetDone = [this](void) -> void
//     {
//         SendConvertCommand();
//     };
// }

// void DS18B20TemperatureSample::SendConvertCommand(void)
// {
//     WriteByte(0xcc); //跳过ROM
//     WriteByte(0x44); //转化
//     auto fun = [this](void) -> void
//     {
//         IsConvertDone();
//     };
//     ticker.addTask(10 * 1000, fun); // 1s后检查是否转换完毕
// }

// void DS18B20TemperatureSample::IsConvertDone(void)
// {
//     if (ReadBit())
//     {
//         //转换完成
//         Reset();
//         m_f_ToDoOnResetDone = [this](void) -> void
//         {
//             SendReadCommandAndRead();
//         };
//     }
//     else
//     {
//         auto fun = [this](void) -> void
//         {
//             IsConvertDone();
//         };
//         ticker.addTask(100, fun); //没有转换完，继续检查
//     }
// }

// void DS18B20TemperatureSample::SendReadCommandAndRead(void)
// {
//     WriteByte(0xcc);
//     WriteByte(0xbe);
//     uint8_t low = ReadByte();
//     uint8_t high = ReadByte();
//     //触发“获取到温度”事件
//     uint8_t buff[] = {low, high};
//     OnGetTemperature(buff);
// }
