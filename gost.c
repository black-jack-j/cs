#include <limits.h>

#include "gost.h"

#define shl(X, L) (((X) << (L)) | ((X) >> (sizeof((X))*CHAR_BIT - (L)))) 

static const uint32_t *K;

static uint8_t book[8][16] = {
  {1,7,14,13,0,5,8,3,4,15,10,6,9,12,11,2},
  {8,14,2,5,6,9,1,12,15,4,11,0,13,10,3,7},
  {5,13,15,6,9,2,12,10,11,7,8,1,4,3,14,0},
  {7,15,5,10,8,1,6,13,0,9,3,14,11,4,2,12},
  {12,8,2,1,13,4,15,6,7,0,10,5,3,14,9,11},
  {11,3,5,8,2,15,10,13,14,1,7,4,12,9,6,0},
  {6,8,2,3,9,10,5,12,1,14,4,7,11,13,0,15},
  {12,4,6,2,10,5,11,9,14,8,13,7,0,3,15,1}
};

static uint64_t base(uint64_t _block, uint32_t _key);

static uint32_t tReplace(uint32_t _block);

uint64_t encrypt(uint64_t _block){
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[7-i]);
	}
	_block = shl(_block, 32);
	return _block;
}

uint64_t decrypt(uint64_t _block){
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[7-i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[7-i]);
	}
	for(int i = 0; i < 8; i++){
		_block = base(_block, K[7-i]);
	}
	_block = shl(_block, 32);
	return _block;
}

void set_key(uint32_t* key){
	K = key;
}

static uint64_t base(uint64_t _block, uint32_t _key){
	uint32_t left, right, temp;
	uint64_t result = 0;

	left = _block >> 32;
	right = _block;

	temp = tReplace(right ^ _key);
	temp = shl(temp, 11);
	temp = temp ^ left;

	result |= right;
	result <<= 32;
	result |= temp;

	return result;
}

static uint32_t tReplace(uint32_t _block){
	uint32_t result = 0;
	uint32_t mask = 0xF0000000;
	uint8_t temp;
	for(int i = 0; i < 8; i++){
		result <<=4;
		temp = (_block & mask) >> ((7-i)*4);
		result |= book[i][temp];
		mask >>= 4;
	}
	return result;
}
