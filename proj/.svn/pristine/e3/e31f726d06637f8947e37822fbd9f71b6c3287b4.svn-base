#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>


/** @defgroup game game
 * @{
 *
 * Funções Principais para a execução do jogo.
 */

#define MOUSE_SENSIVITY 3 /**< @brief Valor da sensabilidade do mouse */
#define INIT_DIFICULTY  1 /**< @brief Valor da dificuldade do Jogo */
#define MAX_SHIP_SPEED  4 /**< @brief Valor da velocidade Maxima da Nave */

#define POINTS_X 15 /**< @brief Posição em X da pontuação */
#define POINTS_Y 5 /**< @brief Posição em Y da pontuação */
#define MENU_POINTS_X 400 /**< @brief Posição em X da Menu */
#define MENU_POINTS_Y 300 /**< @brief Posição em Y da Menu */
#define HIT      10 /**< @brief Acréscimo da pontuação por cada asteroide atingido */
 

/**
 * Struct do mouse para gerir o mesmo
 */
typedef struct mouse_pointer{
  int posX; /**< @brief posição x do mouse */
  int posY;/**< @brief posição y do mouse */
  uint8_t pressed_LB; /**< @brief Se o botão esquerdo está pressionado ou não */
  uint8_t mouse_packet_ready;/**< @brief Se o mouse_packet está pronto ou não */
}Mouse_pointer;


/**
 * Struct de teclas para verificar se certas teclas importantes estão pressionadas
 */
typedef struct key{
  bool A;/**< @brief Se a tecla "A" está pressionada ou não */
  bool S;/**< @brief Se a tecla "S" está pressionada ou não */
  bool D;/**< @brief Se a tecla "D" está pressionada ou não */
  bool W;/**< @brief Se a tecla "W" está pressionada ou não */
  bool ESC;/**< @brief Se a tecla "ESC" está pressionada ou não */
}Keys;

	
/**
 * Função Principal do jogo que gere os diferentes aspetos do jogo.
 */
void game();


/**
 * Função Menu gere os diferentes aspetos do menu.
 */
void menu();

/**
 * Função para gerir o "game loop".
 */
uint8_t play();



#endif /* GAME_H */

