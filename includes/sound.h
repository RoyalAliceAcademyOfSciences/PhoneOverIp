/*
 * sound.h
 *
 *  Created on: Oct 21, 2013
 *      Author: ml
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <sys/types.h>

void sound_init();
void sound_set(uint bits, uint channels, uint rate);
void sound_read(char* buf, uint size);
void sound_write(char* buf, uint size);

#endif /* SOUND_H_ */
