/*
 * cat.h
 *
 *  Created on: Jun 2, 2017
 *      Author: matoran
 */

#ifndef CAT_H_
#define CAT_H_

#include "dtmf_detection.h"

void draw_cat(cat_t *old, cat_t *new, sprites_t *sprites);
void task_cat(void *param);

#endif /* CAT_H_ */
