#ifndef DTYPE_C
	#define DTYPE_C
#endif

#include "dtype.h"
#include <stddef.h>

void initPacket(struct mmylPacket *packet){
	packet->header = NULL;
	packet->body = NULL;
}
