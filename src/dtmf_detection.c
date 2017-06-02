/**
 * Name        : dtmf_detection.c
 * Date        : 5.5.2017
 * Description : template of the DTMF detector
 */
#include <cr_section_macros.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <queue.h>
#include "adc.h"
#include "debug.h"
#include "objects.h"
#include "dtmf_detection.h"

#define BUF_SIZE 400
#define LIMIT_DETEC 10000
#define FE 8000.
#define PI 3.14159265359
#define STEP 2
#define abs1(x) ((x<0)?(-x):x)

QueueHandle_t xQueue;

bool check_freq(uint16_t* buf, uint16_t freq){
	double w;
	complex sum;
	sum.im = 0;
	sum.re = 0;

	for (int i = 0; i < BUF_SIZE - 1; ++i) {
		w = (2*PI*(852/FE)*i)*PI/180;
		sum.im += buf[i] * sinf(w-BUF_SIZE);
		sum.re += buf[i] * cosf(w-BUF_SIZE);
	}

	if ((abs1(sum.im)+abs1(sum.re)) > LIMIT_DETEC){
		return true;
	}else{
		return false;
	}
}

// if you need additional buffers, enable the next line and declare them after it to place them in local RAM (RAM2 could be full!)
//__attribute__ ((section(".data")))
int direction (uint16_t* buf){
	if (check_freq(buf,852)) {
		if (check_freq(buf,1336)) {
			return NORTH;
		}else if (check_freq(buf,1608)){
			return EAST;
		}
	}else if (check_freq(buf,941)) {
		if (check_freq(buf,1336)) {
			return SOUTH;
		}else if (check_freq(buf,1608)){
			return WEST;
		}
	}

	return -1;
}



// callback function which is called each time one of both RX buffer filled by the DMA from ADC is full
// buf_index: 0 or 1, indicating which buffer is full
void buffer_filled(int buf_index)
{
	xQueueSendToBack(xQeue, &buf_index, portMAX_DELAY);
}

// Initialise the reception of the sound samples
unsigned short* init_dtmf()
{
	unsigned short *sig; // pointer on acquisition double buffer
	// initialise DMA&ADC to receive samples from the audio line at 8000 kHz.
	// The return pointer points on the the double buffer allocated.
	if ((sig=init_adc_dma(1, FE, BUF_SIZE, buffer_filled))==NULL)
		EXIT("Not enough memory to allocate acquisition buffers!");
}

cat_t init_cat(){
	cat_t cat;
	cat.posX = 110;
	cat.oldPosX = cat.posX;
	cat.posY = 30;
	cat.oldPoxY = cat.posY;
	cat.direction = SOUTH;
}

void cat_move(){
	if (xQueue = xQueueCreate( 1, sizeof(int)) == NULL) {
		EXIT("Fail to create DMA queue !");
	};

	if (catQueue = xQueueCreate( 10, sizeof(cat)) == NULL) {
		EXIT("Fail to create DMA queue !");
	};

	init_dtmf();
	cat_t cat = init_cat();

	int pos;
	int buf;
	unsigned short *buf1 = init_dtmf();
	unsigned short *buf2 = *buf1 + BUF_SIZE;

	while(1){

		xQeueReceive(xQeue, &buf,portMAX_DELAY);

		if (buf == 0) {
			cat.direction = direction(buf1);
		}else if (buf == 1){
			cat.direction = direction(buf2);
		}

		if (((cat.oldDirection == NORTH)||(cat.oldDirection == SOUTH)) && ((cat.direction == WEST)||(cat.direction == EAST))) {
			cat.posX += ;
			cat.oldPosX += ;
			cat.posY += ;
			cat.oldPoxY += ;
		}else if (((cat.oldDirection == WEST)||(cat.oldDirection == EAST)) && ((cat.direction == NORTH)||(cat.direction == SOUTH))) {
			cat.posX += ;
			cat.oldPosX += ;
			cat.posY += ;
			cat.oldPoxY += ;
		}

		switch (cat.direction) {
			case NORTH:
				cat.posY = cat.oldPosY - STEP;
				if (cat.posY < 26) {
					cat.posY = cat.oldPoxY;
				}
				break;
			case EAST:
				cat.posX = cat.oldPoxX + STEP;
				if (cat.posY > MAX_POS_X-48) {
					cat.posY = cat.oldPoxY;
				}
				break;
			case SOUTH:
				cat.posY = cat.oldPoxY + STEP;
				if (cat.posY > 252-48) {
					cat.posY = cat.oldPoxY;
				}
				break;
			case WEST:
				cat.posX = cat.oldPosX - STEP;
				if (cat.posX < 0) {
					cat.posX = cat.oldPoxX;
				}
				break;
			default:
				break;
		}


		xQueueSendToBack(catQueue, &cat, portMAX_DELAY);

	}
}
