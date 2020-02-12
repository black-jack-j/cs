#ifndef CFB_MODE
#define CFB_MODE

#include <stdio.h>
#include <stdint.h>

/*
  	Тут определены функции шифрования и дешифрования в режиме CFB - 
  		в данном режиме можно шифровать данные, размер которых меньше, чем размер блока (8 байт)
*/

typedef ssize_t (*io_read)(void*, size_t);

typedef ssize_t (*io_write)(const void *, size_t);

int decrypt_cfb(io_read from, io_write to);

int encrypt_cfb(io_read from, io_write to);

void set_IV(uint64_t IV);



#endif