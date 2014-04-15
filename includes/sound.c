/*
 * sound.c
 *
 *  Created on: Oct 21, 2013
 *      Author: ml
 */

#include "sound.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>

/* 用于保存数字音频数据的内存缓冲区 */
//unsigned char buf[LENGTH * RATE * SIZE * CHANNELS / 8];
static int fd; /* 声音设备的文件描述符 */

//void sound_set(uint bits, uint channels, uint rate)

void sound_init()
{
	/* 打开声音设备 */
	fd = open("/dev/dsp", O_RDWR);
	if (fd < 0)
	{
		perror("open of /dev/dsp failed");
	}
	sound_set(8, 1, 8000);
}

void sound_set(uint bits, uint channels, uint rate)
{
	int arg; /* 用于ioctl调用的参数 */
	int status; /* 系统调用的返回值 */
	/* 设置采样时的量化位数 */
	arg = bits;
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_BITS ioctl failed");
	if (arg != bits)
		perror("unable to set sample size");
	/* 设置采样时的声道数目 */
	arg = channels;
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
	if (arg != channels)
		perror("unable to set number of channels");
	/* 设置采样时的采样频率 */
	arg = rate;
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");
}

void sound_read(char* buf, uint size)
{
	int status; /* 系统调用的返回值 */
	status = read(fd, buf, size); /* 录音 */
	if (status != size)
		perror("read wrong number of bytes");
}

void sound_write(char* buf, uint size)
{
	int status; /* 系统调用的返回值 */
	status = write(fd, buf, size); /* 回放 */
	if (status != size)
		perror("wrote wrong number of bytes");
}
