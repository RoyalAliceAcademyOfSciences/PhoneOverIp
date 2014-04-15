/*
 * tc35.c
 *
 *  Created on: Oct 6, 2013
 *      Author: ml
 */

#include "tc35.h"
#include "pdu.h"

#define FALSE 0
#define TRUE 1
#define _POSIX_SOURCE 1 /* POSIX 系统相容*/

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"

void signal_handler_IO(int status); /* 定义讯号处理程序*/
//int wait_flag = TRUE; /* 没收到讯号的话就会是TRUE */

int fd;
struct termios oldtio, newtio;
struct sigaction saio; /* definition of signal action */

int tc35_init()
{

	/* 开启装置为non-blocking (读取功能会马上结束返回) */
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0)
	{
		perror(MODEMDEVICE);
		exit(-1);
	}

	/* 在使装置非同步化前, 安装讯号处理程序*/
	memset(&saio, 0, sizeof(saio));
	saio.sa_handler = signal_handler_IO;
//	saio.sa_mask = 0;
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO, &saio, NULL );

	/* 允许行程去接收SIGIO 讯号*/
	fcntl(fd, F_SETOWN, getpid());
	/* 使档案a​​ke the file descriptor 非同步(使用手册上说只有O_APPEND 及
	 O_NONBLOCK, 而F_SETFL 也可以用...) */
	fcntl(fd, F_SETFL, FASYNC);

	tcgetattr(fd, &oldtio); /* 储存目前的序列埠设定值*/
	/* 设定新的序列埠为标准输入程序*/
	memset(&newtio, 0, sizeof(newtio));
//	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag = ICANON;
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	return 0;
}

int tc35_close()
{
	/* 回存旧的序列埠设定值*/
	tcsetattr(fd, TCSANOW, &oldtio);
	return 0;
}

int tc35_write(char* data, size_t size)
{
	return write(fd, data, size);
}

int tc35_call_dial(char* number)
{
	char buf[255];
	sprintf(buf, "ATD%s;\r\n", number);
	write(fd, buf, strlen(buf));

	return 0;
}

int tc35_call_control(char hang)
{
	char* ata = "ATA\r\n";
	char* ath = "ATH\r\n";
	if(hang == 0)
		write(fd, ata, sizeof(ata));
	else
		write(fd, ath, sizeof(ath));

	return 0;
}

int tc35_sms_send(const char* service_center_number, const char* phone_number, const char* text)
{
	unsigned char buf[SMS_MAX_PDU_LENGTH];
	int len;
	char* cmgf = "AT+CMGF=0\r\n";
	char* cmgs = "AT+CMGS=20\r\n";

	write(fd, cmgf, sizeof(cmgf));
	write(fd, cmgs, sizeof(cmgs));

	len = pdu_encode(service_center_number, phone_number, text, buf, SMS_MAX_PDU_LENGTH);
	buf[len] = 0;
	write(fd, buf, len);

	return 0;
}

int tc35_sms_recv()
{
	return 0;
}

void signal_handler_IO(int status)
{
	char buf[255];
	int res;

	res = read(fd, buf, 255);
	buf[res] = 0;
	printf("received SIGIO signal [status:%d] [len:%d].\n", status, res);
	printf("%s\n", buf);
}
