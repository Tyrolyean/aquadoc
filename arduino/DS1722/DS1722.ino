#define DATAOUT 11 //MOSI - Master Input Slave Output
#define DATAIN  12 //MISO - Master Output Slave Input
#define SPICLOCK  13 //SCK -- Serial Clock
#define SLAVESELECT 10 //SS -- Slave Select
#define DS1722_POWER 9

#define DS1722_SELECT HIGH
#define DS1722_DESELECT LOW

#define DS1722_CONFIG_BYTE 0xEE
#define CONFIG_REG_READ 0x00
#define CONFIG_REG_WRITE 0x80
#define TEMP_ADDR_HI 0x02
#define TEMP_ADDR_LOW 0x01

byte clr;
byte temperature[2];

char spi_transfer(volatile char data){

	// Start the transmission
	SPDR = data;// Wait the end of the transmission
	while (!(SPSR & (1<<SPIF))){};
	// return the received byte
	return SPDR;
}

void setup(){

	byte n, config = 0xAB;
	Serial.begin(9600);

	temperature[0] = 0x12;
	temperature[1] = 0x34;
	
	/* Set DDIR registers */

	pinMode(DATAOUT, OUTPUT);
	pinMode(DATAIN, INPUT);
	pinMode(SPICLOCK, OUTPUT);
	pinMode(SLAVESELECT, OUTPUT);
	pinMode(DS1722_POWER, OUTPUT);

	digitalWrite(DS1722_POWER, HIGH); //disable device
	delay(250);

	digitalWrite(SLAVESELECT, DS1722_DESELECT); //disable device
	// set up SPI control register
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA);
	clr=SPSR;
	clr=SPDR;
	delay(10);

	// read config byte
	digitalWrite(SLAVESELECT, DS1722_SELECT);
	spi_transfer(CONFIG_REG_READ);
	config = spi_transfer(0xFF);
	digitalWrite(SLAVESELECT, DS1722_DESELECT);
	delay(100);

	Serial.print(config, HEX);
	Serial.print('\n', BYTE);

	// write config byte to the configuration register
	digitalWrite(SLAVESELECT, DS1722_SELECT);
	spi_transfer(CONFIG_REG_WRITE);
	spi_transfer(DS1722_CONFIG_BYTE);
	digitalWrite(SLAVESELECT, DS1722_DESELECT);
	delay(100);

	// read config byte
	digitalWrite(SLAVESELECT, DS1722_SELECT);
	spi_transfer(CONFIG_REG_READ);
	config = spi_transfer(0xFF);
	digitalWrite(SLAVESELECT, DS1722_DESELECT);
	delay(100);

	Serial.print(config, HEX);
	Serial.print('\n', BYTE);

	Serial.print('h', BYTE);
	Serial.print('i', BYTE);
	Serial.print('\n', BYTE);//debug
	delay(1000);
	
	return;
}

void loop(){

	//float c, f;
	// write config byte to the configuration register
	digitalWrite(SLAVESELECT, DS1722_SELECT);
	spi_transfer(CONFIG_REG_WRITE);
	spi_transfer(DS1722_CONFIG_BYTE);
	digitalWrite(SLAVESELECT, DS1722_DESELECT);

	delay(1400);

	digitalWrite(SLAVESELECT,DS1722_SELECT);
	spi_transfer(TEMP_ADDR_HI);
	temperature[0] = spi_transfer(0x00);
	//release chip, signal end transfer
	digitalWrite(SLAVESELECT, DS1722_DESELECT);
	delay(25); // just because....
	digitalWrite(SLAVESELECT,DS1722_SELECT);
	spi_transfer(TEMP_ADDR_LOW);
	temperature[1] = spi_transfer(0x00);
	//release chip, signal end transfer
	digitalWrite(SLAVESELECT, DS1722_DESELECT); 
	Serial.print(temperature[0] * 9 / 5 + 32, DEC);
	Serial.print(' ', BYTE);
	Serial.print(temperature[0], DEC);
	
	if(temperature[1] & 0x80){

		Serial.print( '.', BYTE);
		Serial.print( '5', BYTE);
	}
	
	Serial.print('\r', BYTE);
	Serial.print('\n', BYTE);

	delay(2000);
	
	return;
 }
