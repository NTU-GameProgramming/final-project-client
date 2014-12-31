#include <stdio.h>
#include <stdlib.h>


void json_callback() {
	printf("Parser callback is called!.\n");
}

void * data;

void main() {
	json_parser parser;
	if(json_parser_init(&parser, NULL, &json_callback, data)) {
		printf("Something wrong happened during init\n");
	}

	json_parser_free(&parser);
}
