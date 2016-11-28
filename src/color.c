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


/*IMPORTANTE: As bordas sao consideradas fixas e nunca se alteram. */

int*** new_colors(int ***image, int height, int width) {

  int i, j, x, y, color, aux;
  double intensity[3], green_angle, angle, red_component[2], blue_component[2], spread_value, aux2;
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

  for (i = 1; i < width-1; i++) {
    for (j = 1; j < height-1; j++) {
/*RGB values on a [0,1] scale */
      intensity[RED] = ((double)image[i][j][RED] / 255.);
      intensity[GREEN] = ((double)image[i][j][GREEN] / 255.);
      intensity[BLUE] = ((double)image[i][j][BLUE] / 255.);

/*Green Angle : where 90 degrees is 0 in greenAngle, and 0 degrees is 90.*/
      green_angle = (intensity[GREEN] * 2 * PI);
      angle = (PI * 0.5) - green_angle;

/*      printf("%d %d : IMAGE = %d \ngreen angle is %f PI, angle is %f PI, test = %f\n",i,j,image[i][j][GREEN],green_angle/PI, angle/PI, sin(PI/2));*/

/* Red and blue values to be distributed. X component is size * cos, Y component is size * sin */
/* color_component[0] is X, 1 is Y */
      red_component[Y] = intensity[RED] * sin(angle);
      red_component[X] = intensity[RED] * cos(angle);

      blue_component[Y] = intensity[BLUE] * sin(angle + PI);
      blue_component[X] = intensity[BLUE] * cos(angle + PI);

/*      printf("red component X %f and Y %f\n", red_component[X], red_component[Y]);
      printf("blue component X %f and Y %f\n", blue_component[X], blue_component[Y]);*/

/*What neighbours to visit: */
/* ie. If X component > 0, x + 1 neighbour, x -1 otherwise */
      if ((int)(red_component[X] * 255) > 0) x = 1;
      else x = -1;
      if ((int)(red_component[Y] * 255) > 0) y = 1;
      else y = -1;

/*      printf("red: x is %d, and y is %d\n",x,y);*/

/*Calculating new values */
      if (red_component[X] != 0)
        new_image[i + x][j][RED] += received_value(red_component[X] * 255, image[i + x][j][RED]);

      if (red_component[Y] != 0)
        new_image[i][j + y][RED] += received_value(red_component[Y] * 255, image[i][j + y][RED]);

      if ((int)(blue_component[X] * 255) > 0) x = 1;
      else x = -1;
      if ((int)(blue_component[Y] * 255) > 0) y = 1;
      else y = -1;

/*      printf("blue: x is %d, and y is %d\n",x,y);*/

      if (blue_component[X] != 0)
        new_image[i + x][j][BLUE] += received_value(blue_component[X] * 255, image[i + x][j][BLUE]);

      if (blue_component[Y] != 0)
        new_image[i][j + y][BLUE] += received_value(blue_component[Y] * 255, image[i][j + y][BLUE]);

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

  /*printf("Received Value %f\n", delta);*/

  return delta;
}
