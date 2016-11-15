#include "input.h"
#include "prints.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_comment_from_file(FILE *);
int*** set_image(FILE *, int, int);

int main(int argc, char *argv[]) {
  if (!check_input(argc, argv)) return 1;

  char file_type[2];
  int width;
  int height;
  FILE *input;

  input = fopen (argv[1],"r");
  fscanf(input, "%s", file_type);
  if(strcmp(file_type, "P3")) {
    print_file_type_error();
    return 0;
  }

  remove_comment_from_file(input);

  fscanf(input, "%d", &width);
  fscanf(input, "%d", &height);

  int ***image = set_image(input, width, height);

  print_matrix_in_file(argv[2], image, width, height);

  fclose (input);
  return 0;
}

void remove_comment_from_file(FILE * file) {
  int c;

  c = getc(file);
  if(c == '\n') c = getc(file);
  while (c == '#') {
    while (getc(file) != '\n');
    c = getc(file);
  }

  ungetc(c, file);
}

int*** set_image(FILE * input, int width, int height) {
  int ***image;
  image = malloc(sizeof(int**) * width);

  for (size_t i = 0; i < width; i++) {
    image[i] = malloc(sizeof(int*) * height);
    for (size_t j = 0; j < height; j++) {
      image[i][j] = malloc(sizeof(int) * 3);
    }
  }

  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      fscanf(input, "%d", &image[i][j][0]);
      fscanf(input, "%d", &image[i][j][1]);
      fscanf(input, "%d", &image[i][j][2]);
    }
  }

  return image;
}
