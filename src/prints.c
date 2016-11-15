#include <stdio.h>
#include "prints.h"

void print_usage_error() {
  printf("\x1b[31m" "usage: altPhisics ");
  printf("<input_file> ");
  printf("<output_file> ");
  printf("<number of interations> ");
  printf("<number of processors>\n");
}

void print_file_error(char * file) {
  printf("\x1b[31m" "file <%s> not found\n", file);
}
void print_number_error(char * n) {
  printf("\x1b[31m" "invalid number <%s>\n", n);
}

void print_file_type_error() {
  printf("\x1b[31m" "I can only read ppm type P3\n");
}

void print_matrix_in_file(char * file_name, int *** matrix, int width, int height) {
  FILE *output;
  output = fopen (file_name ,"w");

  fprintf(output, "P3\n");
  fprintf(output, "%d %d\n", width, height);

    for (size_t i = 0; i < width; i++) {
      for (size_t j = 0; j < height; j++) {
        for (size_t k = 0; k < 3; k++) {
          fprintf(output, "%3d ", matrix[i][j][k] );
        }
        fprintf(output, "  ");
      }
      fprintf(output, "\n");
    }

    fclose(output);
}
