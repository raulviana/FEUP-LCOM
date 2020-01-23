#include <lcom/lcf.h>

#include <math.h>
#include "object.h"
#include "graphics.h"



Object *createObject( xpm_row_t xpm[], int type){

  Object *obj = (Object *) malloc(sizeof(Object)); 

  if (obj == NULL) return NULL;

  enum xpm_image_type xpmType = EIGHT_BYTES_TYPE; 
  xpm_image_t img;
  memset(&img, 0, sizeof(xpm_image_t)); 
  xpm_load(xpm, xpmType, &img);

  obj->map = img.bytes;
  obj->height = img.height;
  obj->width = img.width;

  uint8_t fixedXY = rand() % 4;

  switch(type){
    
    case METEOR:
        //Initial Position
      
        switch(fixedXY){
          case(0):          
            obj->posX = 0;
            obj->posY = rand() % (v_res);
            obj->speedX = (rand() % MAX_SPEED_X);
            obj->speedY = (rand() % MAX_SPEED_Y) - (ceil(MAX_SPEED_Y / 2));
            if(obj->speedX == 0 && obj->speedY == 0) obj->speedX = 3;
            break;
          case(1): 
            obj->posX = h_res - (obj->width);
            obj->posY = rand() % (v_res);
            obj->speedX = (rand() % MAX_SPEED_X) - MAX_SPEED_X;
            obj->speedY = (rand() % MAX_SPEED_Y) - (ceil(MAX_SPEED_Y / 2));
            if(obj->speedX == 0 && obj->speedY == 0) obj->speedX = -3;
            break;
          case(2):
            obj->posX = rand() % (h_res);
            obj->posY = 0;
            obj->speedX = (rand() % MAX_SPEED_X) - (ceil(MAX_SPEED_X / 2));
            obj->speedY = (rand() % MAX_SPEED_Y);
            if(obj->speedX == 0 && obj->speedY == 0) obj->speedY = 3;
            break;
          case(3):
            obj->posX = rand() % (h_res);
            obj->posY = v_res - (obj->height);
            obj->speedX = (rand() % MAX_SPEED_X) - (ceil(MAX_SPEED_X / 2));
            obj->speedY = (rand() % MAX_SPEED_Y) - MAX_SPEED_Y;
            if(obj->speedX == 0 && obj->speedY == 0) obj->speedX = -3;
            break;
        }
        obj->active = false;
        int tmp = (rand() % 60) - 30; 
        obj->angle = (double) tmp / 10; 
        break;

    case SHIP:
        obj->posX = round(h_res / 2);
        obj->posY = round(v_res / 2);
        obj->speedX = 0;
        obj->speedY = 0;
        obj->active = false;
        break;
      
    case STAR:
        obj->posX = rand() % (h_res - obj->width);
        obj->posY = rand() % (v_res - obj->height);
        obj->speedX = 0;
        obj->speedY = 0;
        obj->active = true;
        obj->angle= 0.0;
        break;
    
    case MENU:
      obj->posX = 170;
      obj->posY = 40;
      obj->speedX = 0;
      obj->speedY = 0;
      break;

    case MOUSE:
      obj->posX = h_res/2;
      obj->posY = (v_res/2) + (v_res/4);
      obj->speedX = 0;
      obj->speedY = 0;
      break;

    case STARTBTN:
      obj->posX = 150;
      obj->posY = 400;
      obj->speedX = 0;
      obj->speedY = 0;
      break;

    case EXITBTN:
      obj->posX = 450;
      obj->posY = 400;
      obj->speedX = 0;
      obj->speedY = 0;
      break;

    case LASER:
      obj->posX = 450;
      obj->posY = 400;
      obj->speedX = 0;
      obj->speedY = 0;
      obj->angle= 0.0;
      obj->active = false;
      break;

    case EXPLOSION:
      obj->posX = 0;
      obj->posY = 0;
      obj->angle= 0.0;
      obj->countDown = EXPLOSION_TIME;
      obj->active = false;
      break;
    
    case SCORE:
      obj->speedX = 0;
      obj->speedY = 0;
      obj->angle= 0.0;
      obj->posX = 230;
      obj->posY = 308;
      break;
  }

  obj->centerX = obj->posX + round(obj->width/2);
  obj->centerY = obj->posY + round(obj->height/2);


  return obj;
}

void destroy_Object(Object *obj) {
  if (obj == NULL) return;

  if (obj->map) free(obj->map);

  free(obj);
  obj = NULL; 
}


void moveXpm(Object *obj){
  obj->posX += obj->speedX;
  obj->centerX += obj->speedX;
  obj->posY += obj->speedY;
  obj->centerY += obj->speedY;
}

uint16_t rotateX(double angle, int x, int y, uint16_t width, uint16_t heigth, int centerX, int centerY, double cos_value, double sin_value){
 
  y -= (centerY);
  x -= (centerX);

  float res = x * cos_value + (y * sin_value);
  
  res += (double) (centerX);
 
  return (uint16_t) round(res);
}

uint16_t rotateY(double angle, int x, int y, uint16_t width, uint16_t heigth, int centerX, int centerY, double cos_value, double sin_value){

  x -= (centerX ) ;
  y -= (centerY );

  float res = (x * (-1) * sin_value + (y * cos_value));
 
  res += (double) (centerY );

  return (uint16_t) round(res);
}



void updateAngle(Object *ship, Mouse_pointer mouse_pointer){
  //find mouse pointer vector
  double vec_mouse_X = (double) (ship->posX - mouse_pointer.posX);
  double vec_mouse_Y = (double) (ship->posY - mouse_pointer.posY);
  double vec_ship_X = (double) SHIP_VEC_X;
  double vec_ship_Y = (double) SHIP_VEC_Y;

  
  // (a*b) / ( ||a|| * ||b||)
  double axb = (vec_ship_X * vec_mouse_X) + (vec_mouse_Y * vec_ship_Y);
  double norms = sqrt( (vec_ship_X * vec_ship_X) + (vec_ship_Y * vec_ship_Y)) *
                 sqrt((vec_mouse_X * vec_mouse_X) + (vec_mouse_Y * vec_mouse_Y));
  ship->angle = acos(axb / norms);
  if(vec_ship_X > vec_mouse_X) ship->angle *= -1;  //acos(angle) varies [0, 3.14]
}

void createPunctuation(Object *punctuation[]){
  xpm_row_t *numbers[MAX_LENGTH_PUNCTUATION];
  numbers[0] = letterZero_xpm;
  numbers[1] = letter1_xpm;
  numbers[2] = letter2_xpm;
  numbers[3] = letter3_xpm;
  numbers[4] = letter4_xpm;
  numbers[5] = letter5_xpm;
  numbers[6] = letter6_xpm;
  numbers[7] = letter7_xpm;
  numbers[8] = letter8_xpm;
  numbers[9] = letter9_xpm;

  
  for ( unsigned int i = 0; i < MAX_LENGTH_PUNCTUATION; i++){
    
    Object *obj = (Object *) malloc(sizeof(Object)); 


    enum xpm_image_type xpmType = EIGHT_BYTES_TYPE; 
    xpm_image_t img;
    memset(&img, 0, sizeof(xpm_image_t)); 
    xpm_load(numbers[i], xpmType, &img);

    obj->map = img.bytes;
    obj->height = img.height;
    obj->width = img.width;
    
    obj->active = false;

    punctuation[i] = obj;
  }
}

