#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"

#include "xpm/asteroid.xpm"
#include "xpm/asteroid2.xpm"
#include "xpm/ship.xpm"
#include "xpm/star.xpm"
#include "xpm/star2.xpm"
#include "xpm/laser.xpm"
#include "xpm/explosion.xpm"
#include "xpm/explosion2.xpm"
#include "xpm/alphabet/letter1.xpm"
#include "xpm/alphabet/letter2.xpm"
#include "xpm/alphabet/letter3.xpm"
#include "xpm/alphabet/letter4.xpm"
#include "xpm/alphabet/letter5.xpm"
#include "xpm/alphabet/letter6.xpm"
#include "xpm/alphabet/letter7.xpm"
#include "xpm/alphabet/letter8.xpm"
#include "xpm/alphabet/letter9.xpm"
#include "xpm/alphabet/letterZero.xpm"
#include "xpm/exitbutton.xpm"
#include "xpm/menu.xpm"
#include "xpm/mouse.xpm"
#include "xpm/mouse2.xpm"
#include "xpm/startbutton.xpm"
#include "xpm/alphabet/score_title.xpm"
#include "xpm/crosshair.xpm"

/** @defgroup object object
 * @{
 *
 * Funções para gerir os "objetos" do jogo
 */


#define PI         3.14 /**< @brief Valor de Pi*/ 
#define DEGREE     180 /**< @brief Valor máximo de graus*/ 

#define MAX_SPEED_X 15 /**< @brief Velocidade máxima da nave em X*/ 
#define MAX_SPEED_Y 15 /**< @brief Velocidade máxima da nave em Y*/ 
#define MAX_LASER_SPEED 20 /**< @brief Velocidade máxima do laser*/
#define LASER_CADENCE 1 /**< @brief Cadência do Laser*/
#define METEOR_CADENCE 3 /**< @brief Cadência do Meteoro*/
#define SHIP_VEC_X 0 /**< @brief Valor Orientador da Nave no vector X*/
#define SHIP_VEC_Y 10 /**< @brief Valor Orientador da Nave no vector Y*/


#define MAX_METEORS 20 /**< @brief Número máximo de Meteoros que podem existir num determinado momento*/
#define MAX_STARS   20 /**< @brief Número máximo de Estrelas que podem existir num determinado momento*/
#define MAX_LASER   5 /**< @brief Número máximo de "Lasers" que podem existir num determinado momento*/
#define MAX_EXPLOSIONS 20 /**< @brief Número máximo de Explosões que podem existir num determinado momento*/
#define EXPLOSION_TIME 10 /**< @brief Tempo máximo de uma Explosão*/
#define MAX_LENGTH_PUNCTUATION 10 /**< @brief Comprimento máximo da Pontuação*/
#define MAX_SCORE_LENGTH 8 /**< @brief Comprimento máximo da Pontuação*/

#define SHIP      0 /**< @brief ID do "Tipo" de Objeto "Nave"*/
#define METEOR    1 /**< @brief ID do "Tipo" de Objeto "Meteoro"*/
#define LASER     2 /**< @brief ID do "Tipo" de Objeto "Laser"*/
#define STAR      3 /**< @brief ID do "Tipo" de Objeto "Estrela"*/
#define MENU      4 /**< @brief ID do "Tipo" de Objeto "Menu"*/
#define MOUSE     5 /**< @brief ID do "Tipo" de Objeto "Mouse"*/
#define STARTBTN  6 /**< @brief ID do "Tipo" de Objeto "Botão Start"*/
#define EXITBTN   7 /**< @brief ID do "Tipo" de Objeto "Botão Exit"*/
#define EXPLOSION 8 /**< @brief ID do "Tipo" de Objeto "Explosão"*/
#define SCORE     9 /**< @brief ID do "Tipo" de Objeto "Pontuação"*/


#define EIGHT_BYTES_TYPE XPM_8_8_8 /**< @brief "Tipo" de XPM tendo em conta o modo de video utilizado e os seus "Bits per pixel"*/




extern uint16_t h_res;		/**< @brief Resolução horizontal do ecrã*/
extern uint16_t v_res; 		/**< @brief Resolução vertical do ecrã*/

/**
 * Struct do "Objeto" para gerir o mesmo
 */
typedef struct object{
    uint8_t *map; /**< @brief XPM map do objeto*/
    uint16_t width; /**< @brief Largura do objeto*/
    uint16_t height; /**< @brief Altura do objeto*/
    int posX; /**< @brief Posição X do objeto*/
    int posY; /**< @brief Posição Y do objeto*/
    int centerX; /**< @brief Posição X do centro do objeto*/
    int centerY; /**< @brief Posição Y do centro do objeto*/
    int speedX; /**< @brief Velocidade em X do objeto*/
    int speedY; /**< @brief Velocidade em Y do objeto*/
    double angle; /**< @brief Ângulo do objeto*/
    bool active; /**< @brief Variével que verifica se o objeto está ativo ou não.*/
    uint16_t countDown; /**< @brief Contagem decrescente (usado para gerir explosões)*/
}Object;


/**
 * Função para criar um objeto
 * @param xpm xpm do objeto a ser criado
 * @param type Tipo de objeto criado
 * @returns Objeto criado
 */
Object *createObject(xpm_row_t xpm[], int type);

/**
 * Função para destruir um objeto
 * @param obj objeto a ser destruido
 */
void destroy_Object(Object *obj);

/**
 * Função para mover um objeto
 * @param obj objeto a ser movido
 */
void moveXpm(Object *obj);


/**
 * Função rodar em X
 * @param angle angulo da rotação
 * @param x coordenada x normalizada
 * @param y coordenada y normalizada
 * @param width largura do objeto
 * @param height altura do objeto
 * @param centerX coordenada X do centro do objeto
 * @param centerY coordenada Y do centro do objeto
 * @param cos_value valor do co-seno
 * @param sin_value valor do seno
 * @returns valor arredondado da resolução
 */
uint16_t rotateX(double angle, int x, int y, uint16_t width, uint16_t heigth, int centerX, int centerY, double cos_value, double sin_value);

/**
 * Função rodar em Y
 * @param angle angulo da rotação
 * @param x coordenada x normalizada
 * @param y coordenada y normalizada
 * @param width largura do objeto
 * @param height altura do objeto
 * @param centerX coordenada X do centro do objeto
 * @param centerY coordenada Y do centro do objeto
 * @param cos_value valor do co-seno
 * @param sin_value valor do seno
 * @returns valor arredondado da resolução
 */
uint16_t rotateY(double angle, int x, int y, uint16_t width, uint16_t heigth, int centerX, int centerY, double cos_value, double sin_value);

/**
 * Função para atualizar o angulo da nave espacial em funcao do apontador do rato
 * @param ship objeto da nave espacial
 * @param mouse_pointer apontador do Rato
 */
void updateAngle(Object *ship, Mouse_pointer mouse_pointer);

/**
 * Função para criar Pontuação do jogo
 * @param punctuation pontuação a ser criada.
 */
void createPunctuation(Object *punctation[]);



#endif /* OBJECT_H */
