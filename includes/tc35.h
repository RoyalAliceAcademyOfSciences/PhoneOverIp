/*
 * tc35.h
 *
 *  Created on: Oct 6, 2013
 *      Author: ml
 */

#ifndef TC35_H_
#define TC35_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

int tc35_write(char* data, size_t size);
int tc35_read();
int tc35_init();
int tc35_close();
int tc35_call_dial(char* number);
int tc35_call_control(char hang);
int tc35_sms_send(const char* service_center_number, const char* phone_number, const char* text);
int tc35_sms_recv();

#endif /* TC35_H_ */
