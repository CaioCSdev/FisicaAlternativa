#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "input.h"
#include "prints.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define PI 3.14159265359
#define X 0
#define Y 1

int*** new_colors(int***, int, int );
double received_value(double, double);

int*** new_colors(int ***image, int height, int width) {

  int i, j, x, y, color, spread_number;
  double intensity[3], green_angle, red_component[2], blue_component[2], spread_value;
  int ***new_image;

/*New Image allocation*/
  new_image = calloc(width, sizeof(int**));
  for (i = 0; i < width; i++) {
    new_image[i] = calloc(height, sizeof(int*));
    for (j = 0; j < height; j++) {
      new_image[i][j] = calloc(3, sizeof(int));
    }
  }

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
/*RGB values on a [0,1] scale */
      intensity[RED] = ((double)image[i][j][RED] / 255.);
      intensity[GREEN] = ((double)image[i][j][GREEN] / 255.);
      intensity[BLUE] = ((double)image[i][j][BLUE] / 255.);

/*Green Angle : where 90 degrees is 0 in greenAngle, and 0 degrees is 90.*/
      green_angle = (intensity[GREEN] * 2 * PI);

/* Red and blue values to be distributed. X component is size * cos, Y component is size * sin */
/* color_component[0] is X, 1 is Y */
      red_component[Y] = intensity[RED] * sin(green_angle);
      red_component[X] = intensity[RED] * cos(green_angle);

      blue_component[Y] = intensity[BLUE] * sin(green_angle);
      blue_component[X] = intensity[BLUE] * cos(green_angle);


/*What neighbours to visit: */
/* ie. If X component > 0, x + 1 neighbour, x -1 otherwise */
      if (red_component[0] > 0) x = 1;
      else x = -1;
     if (red_component[1] > 0) y = 1;
      else y = -1;

/*Calculating new values */
      if (red_component[X] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][RED] += image[i + x][j][RED] + received_value(image[i][j][RED], image[i + x][j][RED]);

      if (red_component[Y] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][RED] += image[i][j + y][RED] + received_value(image[i][j][RED], image[i][j + y][RED]);

      if (blue_component[X] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][BLUE] += image[i + x][j][BLUE] + received_value(image[i][j][BLUE], image[i + x][j][BLUE]);

      if (blue_component[Y] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][BLUE] += image[i][j + y][BLUE] + received_value(image[i][j][BLUE], image[i][j + y][BLUE]);

    }
  }

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      for (color = 0; color < 4; color += 2){
        spread_value = 0;

        if (new_image[i][j][color] < 0) new_image[i][j][RED] = 0;
        else if (new_image[i][j][color] > 255){ 
          spread_value = (1 - new_image[i][j][RED]);
          new_image[i][j][color] = 255;
        }

        if (spread_value){
          spread_number = 0;
          if (i+1 < width) spread_number++;
          if (j + 1 < height) spread_number++;
          if (i-1 > 0) spread_number++;
          if (j - 1 > 0) spread_number++;

          if (i+1 < width) new_image[i+1][j][color] += spread_value/spread_number;
          if (j + 1 < height) new_image[i][j+1][color] += spread_value/spread_number;
          if (i-1 > 0) new_image[i-1][j][color] += spread_value/spread_number;
          if (j - 1 > 0) new_image[i][j-1][color] += spread_value/spread_number;
        }
      }
    }
  }

  return new_image;  
}

double received_value (double self, double neighbour){
  double delta;

  delta = ((1 - neighbour) * self)/4;

  return delta;
}