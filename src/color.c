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
int *** alloc_image (int, int, int ***);
double calculate_angle(double);
double *scale(int * );
void forces(double , int , double , int ***, int *** , int, int);

/*IMPORTANTE: As bordas sao consideradas fixas e nunca se alteram. */

int*** new_colors(int ***image, int height, int width) {

  int i, j, color, aux;
  double *intensity, green_angle, angle, spread_value, aux2;
  int ***new_image;

  new_image = alloc_image(width, height, image);

  for (i = 1; i < width-1; i++) {
    for (j = 1; j < height-1; j++) {
      intensity = scale(image[i][j]);
      angle = calculate_angle(intensity[GREEN]);
      forces(intensity[RED], RED, angle, new_image, image, i, j);
      forces(intensity[BLUE], BLUE, angle + PI, new_image, image, i, j);
    }
  }

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      for (color = 0; color < 4; color += 2){
        spread_value = 0;

        if (new_image[i][j][color] < 0) new_image[i][j][color] = 0;
        else if (new_image[i][j][color] > 255){
          spread_value = (255 - new_image[i][j][color]);
          new_image[i][j][color] = 255;
        }

        if (spread_value){
          aux = spread_value/4;

          if (new_image[i+1][j][color] + aux >=0 && new_image[i+1][j][color] + aux <= 255) new_image[i+1][j][color] += aux;
          if (new_image[i][j+1][color] + aux >=0 && new_image[i][j+1][color] + aux <= 255) new_image[i][j+1][color] += aux;
          if (new_image[i-1][j][color] + aux >=0 && new_image[i-1][j][color] + aux <= 255) new_image[i-1][j][color] += aux;
          if (new_image[i][j-1][color] + aux >=0 && new_image[i][j-1][color] + aux <= 255) new_image[i][j-1][color] += aux;

          if(new_image[i+1][j][color] < 0 || new_image[i-1][j][color] < 0 || new_image[i][j+1][color] < 0 || new_image[i][j-1][color] < 0)printf("ERRO\n");
        }
      }

      aux2 = sqrt(pow(((float)new_image[i][j][RED]/255.),2) + pow(((float)new_image[i][j][BLUE]/255.),2));
      aux2 = acos((float)new_image[i][j][BLUE] / (255*aux2));

      if (aux2 < 0 || aux2 > PI/2)printf("angulo = %f\n", aux2);

      intensity[GREEN] = ((double)image[i][j][GREEN] / 255.);
      green_angle = (intensity[GREEN] * 2 * PI);
      angle = (PI / 2) - green_angle;

      angle += aux2;

      green_angle = (PI/2) - angle;

      intensity[GREEN] = green_angle/(2*PI);

/*      printf("old %d new %f green\n",image[i][j][GREEN], intensity[GREEN] * 255);
*/
      if (intensity[GREEN] > 0 && intensity[GREEN] <= 1)new_image[i][j][GREEN] = intensity[GREEN] * 255;
      else if(intensity[GREEN] > 1) new_image[i][j][GREEN] = 1;
      else new_image[i][j][GREEN] = 0;
    }
  }

  return new_image;
}

double received_value (double self, double neighbour){
  double delta;

  delta = ((255 - neighbour) * self)/1000;

  return delta;
}

/*New Image allocation*/
int *** alloc_image (int width, int height, int ***image){
  int ***new_image = malloc(width * sizeof(int**));
  int i,j;

  for (i = 0; i < width; i++) {
    new_image[i] = malloc(height * sizeof(int*));
    for (j = 0; j < height; j++) {
      new_image[i][j] = malloc(3 * sizeof(int));
      new_image[i][j][RED] = image[i][j][RED];
      new_image[i][j][GREEN] = image[i][j][GREEN];
      new_image[i][j][BLUE] = image[i][j][BLUE];
    }
  }

  return new_image;
}

/*Green Angle : where 90 degrees is 0 in greenAngle, and 0 degrees is 90.*/
double calculate_angle(double g_intensity) {
  return (PI * 0.5) - (g_intensity * 2 * PI);
}

/*RGB values on a [0,1] scale */
double *scale(int * intensities) {
  double *scaled = malloc(3 * sizeof(double));

  for (size_t i = 0; i < 3; i++) {
    scaled[i] = ((double)intensities[i]/255);
  }

  return scaled;
}

void forces(double intensity, int color, double angle, int *** new_image, int *** image, int i, int j) {
  double projection[2];
  int x,y;

  projection[X] = intensity * cos(angle);
  projection[Y] = intensity * sin(angle);

  if (projection[X] > 0) x = 1;
  else x = -1;
  if ((projection[Y]) > 0) y = 1;
  else y = -1;

  if (projection[X])
    new_image[i + x][j][color] += received_value(projection[X] * 255, image[i + x][j][color]);

  if (projection[Y])
    new_image[i][j + y][color] += received_value(projection[Y] * 255, image[i][j + y][color]);

}
