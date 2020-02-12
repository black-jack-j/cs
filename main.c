#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>

#include "gost.h"
#include "cfb.h" 

static uint32_t K[8] = {0, 1, 2, 0xDEADFEED, 4, 5, 6, 7};

static const uint64_t IV = 0xDEAD6666DEAD6666;

ssize_t read1(void* buf, size_t len){
	return read(STDIN_FILENO, buf, len);
}

ssize_t write1(const void *buf, size_t len){
	return write(STDOUT_FILENO, buf, len);
}

int main(int argc, char *argv[])
{
	int mode = 0;
	int opt;
	while((opt = getopt(argc, argv, "cd")) != -1){
		switch(opt){
			case 'c':{
				mode = 0;
				break;
			}
			case 'd':{
				mode = 1;
				break;
			}
		}
	}
	set_key(K);
	set_IV(IV);
	if(mode){
		decrypt_cfb(&read1, &write1);
	}else{
		encrypt_cfb(&read1, &write1);
	}
	return 0;
}