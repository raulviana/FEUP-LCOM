#include <lcom/lcf.h>
#include "object.h"
#include "graphics.h"
#include "i8042.h"
#include "keyboard.h"
#include "time.h"
#include "game.h"
#include "mouse.h"
#include "math.h"
//*******interrupt variables****************
  
static uint8_t kbc_bit; /**< @brief Bit do KBC*/
static uint8_t time_bit; /**< @brief Bit do Timer*/
static uint8_t mouse_bit; /**< @brief Bit do Mouse*/
static struct packet pp; /**< @brief packet do mouse*/
static uint8_t *scanCode = NULL; /**< @brief ScanCode recebido do KBC*/
uint8_t scanByte; 
static Keys key;
static int ipc_status;
static message msg; 
static int r; /**< @brief Recebe o resultado de driver_receive*/
//******************************************
static bool mouse_packet_ready = false;
unsigned int counter;
static Object *punctuation[MAX_LENGTH_PUNCTUATION];

void game(){
  //***********Init interrupts*************
  set_stream();
  enable_data();
  mouse_subscribe_int(&mouse_bit);
  kbc_subscribe_int(&kbc_bit);
  timer_subscribe_int(&time_bit);  
  //**************************************
  
  menu();

  //**********End intterupts***********
  mouse_unsubscribe_int();
  disable_data();
  kbc_unsubscribe_int();
  timer_unsubscribe_int();
}

/**
 * Função para gerir a velocidade da nave no jogo
 * @param ship - Objeto "Nave" em questão
 * @param key - Botão do teclado pressionado
 */
void process_ship_speed(Object *ship, Keys key){
  if(key.A && (abs(ship->speedX - 1) < MAX_SHIP_SPEED) ) ship->speedX--;
  if(key.D && (abs(ship->speedX + 1) < MAX_SHIP_SPEED )) ship->speedX++;
  if(key.S && (abs(ship->speedY + 1) < MAX_SHIP_SPEED )) ship->speedY++;
  if(key.W && (abs(ship->speedY - 1) < MAX_SHIP_SPEED )) ship->speedY--;
  //ship cant sip out of screen
  if((ship->posX + ship->speedX) < 0 ||      
      (ship->posX + ship->speedX) > (h_res - ship->width));
  else{
      ship->posX += ship->speedX;
      ship->centerX += ship->speedX;
  }
  if((ship->posY + ship->speedY) < 0 ||
      (ship->posY + ship->speedY) > (v_res - ship->height));
  else{
        ship->posY += ship->speedY;
        ship->centerY += ship->speedY;
  }
}

/**
 * Função para verificar as colisões
 * @param xpm - Objeto "Nave" em questão
 * @param array - Botão do teclado pressionado
 * @param type - Determina o tipo de objeto a colidir: Meteoro ou Laser
 * @param index - Indica o index
 * @returns 1 for colision and 0 for no colision.
 */
int check_collisions(Object *xpm, Object *array[], int type, uint8_t *index){
     unsigned int size;
     switch(type){
       case METEOR:
        size = MAX_METEORS;
        break;
       case LASER:
        size = MAX_LASER;
        break;
     }
     
     for(unsigned int i = 0; i < size; i++){
      if(array[i]->active){
        if(xpm->posX == array[i]->posX && xpm->posY == array[i]->posY && xpm->angle == array[i]->angle) return 0; // trata-se do mesmo meteoro
          if(((xpm->posX < (array[i]->posX + array[i]->width) &&               //XX box
               xpm->posX > array[i]->posX) || 
             ((xpm->posX + xpm->width) < (array[i]->posX + array[i]->width) && 
              (xpm->posX + xpm->width) > array[i]->posX)) 
            &&
             ((xpm->posY < (array[i]->posY + array[i]->height) &&                //YY box
               xpm->posY > array[i]->posY) ||
             ((xpm->posY + xpm->height) < (array[i]->posY + array[i]->height) &&
              (xpm->posY + xpm->height) > array[i]->posY))){
                *index = i;
                array[i]->active = false;
                return 1;
          }
        }
      }
    return 0;
}

	
/**
 * Função para reposicionar uma determinada explosão
 * @param explosion - Objeto "Explosão" em questão
 */
void repositionExplosion(Object *explosion){
  explosion->posX -= explosion->width/2;
  explosion->posY -= explosion->height/2;
}


