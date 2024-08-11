#ifndef ENCODE_C
	#define ENCODE_C
#endif

#include "encode.h"

int setHeader(struct mmylPacket *packet, char *key, char *value){
	int ret = 0;

	if(packet->header == NULL){
		packet->header = (char *)malloc(strlen(key) + strlen(value) + 4);
		if(packet->header == NULL){
			ret = 1;
			goto err;
		}
		
		sprintf(packet->header, "%s: %s\n", key, value);	
	}else{
		packet->header = (char *)realloc(packet->header, 
				strlen(packet->header) + strlen(key) + strlen(value) + 4);
		if(packet->header == NULL){
                        ret = 1;
                        goto err;
                }

		char *temp = (char *)malloc(strlen(key) + strlen(value) + 4);
		sprintf(temp, "%s: %s\n", key, value);

		strncat(packet->header, temp, strlen(temp) + 1);

		free(temp);
		temp = NULL;
	}

err:
	return ret;
}

int setBody(struct mmylPacket *packet, char *main_type, char *subtype, char *data, unsigned int size){
	int ret = 0;
	
	packet->body = (char *)malloc(strlen(data) + 1);
	if(packet->body == NULL){
		ret = 1;
		goto err;
	}

	memset(packet->body, 0, strlen(data) + 1);
	sprintf(packet->body, "%s", data);

	char *temp = (char *)malloc(strlen(main_type) + strlen(subtype) + 2);
	sprintf(temp, "%s/%s", main_type, subtype);

	ret = setHeader(packet, CONTENT, temp);
	if(ret)
		goto err_with_free;

	sprintf(temp, "%u", size);
	
	ret = setHeader(packet, LENGTH, temp);
	if(ret)
		goto err_with_free;

err_with_free:
	free(temp);
	temp = NULL;
err:
	return ret;
}

char *toString(struct mmylPacket *packet){
	char *str = NULL;

	if(packet->body == NULL){
		str = (char *)malloc(strlen(packet->header) + 3);
		sprintf(str, "%s\r\n", packet->header);
	}else{
		str = (char *)malloc(strlen(packet->header) + strlen(packet->body) + 3);
		sprintf(str, "%s\r\n%s", packet->header, packet->body);
	}

	return str;
}
