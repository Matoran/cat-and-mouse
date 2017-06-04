/**
 * @authors: LOPES Marco, ISELI Cyril and RINGOT Gaëtan
 * Purpose: cat
 * Language:  C
 * Date : june 2017
 */

#ifndef CAT_H_
#define CAT_H_

#include "dtmf_detection.h"
#include "objects.h"

#define JOYSTICK_LEFT	 23
#define JOYSTICK_RIGHT	 21
#define JOYSTICK_TOP	 20
#define JOYSTICK_BOTTOM	 22
#define STEP_CAT		 4

typedef struct cat_t{
	object_t object;
}cat_t;

void init_cat(cat_t *cat);
void draw_cat(cat_t *old, cat_t *new, sprites_t *sprites);
void task_cat(void *param);

#endif /* CAT_H_ */
