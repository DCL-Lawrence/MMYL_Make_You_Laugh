#pragma once

#ifndef ENCODE_H
	#define ENCODE_H
#endif

#include "header/header.h"
#include "header/jtype.h"
#include "header/mtype.h"
#include "dtype.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int setHeader(struct mmylPacket *packet, char *key, char *value);

int setBody(struct mmylPacket *packet, char *main_type, char *subtype, char *data, unsigned int size);

char *toString(struct mmylPacket *packet);
