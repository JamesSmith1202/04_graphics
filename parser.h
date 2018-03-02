#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"

char ** parse_args(char * str);
void parse_file ( char * filename, 
		  struct matrix * transform, 
		  struct matrix * edges,
		  screen s);

#endif
