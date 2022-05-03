#include "hardware.h"
#include "HardwareSerial.h"
#include "Timer.h"

#pragma region init 初始化
/******************************************************/
/*************************初始化************************/
const uint16_t init::freDCO = 28000; // DCO的频率
const uint16_t init::ratio = freDCO / 4000;

void init::initSys(void)
{
    WDT_A_hold(WDT_A_BASE); //关闭看门狗

    //初始化时钟系统
    [](const uint16_t freDCO, const uint16_t ratio) -> void
    {
        /*提高二次侧电压*/
        PMM_setVCore(3);
        /* XOUT */
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN5);
        /* XIN */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4);
        /* X2OUT */
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3);
        /* X2IN */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2);
        /* 打开XT2 */
        UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
        /* 打开XT1 */
        UCS_turnOnLFXT1(UCS_XT1_DRIVE_3, UCS_XCAP_3);
        /* 设置FLL的参考时钟信号 */
        UCS_initClockSignal(UCS_FLLREF, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
        /*初始化DCO*/
        UCS_initFLL(freDCO, ratio);
        /* 设置SMCLK的时钟源为XT1 */
        UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
        UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    }(freDCO, ratio);

    //初始化GPIO
    [](void) -> void
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);
    }();

    //初始化TA0
    [](const uint16_t prTA0) -> void
    {
        Timer_A_initUpModeParam paramUp =
            {
                .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
                .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_4, // 4分频
                .timerPeriod = prTA0,
                .timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,
                .captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
                .timerClear = TIMER_A_DO_CLEAR,
                .startTimer = true,
            };
        Timer_A_initUpMode(TIMER_A0_BASE, &paramUp);
    }(100); // 100us

    __bis_SR_register(GIE); //使能全局中断
}
/*************************初始化************************/
/******************************************************/
#pragma endregion init 初始化

#pragma region 中断服务程序
/* 定时器中断 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt
#endif
    void
    TIMER0_A0_ISR(void)
{
    timer.m_bTimeUp = true;
}
/**************************************************************/
/* 串口中断 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A0_VECTOR
__interrupt
#endif
    void
    USCI_A0_ISR(void)
{
    switch (UCA0IV)
    {
    case USCI_UCTXIFG:
    {
        Serial.sendDataInTxISR();
        break;
    }
    case USCI_UCRXIFG:
    {
        Serial.readDataInRxISR();
        break;
    }
    default:
        break;
    }
}
/**************************************************************/
/* P2.3中断 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR
__interrupt
#endif
    void
    port2Pin3ISR(void)
{
    switch (P2IV)
    {
    case P2IV_P2IFG3:
    {
    }
    }
}
#pragma endregion 中断服务程序
