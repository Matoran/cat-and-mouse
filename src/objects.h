/*
 * objects.h
 *
 *  Created on: 12 avr. 2017
 *      Author: pillouxv
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <stdint.h>

#define TOP_POS 26

typedef enum {
	NORTH=0,
	EAST,
	SOUTH,
	WEST,
	NONE
} dir_t;

typedef struct {
	int x;
	int y;
} pos_t;

typedef struct {
	uint16_t *bitmap;			// index 0 is north, 1 east, etc.
	uint16_t width, height;		// of bitmap
} image_t;

typedef struct {
	image_t cat_im[4], mouse_im[4], cheese_im;
} sprites_t;

typedef struct {
	pos_t pos;
	dir_t dir;
} object_t;


#endif /* OBJECTS_H_ */
