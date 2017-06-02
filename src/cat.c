/*
 * cat.c
 *
 *  Created on: Jun 2, 2017
 *      Author: matoran
 */

#include "cat.h"
#include "FreeRTOS.h"
#include "task.h"

void draw_cat(cat_t *old, cat_t *new, sprites_t *sprites){

}

void task_cat(void *param){
	portTickType xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, 50 / portTICK_RATE_MS);
	}
}
