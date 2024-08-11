%option yylineno noyywrap noinput nounput
%{
#ifndef DECODE_C
	#define DECODE_C
#endif

#include "decode.h"

YY_BUFFER_STATE bp;

struct packetHandler h = {.mainType = NULL, .subType = NULL,
			.size = 0, .jokeType = NULL};

%}

%%
"Content-Type: "[a-z/]+	{ 
			unsigned int length = strchr(yytext, ' ') - yytext;
			unsigned int slash = strchr(yytext, '/') - yytext;
			
			h.mainType = (char *)malloc(slash - length);
			h.subType = (char *)malloc(strlen(yytext) - slash + 1);
				
			memset(h.mainType, 0, slash - length);
			memset(h.subType, 0, strlen(yytext) - slash + 1);
			
			strncpy(h.mainType, yytext + length + 1, slash - length - 1);
			strncpy(h.subType, yytext + slash + 1, strlen(yytext) - slash);
			}

"Accept: "[a-z/]+	{
			unsigned int length = strchr(yytext, ' ') - yytext; 
                        unsigned int slash = strchr(yytext, '/') - yytext;

                        h.mainType = (char *)malloc(slash - length);
                        h.subType = (char *)malloc(strlen(yytext) - slash + 1);
                                
                        memset(h.mainType, 0, slash - length);
                        memset(h.subType, 0, strlen(yytext) - slash + 1);

                        strncpy(h.mainType, yytext + length + 1, slash - length - 1);
                        strncpy(h.subType, yytext + slash + 1, strlen(yytext) - slash);
			}

"Content-Length: "[0-9]+ {
			sscanf(yytext, "Content-Length: %u", &h.size);	
			}

"Joke-Type: "[a-z]+	{
			unsigned int length = strchr(yytext, ' ') - yytext;

			h.jokeType = (char *)malloc(strlen(yytext) - length);

			memset(h.jokeType, 0, strlen(yytext) - length);

			strncpy(h.jokeType, yytext + length + 1, strlen(yytext) - length - 1);
			}

"\n"			{ /* ignore */ }
.			{ /* ignore */ }
%%

int parsePacket(char *str, struct mmylPacket *packet, struct packetHandler *handler){
	int ret = 0;
	
	/* Split str to packet header and body. */		
	unsigned int length = strchr(str, '\r') - str;
	packet->header = (char *)malloc(length + 1);
	packet->body = (char *)malloc(strlen(str) - length - 1);

	memset(packet->header, 0, length + 1);
	memset(packet->body, 0, strlen(str) - length - 1);

	strncpy(packet->header, str, length);
	strncpy(packet->body, str + length + 2, strlen(str) - length - 2);	

	/* Switch lex buffer from stdin to str. */
	bp = yy_scan_string(packet->header);
	yy_switch_to_buffer(bp);

	ret = yylex();

	yy_delete_buffer(bp);

	/* Assign packet handler information. */
	handler->mainType = h.mainType;
	handler->subType = h.subType;
	handler->size = h.size;
	handler->jokeType = h.jokeType;

	return ret;
}

void yyerror(char* s, ...) {
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
 	fprintf(stderr, "\n");
	va_end(args);
}
