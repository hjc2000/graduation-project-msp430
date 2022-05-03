#include "HardwareSerial.h"

HardwareSerial::HardwareSerial(void)
{
    auto fun = [this]() -> void
    {
        startSend();
    };
    pTask->add(fun);
}

void HardwareSerial::begin(void)
{
    /* UART_A0 TXD */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    /* UATR_A0 RXD */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN4);
    /* UATR初始化结构体 */
    USCI_A_UART_initParam param =
        {
            .selectClockSource = USCI_A_UART_CLOCKSOURCE_ACLK,
            .clockPrescalar = 3,
            .firstModReg = 0,
            .secondModReg = 3,
            .parity = USCI_A_UART_NO_PARITY,
            .msborLsbFirst = USCI_A_UART_LSB_FIRST,
            .numberofStopBits = USCI_A_UART_ONE_STOP_BIT,
            .uartMode = USCI_A_UART_MODE,
            .overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION

        };
    /* 初始化UART */
    USCI_A_UART_init(USCI_A0_BASE, &param);
    /* 使能UART模块 */
    USCI_A_UART_enable(USCI_A0_BASE);
    /*清除RX中断标志位*/
    USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    /*使能RX中断*/
    USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    /* 使能TX中断 */
    USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_TRANSMIT_INTERRUPT);
}

void HardwareSerial::startSend(void)
{
    if (m_btxDone && m_txBuff.size())
    {
        m_btxDone = false;
        uint8_t data = m_txBuff.pop_front();
        USCI_A_UART_transmitData(USCI_A0_BASE, data);
    }
}

void HardwareSerial::readDataInRxISR(void)
{
    m_readBuff.push_back(USCI_A_UART_receiveData(USCI_A0_BASE));
}

void HardwareSerial::sendDataInTxISR(void)
{
    if (m_txBuff.size())
    {
        uint8_t data = m_txBuff.pop_front();
        USCI_A_UART_transmitData(USCI_A0_BASE, data);
    }
    else
    {
        m_btxDone = true;
    }
}

void HardwareSerial::write(uint8_t data)
{
    m_txBuff.push_back(data);
}

uint8_t HardwareSerial::read(void)
{
    if (m_readBuff.size())
    {
        uint8_t data = m_readBuff.pop_front();
        return data;
    }
    else
    {
        return 0;
    }
}

uint8_t HardwareSerial::availableForWrite(void)
{
    return m_txBuff.available();
}

uint8_t HardwareSerial::available(void)
{
    return m_readBuff.size();
}

void HardwareSerial::flush(void)
{
    while (!m_txBuff.empty())
    {
        startSend();
    }
}
