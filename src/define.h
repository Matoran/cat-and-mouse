/*
 * define.h
 *
 *  Created on: May 31, 2017
 *      Author: matoran
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#include "FreeRTOS.h"
#include "queue.h"

extern xQueueHandle mouseQueue;
extern xQueueHandle catQueue;

#define MAX_POS_X 239
#define MAX_POS_Y 299

#endif /* DEFINE_H_ */
