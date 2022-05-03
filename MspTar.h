#ifndef MSPTAR_H
#define MSPTAR_H
#include "UartTar.h"
#include "HardwareSerial.h"

class MspTar : public UartTar
{
public:
    MspTar(void);

protected:
    void Init(void) override;
    int availableForWrite(void) override;
    void flush(void) override;
    void write(uint8_t data) override;
    void OnReceive(CircularQueue<uint8_t> &data) override;

public:
    void Handle(void);
};
extern MspTar tar;

#endif // MSPTAR_H
