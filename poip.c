/*
 ============================================================================
 Name        : poip.c
 Author      : Royal Alice Academy of Sciences
 Version     :
 Copyright   : GPL V2.1
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/netmod.h"
#include "includes/tc35.h"
#include "includes/sound.h"

int main()
{
//	char* at = "AT\r\n";
//
//	tc35_init();
//	sleep(1);
//	tc35_write(at, sizeof(at));
//	while(1)
//	{
//		sleep(1);
////		tc35_read();
//	}
//	tc35_close();
	char buf[1024];

	netmod_init(54321);
	sound_init();
	netmod_recv();
	while(1)
	{
		sound_read(buf, 1024);
		netmod_send(buf, 1024);
		sound_write(buf, 1024);
	}

	return 0;
}
