#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "prints.h"

#define TRUE 1
#define FALSE 0

int valid_size(int);
int valid_input(char *);
int valid_processors(char *);
int valid_interations(char *);

int check_input(int argc, char *argv[]) {
  if (!valid_size(argc)) {
    print_usage_error();
    return FALSE;
  }
  if (!valid_input(argv[1])) {
    print_file_error(argv[1]);
    return FALSE;
  }
  if (!valid_processors(argv[3])) {
    print_number_error(argv[3]);
    return FALSE;
  }
  if(!valid_interations(argv[4])) {
    print_number_error(argv[4]);
    return FALSE;
  }

  return TRUE;
}

int valid_size(int argc) {
  if (argc != 5) return FALSE;
  return TRUE;
}

int valid_input(char *input_name) {
  FILE *input;
  input = fopen (input_name,"r");

  if (input!=NULL) {
    fclose (input);
    return TRUE;
  }

  return FALSE;
}

int valid_processors(char *n) {
  return atoi(n);
}

int valid_interations(char *n) {
  return atoi(n);
}
