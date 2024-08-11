#pragma once

#ifndef DTYPE_H
	#define DTYPE_H
#endif

struct mmylPacket{
	char *header;
	char *body;
};

struct packetHandler{
	char *mainType;
	char *subType;
	unsigned int size;
	char *jokeType;
};

void initPacket(struct mmylPacket *packet);
