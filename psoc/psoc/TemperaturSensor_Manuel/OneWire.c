// made by Manuel Eiter

#include "OneWire.h"
#include "cypins.h"
#include <stdio.h>


// the pin used to communicate with iButton.
static unsigned int owpin;

int OW_Detect(unsigned int pin)
{
	owpin=pin;
	
	CyPins_ClearPin(owpin);
	CyDelayUs(480);
	
	CyPins_SetPin(owpin);
	CyDelayUs(150);             // laengere Zeitspanne
	
	return CyPins_ReadPin(owpin);
}

void OW_WriteByte(unsigned char u8)
{
	int i=0;
	for(i=0; i<8; i++)
	{
		if(u8 & 0x01)
		{
			CyPins_ClearPin(owpin);
			CyDelayUs(6);
			CyPins_SetPin(owpin);
			CyDelayUs(64);
		}
		else
		{
			CyPins_ClearPin(owpin);
			CyDelayUs(60);
			CyPins_SetPin(owpin);
			CyDelayUs(10);			
		}
		
		u8>>=1;
	}
}

unsigned char OW_ReadByte()
{
	unsigned char b=0;
	int i=0;
	
	while(i<8)
	{
		b >>= 1;
		CyPins_ClearPin(owpin);
		CyDelayUs(6);
		CyPins_SetPin(owpin);
		CyDelayUs(9);
		
		b |= (CyPins_ReadPin(owpin)>0) ? 0x80 : 0x00;
		CyDelayUs(55);
		
		i++;
	}
	
	return b;
	
}

unsigned char OW_CRC(unsigned char *pBuf, int len)
{
	unsigned char loop, i, shiftedBit;
	unsigned char crc = 0x00;
	
	for(loop=0; loop<len; loop++)
	{
		crc = (crc ^ pBuf[loop]);
		
		for(i=8; i>0; i--)
		{
			shiftedBit= (crc & 0x01);
			crc >>= 1;
			
			if(shiftedBit)
			{
				crc = (crc ^ 0x8c);
			}
		}
	}
	
	return crc;
}
