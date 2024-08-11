#pragma once

#ifndef DECODE_H
	#define DECODE_H
#endif

#include "header/header.h"
#include "header/jtype.h"
#include "header/mtype.h"
#include "dtype.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

int parsePacket(char *str, struct mmylPacket *packet, struct packetHandler *handler);

int yylex();

void yyerror(char* s, ...);
