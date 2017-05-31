/**
 * @authors: LOPES Marco, ISELI Cyril and RINGOT Gaëtan
 * Purpose: random numbers
 * Language:  C
 * Date : april 2017
 */
#include "random.h"
#include "custom_rand.h"

/**
 * generate a number between ]min and max[
 * @param min
 * @param max
 * @return random number
 */
uint32_t randBetween(uint32_t min, uint32_t max){
	return rnd_32() % (max + 1 - min) + min;
}
