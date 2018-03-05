#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

/*-------------- int * parse_args() --------------
Inputs:  char * str
Returns: char **

Splits a string on spaces and adds each argument to the array;
*/
char ** parse_args(char * str){
  short counter = 0;
  char ** arr = (char **)malloc(sizeof(char *) * 7);
  char * tmp;
  while((tmp = strsep(&str, " "))){
    arr[counter] = tmp;
    counter++;
  }
  arr[counter]=0;
  return arr;
} 

/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - x
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
  char ** arg_arr;
  struct matrix * tmp_mat;

  color c;
  c.red = 150;
  c.green = 150;
  c.blue = 150;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    if (!strcmp(line, "line")){
        fgets(line, 255, f);
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);
        arg_arr = parse_args(line);
        add_edge(edges, atof(arg_arr[0]), atof(arg_arr[1]), atof(arg_arr[2]), atof(arg_arr[3]), atof(arg_arr[4]), atof(arg_arr[5]));
        free(arg_arr);
    }
    else if (!strcmp(line, "scale")){
        fgets(line, 255, f);
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);
        arg_arr = parse_args(line);
        tmp_mat = make_scale(atof(arg_arr[0]), atof(arg_arr[1]), atof(arg_arr[2]));
        matrix_mult(tmp_mat, transform);
        free(tmp_mat);
        free(arg_arr);
    }
    else if (!strcmp(line, "move")){
        fgets(line, 255, f);
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);
        arg_arr = parse_args(line);
        tmp_mat = make_translate(atof(arg_arr[0]), atof(arg_arr[1]), atof(arg_arr[2]));
        matrix_mult(tmp_mat, transform);
        free(tmp_mat);
        free(arg_arr);
    }
    else if (!strcmp(line, "rotate")){
        fgets(line, 255, f);
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);
        arg_arr = parse_args(line);
        if(!strcmp(arg_arr[0],"x")){
            tmp_mat = make_rotX(atof(arg_arr[1]));
        }
        else if(!strcmp(arg_arr[0], "y")){
            tmp_mat = make_rotY(atof(arg_arr[1]));
        }
        else if(!strcmp(arg_arr[0], "z")){
            tmp_mat = make_rotZ(atof(arg_arr[1]));
        }
        else{
            printf("Error: unknown axis of rotation\n");
        }
        matrix_mult(tmp_mat, transform);
        free(tmp_mat);
        free(arg_arr);
    }
    else if (!strcmp(line, "save")){
        fgets(line, 255, f);
        line[strlen(line)-1]='\0';
        printf(":%s:\n",line);
        draw_lines(edges, s, c);
        save_extension(s, line);
        clear_screen(s);
    }
    else if (!strcmp(line, "ident")){
        ident(transform);
    }
    else if (!strcmp(line, "apply")){
        matrix_mult(transform, edges);
        ident(transform);
    }
    else if (!strcmp(line, "display")){
        draw_lines(edges, s, c);
        display(s);
        clear_screen(s);
    }
    else if (!strcmp(line, "quit")){
        exit(1);
    }
    else{   
        printf("Error: Unknown Command\n");
    }
  }
}
  