void menu() {
  
  unsigned int temporizador = 0;
  bool finish = false;
  uint8_t score = 0;
  uint8_t max_score = 0;
  uint8_t units = 0;
  uint8_t dozens = 0;
  uint8_t hundreds = 0;
  uint16_t pointsX = MENU_POINTS_X;
  uint16_t pointsY = MENU_POINTS_Y;
  //Init mouse pointer
  Mouse_pointer mouse_pointer;
  mouse_pointer.posX = ceil(h_res / 2);
  mouse_pointer.posY = ceil((v_res / 2) + (v_res / 4));
  mouse_pointer.pressed_LB = false;
  mouse_pointer.mouse_packet_ready = false;
  //******MENU OBJECTS**************
  Object *menu;
  Object *pointer;
  Object *startButton;
  Object *exitButton;
  menu = createObject(menu_xpm, MENU);
  pointer = createObject(mouse2_xpm, MOUSE);
  startButton = createObject(startbutton_xpm, STARTBTN);
  exitButton = createObject(exitbutton_xpm, EXITBTN);
  Object *score_text = createObject(score_title_xpm, SCORE);
  createPunctuation(punctuation);
 
  //********************************************
  
  while (! finish){
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
             if (msg.m_notify.interrupts & BIT(kbc_bit)) { /*KEYBOARD Interrupt*/
              scanCode = kbcIH();
              switch(*scanCode){
                case ESC_BREAK:
                  key.ESC = true;
                  finish = true;
                  break;
              }
             }
             if(msg.m_notify.interrupts & BIT(time_bit)){ //TIMER Interrupt
              timer_int_handler();
              if(counter == sys_hz()){
                 counter = 0;
                 temporizador++;   //temporizador em segundos
              }      
             }
             if (msg.m_notify.interrupts & BIT(mouse_bit)) { /*MOUSE interrupt*/
                 mouseIH(&pp, &mouse_packet_ready);
                 mouse_pointer.mouse_packet_ready = mouse_packet_ready;            
             }
          break;
       default:
          break; /*no other notifications expected: do nothing*/
                }
      }
      else { /*received a standard message, not a notification*/
                /*no standard messages expected: do nothing*/ 
      }
    if (counter % 5 == 0) {
      //process mouse 
        if(mouse_pointer.mouse_packet_ready == true && !pp.x_ov && !pp.y_ov ){
          if(((mouse_pointer.posX + (MOUSE_SENSIVITY * pp.delta_x) < 0)) ||
             ((mouse_pointer.posX + (MOUSE_SENSIVITY * pp.delta_x) > (h_res - pointer->width))));
          else {
            mouse_pointer.posX += (MOUSE_SENSIVITY * pp.delta_x);
          }
          if(((mouse_pointer.posY + (MOUSE_SENSIVITY * pp.delta_y)) < 270) ||
             ((mouse_pointer.posY + (MOUSE_SENSIVITY * pp.delta_y) > (v_res - pointer->height))));
          else{
            mouse_pointer.posY += (MOUSE_SENSIVITY * pp.delta_y);
          } 
        }  
        mouse_pointer.mouse_packet_ready = false;
        if(pp.lb){
          mouse_pointer.pressed_LB = true; 
          pp.lb = false;
        }
        else mouse_pointer.pressed_LB = false;
       
        pointer->posX = mouse_pointer.posX;
        pointer->posY = mouse_pointer.posY;
        //Determinar se o Mouse Pointer está sobre o botão Start ou Exit
       
        if (pointer->posY >= startButton->posY && pointer->posY <= (startButton->posY + startButton->height)) {
          if (pointer->posX >= startButton->posX && pointer->posX <= (startButton->posX + startButton->width)) {
            if (mouse_pointer.pressed_LB){
              score = play();
              if(score >= max_score) max_score = score;
              mouse_pointer.posX = ceil(h_res / 2);
              mouse_pointer.posY = ceil((v_res / 2) + (v_res / 4));
            }
          }
          else if (pointer->posX >= exitButton->posX && pointer->posX <= (exitButton->posX + exitButton->width))  {
             if(mouse_pointer.pressed_LB) finish = true;
          } 
        } 
      mouse_pointer.mouse_packet_ready = false;
      
      //DRAW
      clear_screen();
      drawMenu(menu);
      drawMenu(startButton);
      drawMenu(exitButton);
      drawMenu(pointer);
      //     // draw points
      uint8_t temp_points = max_score;
      units = temp_points % 10;
      temp_points /= 10;
      dozens = temp_points % 10;
      temp_points /= 10;
      hundreds = temp_points % 10;
      punctuation[hundreds]->posX = pointsX;
      punctuation[hundreds]->posY = pointsY;
      draw_xpm(punctuation[hundreds]);
      punctuation[dozens]->posX = punctuation[hundreds]->posX + punctuation[hundreds]->width;
      punctuation[dozens]->posY = pointsY;
      draw_xpm(punctuation[dozens]);
      punctuation[units]->posX = punctuation[dozens]->posX + punctuation[dozens]->width;
      punctuation[units]->posY = pointsY;
      draw_xpm(punctuation[units]);

      draw_xpm(score_text);

      copyBuffToRam();

      counter++;
    }
  }
  free(punctuation);
}

