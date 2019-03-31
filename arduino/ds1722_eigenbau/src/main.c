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
	
	write_frame_to_master("INIT\n");
	spi_init();
	write_frame_to_master("SPI INIT DONE\n");
	uint8_t got = SPCR;
	char recv[20];
	memset(recv, 0, sizeof(recv));
	snprintf(recv, sizeof(recv), "SPCR: %X\n", got);
	write_frame_to_master(recv);
	
	sei();
	
	
	while(1){
                if(routine() < 0){
                }
		_delay_ms(100);

        }

        return 0;
}

int routine(){
	/* Clock at 125kHz */
	
	ENABLE_CHIP();

	uint8_t got1 = spi_transmit(0);
	uint8_t got2 = spi_transmit(0);
	uint8_t got3 = spi_transmit(0);
	uint8_t got4 = spi_transmit(0);

	DISABLE_CHIP();
	char recv[20];
	memset(recv, 0, sizeof(recv));
	snprintf(recv, sizeof(recv), "GOT1: %u\n", got1);
	write_frame_to_master(recv);
	
	memset(recv, 0, sizeof(recv));
	snprintf(recv, sizeof(recv), "GOT2: %u\n", got2);
	write_frame_to_master(recv);
	
	memset(recv, 0, sizeof(recv));
	snprintf(recv, sizeof(recv), "GOT3: %u\n", got3);
	write_frame_to_master(recv);
	
	memset(recv, 0, sizeof(recv));
	snprintf(recv, sizeof(recv), "GOT4: %u\n", got4);
	write_frame_to_master(recv);

        return 0;
}
