/* Controller software for the arduino for the painter at the escape game ibk
 * Copyright © 2018 tyrolyean
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "serial.h"
#include "mfrc522.h"
#include "spix.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <stdlib.h>

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
void get_mcusr(void) \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void)
{
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

void mfrc522_check_reader()
{
	uint8_t curr_read_status = mfrc522_get_version();
	if (curr_read_status<0x90 || curr_read_status>0x92)
	{
		write_frame_to_master("MFRC522 NOT present on spi\n");
		for(;;);
	}
}


int routine();
int main(){

	
	/* Disable interrupts during initialisation phase */	
	cli();

	/* initialize the uart connection to the controller */
        serial_init();

	//sei();
	/* Enable the hardware watchdog. In case the microcontroller fails to 
	 * finish it's task within the specified time, the watchdog will reset
	 * the atmel cookie.
	 */
	//wdt_enable(WDTO_2S);
	
	write_frame_to_master("INIT\n");
	spi_init();

	
	while(1){
		wdt_reset();
                if(routine() < 0){
                }


        }

        return 0;
}

uint8_t rc522_read_card_id(uint8_t *card_id)
{
	uint8_t status, resx = 0;
	uint8_t buff_data[MAX_LEN]; 
	uint16_t is_present = 0;
	if(mfrc522_is_card(&is_present))
	{
		status = mfrc522_get_card_serial(buff_data);
		if (status==CARD_FOUND)
		{
			memcpy(card_id,buff_data,5);//kopi id and checksum at last byte (5th)
			resx = 1;
		}
		else
		{
			resx = 0;
		}
	}
	else
	{
		resx = 0;
	}
	
	return resx;
}


int routine(){
	char s[20];
	s[0] = 0;

	/*
	uint8_t rec = SPI_MasterTransmit(0b10001010);
	PORTD |= 1 << 3;
	
	sprintf(s,"D:%x\n", rec);*/


	uint32_t id = 0;

	if(rc522_read_card_id((uint8_t*) &id)){

		sprintf(s,"ID:%lu\n", id);
		write_frame_to_master(s);

	}
        return 0;
}
