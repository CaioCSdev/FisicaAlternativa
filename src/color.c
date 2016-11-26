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

  int i, j, x, y, color, spread_number, aux;
  double intensity[3], green_angle, angle, red_component[2], blue_component[2], spread_value;
  int ***new_image;

/*New Image allocation*/
  new_image = malloc(width * sizeof(int**));
  for (i = 0; i < width; i++) {
    new_image[i] = malloc(height * sizeof(int*));
    for (j = 0; j < height; j++) {
      new_image[i][j] = malloc(3 * sizeof(int));
      new_image[i][j][RED] = image[i][j][RED];
      new_image[i][j][GREEN] = image[i][j][GREEN];
      new_image[i][j][BLUE] = image[i][j][BLUE];
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
      angle = (PI / 2) - green_angle;

      printf("%d %d : green angle is %f PI, angle is %f PI, test = %f\n",i,j,green_angle/PI, angle/PI, sin(PI/2));

/* Red and blue values to be distributed. X component is size * cos, Y component is size * sin */
/* color_component[0] is X, 1 is Y */
      red_component[Y] = intensity[RED] * sin(angle);
      red_component[X] = intensity[RED] * cos(angle);

      blue_component[Y] = intensity[BLUE] * sin(angle + PI);
      blue_component[X] = intensity[BLUE] * cos(angle + PI);

      printf("red component Y %f and X %f\n", red_component[Y], red_component[X]);
      printf("blue component Y %f and X %f\n", blue_component[Y], blue_component[X]);

/*What neighbours to visit: */
/* ie. If X component > 0, x + 1 neighbour, x -1 otherwise */
      if (red_component[X] > 0) x = 1;
      else x = -1;
     if (red_component[Y] > 0) y = 1;
      else y = -1;

      printf("x is %d, and y is %d\n",x,y);

/*Calculating new values */
      if (red_component[X] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][RED] += received_value(red_component[X] * 255, image[i + x][j][RED]);

      if (red_component[Y] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][RED] += received_value(red_component[Y] * 255, image[i][j + y][RED]);

      if (blue_component[X] > 0) x = 1;
      else x = -1;
     if (blue_component[Y] > 0) y = 1;
      else y = -1;

      printf("x is %d, and y is %d\n",x,y);

      if (blue_component[X] != 0 && i + x >= 0 && i + x < width)
        new_image[i + x][j][BLUE] += received_value(blue_component[X] * 255, image[i + x][j][BLUE]);

      if (blue_component[Y] != 0 && j + y >= 0 && j + y < height)
        new_image[i][j + y][BLUE] += received_value(blue_component[Y] * 255, image[i][j + y][BLUE]);

    }
  }

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      for (color = 0; color < 4; color += 2){
        spread_value = 0;

        if (new_image[i][j][color] < 0) new_image[i][j][color] = 0;
        else if (new_image[i][j][color] > 255){ 
          spread_value = (1 - new_image[i][j][color]);
          new_image[i][j][color] = 255;
        }

        if (spread_value){
          spread_number = 0;
          if (i+1 < width) spread_number++;
          if (j + 1 < height) spread_number++;
          if (i-1 > 0) spread_number++;
          if (j - 1 > 0) spread_number++;

          aux = spread_value/spread_number;

          if (i+1 < width && new_image[i+1][j][color] + aux >=0 && new_image[i+1][j][color] + aux <= 255) new_image[i+1][j][color] += aux;
          if (j + 1 < height && new_image[i][j+1][color] + aux >=0 && new_image[i][j+1][color] + aux <= 255) new_image[i][j+1][color] += aux;
          if (i-1 >= 0 && new_image[i-1][j][color] + aux >=0 && new_image[i-1][j][color] + aux <= 255) new_image[i-1][j][color] += aux;
          if (j - 1 >= 0 && new_image[i][j-1][color] + aux >=0 && new_image[i][j-1][color] + aux <= 255) new_image[i][j-1][color] += aux;
        }
      }
    }
  }

  return new_image;  
}

double received_value (double self, double neighbour){
  double delta;

  delta = ((255 - neighbour) * self)/1000;

  printf("Received Value %f\n", delta);

  return delta;
}