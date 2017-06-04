/**
 * @authors: LOPES Marco, ISELI Cyril and RINGOT Gaëtan
 * Purpose: dtmf_detection
 * Language:  C
 * Date : june 2017
 */
#include <cr_section_macros.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "adc.h"
#include "debug.h"
#include "objects.h"
#include "dtmf_detection.h"

// if you need additional buffers, enable the next line and declare them after it to place them in local RAM (RAM2 could be full!)
//__attribute__ ((section(".data")))

float sin_values[4][BUF_SIZE];
float cos_values[4][BUF_SIZE];


/**
 * Init all data for sin and cos
 * It's usefull because it need time
 * but never change.
 */
void init_values(){
	float w;
	float freq[4] = {FREQ1, FREQ2, FREQ3, FREQ4};
	for (int f = 0; f < 4; f++) {
		for (int i = 0; i < BUF_SIZE; i++) {
			w = (2*PI*((float)i*freq[f]/FE));
			sin_values[f][i] = sinf(w);
			cos_values[f][i] = cosf(w);
		}
	}
}

/**
 * Test 2 frequencies
 * @param1 Buffer to use in calculation
 * @param2 0 = 852/941, 2 = 1336/1608
 * @return Frequency
 */
int check_freq(uint16_t* buf,int numCheck){
	complex res[2];
	for(int i = 0; i < 2; i++){
		res[i].re = 0;
		res[i].im = 0;
	}

	for (int i = 0; i < BUF_SIZE ; i++) {
		float val = buf[i]  - (INT16_MAX >> 1);
		res[0].im -= val * cos_values[numCheck][i];
		res[0].re += val * sin_values[numCheck][i];

		res[1].im -= val * cos_values[numCheck+1][i];
		res[1].re += val * sin_values[numCheck+1][i];
	}


	if (numCheck == 0) {
		if ((abs1(res[0].im)+abs1(res[0].re)) > LIMIT_DETEC) return FREQ1;
		if ((abs1(res[1].im)+abs1(res[1].re)) > LIMIT_DETEC) return FREQ2;
	}else if (numCheck == 2) {
		float freq3 = (abs1(res[0].im)+abs1(res[0].re));
		float freq4 = (abs1(res[1].im)+abs1(res[1].re));
		//second try
		// if ((freq4 > LIMIT_DETEC) && (freq4 > freq3)) return FREQ4;
		if ((freq3 > LIMIT_DETEC)) return FREQ3;
		if ((freq4 > LIMIT_DETEC)) return FREQ4;

	}
	return -1;
}


/**
 * Check which direction is in the buffer
 * @param1 Buffer to use in calculation
 * @return Direction or -1 if no direction
 */
int direction (uint16_t* buf){
	int resFreq1 = check_freq(buf,0);
	int resFreq2 = check_freq(buf,2);

	if (resFreq1 == FREQ1) {
		if (resFreq2 == FREQ3) {
			return NORTH;
		}else if (resFreq2 == FREQ4){
			return SOUTH;
		}
	}else if (resFreq1 == FREQ2) {
		if (resFreq2 == FREQ3) {
			return EAST;
		}else if (resFreq2 == FREQ4){
			return WEST;
		}
	}
	return NONE;
}



// callback function which is called each time one of both RX buffer filled by the DMA from ADC is full
// buf_index: 0 or 1, indicating which buffer is full
void buffer_filled(int buf_index)
{
	xQueueSendFromISR(xQueue, &buf_index, 0);
}

// Initialise the reception of the sound samples
unsigned short* init_dtmf()
{
	init_values();
	unsigned short *sig; // pointer on acquisition double buffer
	// initialise DMA&ADC to receive samples from the audio line at 8000 kHz.
	// The return pointer points on the the double buffer allocated.
	if ((sig=init_adc_dma(1, FE, BUF_SIZE, buffer_filled))==NULL)
		EXIT("Not enough memory to allocate acquisition buffers!");
	return sig;
}