uint8_t play(){
  uint8_t dificulty = INIT_DIFICULTY;
  bool game_over = false;
  uint8_t index = 0;
  uint8_t points = 0;
  uint8_t units = 0;
  uint8_t dozens = 0;
  uint8_t hundreds = 0;
  uint8_t pointsX = POINTS_X; //posicao da pontuacao
  uint8_t pointsY = POINTS_Y;
  
  //Init mouse pointer
  Mouse_pointer mouse_pointer;
  mouse_pointer.posX = ceil(h_res / 2);
  mouse_pointer.posY = ceil(v_res / 3);
  mouse_pointer.pressed_LB = false;
  mouse_pointer.mouse_packet_ready = false;

 
  
  unsigned int temporizador = 0;
  unsigned int meteor_timer = 0;
  unsigned int laser_timer = 0;
  unsigned int explosion_timer = 0;
     
 
  /**********Init Objects****************/
  uint8_t num_active_meteor = 0;
  uint8_t num_active_laser = 0;
  uint8_t num_active_stars = MAX_STARS;
  Object *stars[MAX_STARS];

  Object *crosshair;
  crosshair = createObject(crosshair_xpm, MOUSE);
  
  for(unsigned int i = 0; i < MAX_STARS; i++){
    memset(&stars[i], 0, sizeof(stars[i]));
    stars[i] = createObject( star_xpm, STAR);
  }

  Object *meteors[MAX_METEORS];
  
  for(unsigned int i = 0; i < MAX_METEORS; i++){
    memset(&meteors[i], 0, sizeof(meteors[i]));
    meteors[i] = createObject( asteroid2_xpm, METEOR);
  }

  Object *laser[MAX_LASER];
  
  for (unsigned int i = 0; i < MAX_LASER; i++){
    memset(&laser[i], 0, sizeof(laser[i]));
    laser[i] = createObject(laser_xpm, LASER);
  }
  
  Object *ship = createObject(ship_xpm, SHIP);


  Object *explosion[MAX_EXPLOSIONS];

  for(unsigned int i = 0; i< MAX_EXPLOSIONS; i++){
    memset(&explosion[i], 0, sizeof(explosion[i]));
    explosion[i] = createObject(explosion2_xpm, EXPLOSION);
  }
  
  /********************************************/
 //***************GameLoop*************************
    while (! game_over){
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
             if (msg.m_notify.interrupts & BIT(kbc_bit)) { /*KEYBOARD Interrupt*/
              scanCode = kbcIH();
              switch(*scanCode){
                case A_M_CODE:
                  key.A = true;
                  break;
                case A_B_CODE:
                  key.A = false;
                  break;
                case S_M_CODE:
                  key.S = true;
                  break;
                case S_B_CODE:
                  key.S = false;
                  break;
                case D_M_CODE:
                  key.D = true;
                  break;
                case D_B_CODE:
                  key.D = false;
                  break;
                case W_M_CODE:
                  key.W = true;
                  break;
                case W_B_CODE:
                  key.W = false;
                  break;
                case ESC_BREAK:
                  key.ESC = true;
                  game_over = true;
                  break;
              
              }
             }
             if(msg.m_notify.interrupts & BIT(time_bit)){ //TIMER Interrupt
              timer_int_handler();
              laser_timer++;
              explosion_timer++;
              if(counter == sys_hz()){
                 counter = 0;
                 meteor_timer++;
                 temporizador++;   //temporizador em segundos
              }      
             }
             if (msg.m_notify.interrupts & BIT(mouse_bit)) { /*MOUSE interrupt*/
                 mouseIH(&pp, &mouse_packet_ready);
                 mouse_pointer.mouse_packet_ready = mouse_packet_ready;            
             }
          break;
       default:
          break; /*no other notifications expected: do nothing*/
                }
      }
      else { /*received a standard message, not a notification*/
                /*no standard messages expected: do nothing*/ 
       
 
      }
    
      //***********Update Keys && check collisions******************************
           
      
       if(counter % 5 == 3){  // ~16ms, mas alternado com o update do display
        
        
        
        //COLLISIONS
        //ship
        if(check_collisions(ship, meteors, METEOR, &index)){
          for(unsigned int i = 0; i < MAX_EXPLOSIONS; i++){
            if(! explosion[i]->active){
              explosion[i]->posX = round((ship->centerX + meteors[index]->posX)/2);
              explosion[i]->posY = round((ship->centerY + meteors[index]->posY)/2);
              repositionExplosion(explosion[i]);
              explosion[i]->active = true;
              draw_xpm(explosion[i]);
              game_over = true;
            }
          }
        } 
        //laser
        for(unsigned int i = 0; i < MAX_LASER; i++){
          if(laser[i]->active){
            if(check_collisions(laser[i], meteors, LASER, &index)){
              laser[i]->active = false;
              meteors[index]->active = false;
              num_active_meteor--;
              num_active_laser--;
              points += HIT;
              for(unsigned int i = 0; i < MAX_EXPLOSIONS; i++){
                if(! explosion[i]->active){
                explosion[i]->posX = round((laser[i]->centerX + meteors[index]->posX)/2);
                explosion[i]->posY = round((laser[i]->centerY + meteors[index]->posY)/2);
                repositionExplosion(explosion[i]);
                explosion[i]->active = TRUE;
                explosion[i]->countDown = EXPLOSION_TIME;
                break;
                }
              }
            }
          }
        } 
        //meteors
        for(unsigned int i = 0; i < MAX_METEORS; i++){
          if(meteors[i]->active){
            if(check_collisions(meteors[i], meteors, METEOR, &index)){
              meteors[i]->active = false;
              meteors[index]->active = false;
              num_active_meteor--,
              num_active_meteor--;
              for(unsigned int j = 0; j < MAX_EXPLOSIONS; j++){
                if(! explosion[j]->active){
                explosion[j]->posX = round((meteors[i]->posX + meteors[index]->posX)/2);
                explosion[j]->posY = round((meteors[i]->posY + meteors[index]->posY)/2);
                repositionExplosion(explosion[j]);
                explosion[j]->active = TRUE;
                explosion[j]->countDown = EXPLOSION_TIME;
                break;
                }
              }
              
            }
          }
        } 
        //*********************************************************


        if(num_active_laser + 1 < MAX_LASER){
          for(unsigned int i = 0; i < MAX_LASER; i++){
            if(! laser[i]->active && mouse_pointer.pressed_LB && (laser_timer > 30)){//no máximo dispara dois lasers por segundo
                            
              laser[i]->posX = ship->centerX - laser[i]->width/2;
              laser[i]->posY = ship->centerY - laser[i]->height/2;
              laser[i]->centerX = laser[i]->posX + laser[i]->width/2;
              laser[i]->centerY = laser[i]->posY + laser[i]->height/2;
              laser[i]->speedX = -1 * MAX_LASER_SPEED * sin(ship->angle);
              laser[i]->speedY = -1 * MAX_LASER_SPEED * cos(ship->angle);
              laser[i]->angle = ship->angle;
              num_active_laser++;
              laser[i]->active = true;
              laser_timer = 0;
              break;
            }
          }
        }
        
        for(unsigned int i = 0; i < MAX_STARS; i++){ //update stars speed
          if(stars[i]->active){
            stars[i]->speedX = -2 * ship->speedX;
            stars[i]->speedY = -2 * ship->speedY;
          }
          if(num_active_stars < MAX_STARS){
            if(! stars[i]->active){
              stars[i] = createObject(star_xpm, STAR);
              num_active_stars++;
              stars[i]->active = true;
            }
          }
        }
        
        counter++;
       }
      //******************************************************
     //***********Update display*****************************
      
      copyBuffToRam();
      
      if(temporizador > 15 && dificulty < MAX_METEORS){
         dificulty++; //de 15 em 15 segundos dificuldade aumenta
         temporizador = 0;
      }
      if(num_active_meteor < dificulty && meteor_timer > METEOR_CADENCE){ //atualiza a cada METEOR_CADENCE
        uint8_t count = dificulty;
        while(count > 0){
          for(unsigned int i = 0; i < MAX_METEORS; i++){
          if(! meteors[i]->active){
            meteors[i] = createObject(asteroid2_xpm, METEOR);
            num_active_meteor++;
            meteors[i]->active = true;
            break;
          }
          meteor_timer = 0; 
          }
          count--;
        }
      }


      if(counter % 5 == 0){   // ~16 ms
       
        //process mouse 
        if(mouse_pointer.mouse_packet_ready == true && !pp.x_ov && !pp.y_ov ){
          if(((mouse_pointer.posX + (MOUSE_SENSIVITY * pp.delta_x) < 0)) ||
             ((mouse_pointer.posX + (MOUSE_SENSIVITY * pp.delta_x) > (h_res - meteors[0]->width))));
          else mouse_pointer.posX += (MOUSE_SENSIVITY * pp.delta_x);
          
          if(((mouse_pointer.posY + (MOUSE_SENSIVITY * pp.delta_y)) < 0) ||
             ((mouse_pointer.posY + (MOUSE_SENSIVITY * pp.delta_y) > (v_res - meteors[0]->height))));
          else mouse_pointer.posY += (MOUSE_SENSIVITY * pp.delta_y);
        }  
        mouse_pointer.mouse_packet_ready = false;
        if(pp.lb){
          mouse_pointer.pressed_LB = true; 
          pp.lb = false;
        }
        else mouse_pointer.pressed_LB = false;

        crosshair->posX = mouse_pointer.posX;
        crosshair->posY = mouse_pointer.posY;
       
        /***********DRAW******************************/
    
        clear_screen();
        drawMenu(crosshair);

        for(unsigned int i = 0; i < MAX_STARS; i++){ //draw active stars
          if(stars[i]->active == true){
              moveXpm(stars[i]);
            if(draw_xpm(stars[i]) != OK){
              stars[i]->active = false;
              num_active_stars--;
            }
          }
        }
        for(unsigned int i = 0; i < MAX_METEORS; i++){//draw active meteors
          if(meteors[i]->active == true){
              moveXpm(meteors[i]);
              meteors[i]->angle += 0.1;
            if(draw_xpm(meteors[i]) != OK){
              meteors[i]->active = false;
              num_active_meteor--;
            }
          }
        }
      
      for(unsigned int i = 0; i < MAX_LASER; i++){//draw active lasers
          if(laser[i]->active == true){
              moveXpm(laser[i]);
            if(draw_xpm(laser[i]) != OK){
              laser[i]->active = false;
              num_active_laser--;
            }
          }
      }
      process_ship_speed(ship, key);
      //update angle: angle between the two vectors, ship current position in vertical and current mouse pointer
      updateAngle(ship, mouse_pointer);
      draw_xpm(ship);
      //draw explosions
      for(unsigned int i = 0; i < MAX_EXPLOSIONS; i++){
        if(explosion[i]->active){
          draw_xpm(explosion[i]);
        }
        if(explosion[i]->countDown != 0){
          explosion[i]->countDown--;
        }
        else explosion[i]->active = false;
      }
      
      // draw points
      uint8_t temp_points = points;
      units = temp_points % 10;
      temp_points /= 10;
      dozens = temp_points % 10;
      temp_points /= 10;
      hundreds = temp_points % 10;
      punctuation[hundreds]->posX = pointsX;
      punctuation[hundreds]->posY = pointsY;
      draw_xpm(punctuation[hundreds]);
      punctuation[dozens]->posX = punctuation[hundreds]->posX + punctuation[hundreds]->width;
      punctuation[dozens]->posY = pointsY;
      draw_xpm(punctuation[dozens]);
      punctuation[units]->posX = punctuation[dozens]->posX + punctuation[dozens]->width;
      punctuation[units]->posY = pointsY;
      draw_xpm(punctuation[units]);
  
      
      counter++;
      }
       /****************************************/
     //*****************************************************
    }
  //**********************************************************
  for(unsigned int i = 0; i < MAX_METEORS; i++){
  free(meteors[i]);
  destroy_Object(meteors[i]);
  }
  for(unsigned int i = 0; i < MAX_STARS; i++){
  destroy_Object(stars[i]);
  }
  for (unsigned int i = 0; i < MAX_LASER; i++){
    destroy_Object(laser[i]);
    free(laser[i]);
  }
  for(unsigned int i = 0; i < MAX_EXPLOSIONS; i++){
    destroy_Object(explosion[i]);
    free(explosion[i]);
  }
  destroy_Object(ship);
  free(scanCode);
  scanCode = NULL;
  sleep(1);
  return points;
}



