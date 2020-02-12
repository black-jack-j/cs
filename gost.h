#ifndef LAB2CYBSEC
#define LAB2CYBSEC

#include <stdint.h>

#define BLOCKS 8

/*
	Тут определяются базовые функции шифрования по ГОСТу - фактически это режим ECB
*/

uint64_t encrypt(uint64_t _block);

uint64_t decrypt(uint64_t _block);

void set_key(uint32_t* key);


#endif