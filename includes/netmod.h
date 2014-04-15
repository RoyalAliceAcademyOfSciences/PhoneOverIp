/*
 * netmod.h
 *
 *  Created on: Oct 10, 2013
 *      Author: ml
 */

#ifndef NETMOD_H_
#define NETMOD_H_

int netmod_init(unsigned short port);
int netmod_recv();
int netmod_send(char* buff, unsigned short len);

#endif /* NETMOD_H_ */
