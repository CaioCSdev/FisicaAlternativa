#include <stdio.h>
#include "prints.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void print_usage_error() {
  printf(RED "usage: altPhisics ");
  printf("<input_file> ");
  printf("<output_file> ");
  printf("<number of interations> ");
  printf("<number of processors>\n" RESET);
}

void print_file_error(char * file) {
  printf(RED "file <%s> not found\n" RESET, file);
}
void print_number_error(char * n) {
  printf(RED "invalid number <%s>\n" RESET, n);
}

void print_file_type_error() {
  printf(RED "I can only read ppm type P3\n" RESET);
}

void print_matrix_in_file(char * file_name, int *** matrix, int width, int height) {
  FILE *output;
  output = fopen (file_name ,"w");

  fprintf(output, "P3\n");
  fprintf(output, "%d %d\n", width, height);
  fprintf(output, "255\n");

    for (size_t i = 0; i < width; i++) {
      for (size_t j = 0; j < height; j++) {
        for (size_t k = 0; k < 3; k++) {
          fprintf(output, "%3d ", matrix[i][j][k] );
          if (matrix[i][j][k] < 0) printf("%d %d %d eh <0\n",(int)i,(int)j,(int)k);
        }
        fprintf(output, "  ");
      }
      fprintf(output, "\n");
    }

    fclose(output);
}
