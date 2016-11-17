#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "input.h"
#include "prints.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define PI 3.14159265359

int*** new_colors(int***, int, int );
double received_value(double, double);

int*** new_colors(int ***image, int height, int width) {

  int i, j, x, y;
  double intensity[3], green_angle, red_component[2], blue_component[2];
  int ***new_image;

/*New Image allocation*/
  new_image = malloc(sizeof(int**) * width);
  for (i = 0; i < width; i++) {
    new_image[i] = malloc(sizeof(int*) * height);
    for (j = 0; j < height; j++) {
      new_image[i][j] = malloc(sizeof(int) * 3);
    }
  }

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
/*RGB values on a [0,1] scale */
      intensity[RED] = ((double)image[i][j][RED] / 255.);
      intensity[GREEN] = ((double)image[i][j][GREEN] / 255.);
      intensity[BLUE] = ((double)image[i][j][BLUE] / 255.);

/*Green Angle */
      green_angle = (intensity[GREEN] * 2 * PI);

/* Red and blue values to be distributed. X component is size * cos, Y component is size * sin */
/* color_component[0] is X, 1 is Y */
      red_component[1] = intensity[RED] * sin(green_angle);
      red_component[0] = intensity[RED] * cos(green_angle);

      blue_component[1] = intensity[BLUE] * sin(green_angle);
      blue_component[0] = intensity[BLUE] * cos(green_angle);


/*What neighbours to visit: */
/* ie. If X component > 0, x + 1 neighbour, x -1 otherwise */
      if (red_component[0] > 0) x = 1;
      else x = -1;
     if (red_component[1] > 0) y = 1;
      else y = -1;

/*Calculating new values */
      if (red_component[0] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][RED] = image[i + x][j][RED] + received_value(image[i][j][RED], image[i + x][j][RED]);

      if (red_component[1] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][RED] = image[i][j + y][RED] + received_value(image[i][j][RED], image[i][j + y][RED]);

      if (blue_component[0] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][BLUE] = image[i + x][j][BLUE] + received_value(image[i][j][BLUE], image[i + x][j][BLUE]);

      if (blue_component[1] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][BLUE] = image[i][j + y][BLUE] + received_value(image[i][j][BLUE], image[i][j + y][BLUE]);

    }
  }

  return new_image;  
}

double received_value (double self, double neighbour){
  double delta;

  delta = ((1 - neighbour) * self)/4;

  return delta;
}