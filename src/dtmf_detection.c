/**
 * Name        : dtmf_detection.c
 * Date        : 5.5.2017
 * Description : template of the DTMF detector
 */
#include <cr_section_macros.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "adc.h"
#include "debug.h"
#include "objects.h"

#define BUF_SIZE 400
#define FE 8000.
#define PI 3.14159265359



// if you need additional buffers, enable the next line and declare them after it to place them in local RAM (RAM2 could be full!)
//__attribute__ ((section(".data")))





// callback function which is called each time one of both RX buffer filled by the DMA from ADC is full
// buf_index: 0 or 1, indicating which buffer is full
void buffer_filled(int buf_index)
{

}

// Initialise the reception of the sound samples
void init_dtmf()
{
	unsigned short *sig;       // pointer on acquisition double buffer
	// initialise DMA&ADC to receive samples from the audio line at 8000 kHz.
	// The return pointer points on the the double buffer allocated.
	if ((sig=init_adc_dma(1, FE, BUF_SIZE, buffer_filled))==NULL)
		EXIT("Not enough memory to allocate acquisition buffers!");
}


