#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>

#include "cfb.h"
#include "gost.h"

#define rmask(X, Y) ((X) & ((1UL << (Y)) - 1))

#define lmask(X, Y) ((X) & (((1UL << (Y)) - 1) << ((sizeof((X))*CHAR_BIT)-(Y))))

#define mask(X) ((1UL << (X)) - 1)

typedef struct {
	uint64_t _data;
} shift64_t;

static uint64_t IV = 0xDEAD6666DEAD6666;

static shift64_t SHIFT;

static void shift_left(shift64_t* reg, uint64_t tail, uint8_t bits){
	if(bits == 64){
		reg->_data = tail;
		return;
	}else if (bits > 64) return;
	reg->_data = (reg->_data << bits) | rmask(tail, bits);
}

void set_IV(uint64_t _v){
	IV = _v;
}

int encrypt_cfb(io_read from, io_write to){
	uint64_t acc, buf = 0;
	ssize_t res;

	shift_left(&SHIFT, IV, 64);

	while(1){
		while((res = from(&buf, sizeof(uint64_t))) != 0){
			acc = encrypt(SHIFT._data);
			if(res < sizeof(uint64_t)){
				acc = lmask(acc, res*CHAR_BIT) >> ((sizeof(uint64_t)-res)*CHAR_BIT);
				buf = rmask(buf, res*CHAR_BIT);
			}
			buf ^= acc;
			if(res < sizeof(uint64_t)){
				buf = rmask(buf, res*CHAR_BIT);
			}
			to(&buf, res);
			shift_left(&SHIFT, buf, res*CHAR_BIT);
		}
		break;
	}
	return 0;
}

int decrypt_cfb(io_read from, io_write to){
	uint64_t acc, buf = 0;
	ssize_t res;

	shift_left(&SHIFT, IV, 64);

	while(1){
		while((res = from(&buf, sizeof(uint64_t))) != 0){
			acc = encrypt(SHIFT._data);
			if(res < sizeof(uint64_t)){
				acc = lmask(acc, res*CHAR_BIT) >> ((sizeof(uint64_t)-res)*CHAR_BIT);
				buf = rmask(buf, res*CHAR_BIT);
			}
			acc = buf ^ acc;
			if(res < sizeof(uint64_t)){
				acc = rmask(acc, res*CHAR_BIT);
			}
			to(&acc, res);
			shift_left(&SHIFT, buf, res*CHAR_BIT);
		}
		break;
	}
	return 0;
}