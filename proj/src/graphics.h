#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <minix/type.h>

#include "object.h"
#include "game.h"

/** @defgroup graphics graphics
 * @{
 *
 * Funções para o modo de video
 */

#define VBE_INFO	 0x4F01 /**< @brief VBE: Get mode info*/ 
#define SET_VBE_MODE   0x4F02 /**< @brief VBE: Set mode info*/ 
#define NO_MESSAGE     0
#define 	PB2BASE(x)   (((x) >> 4) & 0x0F000)
#define 	PB2OFF(x)    ((x) & (0x0FFFF))
#define VBE_MODE_INF   0x4F02
#define VBE_INT        0x10    /**< @brief comando para aceder aos serviços de video do VBE*/ 

#define MODE105 			  0x105 /**< @brief Modo de Video 0x105*/ 
#define MODE110 		  	0x110 /**< @brief Modo de Video 0x110*/ 
#define MODE115 		  	0x115 /**< @brief Modo de Video 0x115*/ 
#define MODE11A		  		0x11A /**< @brief Modo de Video 0x11A*/ 
#define MODE14C		 	  	0x14C /**< @brief Modo de Video 0x14C*/ 

#define BLACK_COLOR 0x000000 /**< @brief Cor Preta*/

#define ANGLE2RAD(A)  A * 3.14 / 180.0 /**< @brief Ângulo*/


static vbe_mode_info_t vmi_p;
 uint16_t h_res;		//screen horizontal resolution
 uint16_t v_res; 		//screen vertical resolution
static uint8_t bits_per_pixel; // number of bits on each pixel

/**
 * Função para inicializar o modo de video
 * @param mode - O modo de video que irá ser inicializado
 */
void *(vg_init)(uint16_t mode);

	
/**
 * Função para se obter a informação do modo de video
 * @param mode - O modo de video cuja informação será obtida
 * @param vmi_p - struct onde será guardada a informação obtida
 * @returns 0 em caso de sucesso, 1 em caso de falha
 */
int vbe_get_info(uint16_t mode, vbe_mode_info_t* vmi_p);

/**
 * Função para desenhar um rectângulo
 * @param x - Coordenada no eixo dos x em que começa o desenho do rectângulo
 * @param y - Coordenada no eixo dos y em que começa o desenho do rectângulo
 * @param width - Largura do rectângulo em questão
 * @param height - Altura do rectângulo em questão
 * @param color - Cor do rectângulo em questão
 * @returns 0 em caso de sucesso
 */
int draw_rectangle(int x, int y, uint16_t width,uint16_t height,uint32_t color);

/**
 * Função para desenhar uma linha
 * @param x - Coordenada no eixo dos x em que começa o desenho da linha
 * @param y - Coordenada no eixo dos y em que começa o desenho da linha
 * @param width - Largura da linha em questão
 * @param color - Cor da linha em questão
 * @returns 0 em caso de sucesso
 */
int drawLine(int x, int y,uint16_t width,uint32_t color);

/**
 * Função para definir o modo de video
 * @param mode - O modo de video que irá ser definido
 * @returns 0 em caso de sucesso
 */
int set_mode(uint16_t mode);

/**
 * Função para desenhar um objeto no ecrã de jogo
 * @param xpm - Objeto a ser desenhado
 * @returns 0 em caso de sucesso
 */
int draw_xpm(Object *xpm);

/**
 * Função para desenhar um objeto no ecrã de menu
 * @param xpm - Objeto a ser desenhado
 * @returns 0 em caso de sucesso
 */
int drawMenu(Object *xpm);

/**
 * Função para limpar o ecrã
 * @returns 0 em caso de sucesso
 */
int clear_screen();

/**
 * Função para pintar um pixel de uma determinada cor
 * @param x - Coordenada no eixo dos x do pixel a ser desenhado
 * @param y - Coordenada no eixo dos y do pixel a ser desenhado
 * @param color - Cor na qual o pixel será pintado
 * @param transpColor - Cor transparente definida
 * @returns - número de pixeis desenhados fora do ecran
 */
int coloring(int x,int y, uint32_t color, uint32_t transpColor);

/**
 * Função para copiar a memória buffer para o RAM
 */
void copyBuffToRam();

/**
 * Função para sair do modo de video e voltar ao modo texto (default do Minix)
 */
void vg_default();

#endif /* GRAPHICS_H */
