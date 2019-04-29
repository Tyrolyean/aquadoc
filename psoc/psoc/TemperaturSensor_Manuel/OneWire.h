#include <project.h>

#define ONE_WIRE_PRESENT 0x00
#define ONE_WIRE_NOT_PRESENT 0x01

int OW_Detect(unsigned int pin);
void OW_WriteByte(unsigned char u8);
unsigned char OW_ReadByte();
unsigned char OW_CRC(unsigned char *pBuf, int len);
