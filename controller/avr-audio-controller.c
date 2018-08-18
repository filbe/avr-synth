#include <avr-sound.h>
#include <twi.h>
#include <util/delay.h>

#define DEVICE_ID 1

void sendnote(uint8_t i2c_addr, uint8_t channel, uint8_t note) {
	uint8_t data[3];
	data[0] = channel;
	data[1] = 1;
	data[2] = note;
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
	/*
	Speed @ 16MHz: 1.12ms per write when sizeof(data) == 4

	*/
}

void sendnotestop(uint8_t i2c_addr, uint8_t channel) {
	uint8_t data[2];
	data[0] = channel;
	data[1] = 0;
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
	/*
	Speed @ 16MHz: 1.12ms per write when sizeof(data) == 4

	*/
}

void sendwaveform(uint8_t i2c_addr, uint8_t wf) {
	uint8_t data[2];
	data[0] = 0;
	data[1] = 0x10+wf;
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
}

void sendfinetune(uint8_t i2c_addr, uint16_t ft) {
	uint8_t data[4];
	data[0] = 0;
	data[1] = 0x20;
	data[2] = ft >> 8;
	data[3] = ft & 0xff;
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
}

void sendarpeggio(uint8_t i2c_addr, uint8_t notescount, uint8_t *notes, uint8_t notedelay) {
	uint8_t data[4+notescount];
	data[0] = 0;
	data[1] = 0x51;
	data[2] = notescount;
	data[3] = notedelay;
	for (uint8_t i=0;i<notescount;i++) {
		data[4+i] = notes[i];
	}
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
}

void sendchord(uint8_t i2c_addr, uint16_t chord, uint8_t base) {
	uint8_t data[5];
	data[0] = 0;
	data[1] = 0x61;
	data[2] = chord >> 8;
	data[3] = chord & 0xff;
	data[4] = base;
	twi_writeTo(i2c_addr,&data,sizeof(data),1);
}




uint8_t song[] = {
	60,60,60,59,59,57,64,64,
	64,64,64,64,64,64,62,60,
	62,62,62,67,67,62,71,71,
	71,71,71,71,71,71,69,71,

	72,72,72,71,71,69,64,64,
	64,64,64,64,64,64,62,60,
	62,62,62,62,62,62,69,71,
	72,71,69,64,62,60,59,55,
};

uint8_t song2[] = {
	33,33,0,33,33,0,33,33,
	0,0,45,47,48,52,57,64,
	31,31,0,31,31,0,31,31,
	0,0,43,45,47,50,55,62,

	29,29,0,29,29,0,29,29,
	0,0,41,43,45,48,53,60,
	31,31,0,31,31,0,31,31,
	55,55,52,52,50,50,43,43,
};

uint8_t song3[] = {
	33,33,0,33,33,0,33,33,
	0,0,45,47,48,52,57,64,
	31,31,0,31,31,0,31,31,
	0,0,43,45,47,50,55,62,

	29,29,0,29,29,0,29,29,
	0,0,41,43,45,48,53,60,
	31,31,0,31,31,0,31,31,
	55,55,52,52,50,50,43,43,
};


float midi[100];
int main(void) {
	for (float i=0;i<100;i+=1.0) {
		midi[(uint8_t)(i)] = pow(2.0, (i-69.0)*0.083333)*440.0;
	}
	DDRC = 255;
	twi_init();	
	uint8_t recv = 1; // address
	
	sei();
	uint16_t c=0;
	
	sendwaveform(1, 2);
	while(1) {
		//sendchord(1, 0b0100000011001100, 60);
		
		sendnote(1, 0, 138);
		_delay_ms(200);
	}
	
	return 0;
}