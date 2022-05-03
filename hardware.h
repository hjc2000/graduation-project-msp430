/*文件说明：
这个文件中放与硬件有关的函数或类，即MSP和ESP不通用的
*/

#ifndef HARDWARE_H
#define HARDWARE_H

#include "driverlib.h"
#include <stdint.h>

/* 进行系统的初始化 */
namespace init
{
    void initSys(void);
    extern const uint16_t freDCO;
    extern const uint16_t ratio;
}

#endif // HARDWARE_H
