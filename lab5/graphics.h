#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <minix/type.h>

#define VBE_INFO	 0x4F01
#define SET_VBE_MODE   0x4F02
#define NO_MESSAGE     0
#define 	PB2BASE(x)   (((x) >> 4) & 0x0F000)
#define 	PB2OFF(x)    ((x) & (0x0FFFF))
#define VBE_MODE_INF   0x4F02
#define VBE_INT        0x10    //access VBE video services

#define MODE105 			  0x105
#define MODE110 		  	0x110
#define MODE115 		  	0x115
#define MODE11A		  		0x11A
#define MODE14C		 	  	0x14C


static vbe_mode_info_t vmi_p;
static uint16_t h_res;		//screen horizontal resolution
static uint16_t v_res; 		//screen vertical resolution
static uint8_t bits_per_pixel; // number of bits on each pixel
static char *video_mem;

int vbe_get_info(uint16_t mode, vbe_mode_info_t* vmi_p);

int draw_rectangle(uint16_t x, uint16_t y,uint16_t width,uint16_t height,uint32_t color);

int drawLine(uint16_t x, uint16_t y,uint16_t width,uint32_t color);

int set_mode(uint16_t mode);

int draw_xpm( xpm_map_t xpm, uint16_t x, uint16_t y);


#endif /* GRAPHICS_H */
