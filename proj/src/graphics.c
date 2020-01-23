#include <lcom/lcf.h>

#include "graphics.h"


#include <math.h>

static char *video_mem = NULL;
static char *buffer = NULL;
static uint16_t num_bytes = 0;


void *(vg_init)(uint16_t mode){

  struct minix_mem_range mr;
  int ra; 
  mr.mr_base = 0;	
  mr.mr_limit = 1<<20;  
  if( OK != (ra = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", ra);

    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = SET_VBE_MODE;// VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = VBE_INT;
    
    if( sys_int86(&r) != OK ) {
      printf("set_vbe_mode: sys_int86() failed \n");
      return NULL;
    }
    return 0;
}


int vbe_get_info(uint16_t mode, vbe_mode_info_t* vmi_p){		
   mmap_t map;
  phys_bytes buf;
  reg86_t r;

  memset(&r, 0, sizeof(r));	/* zero the structure */
 

  if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
  	printf("vbe_get_mode_inf: failed to allocate memory \n");
  	return 1;
  }

  buf = map.phys;

  r.ax = VBE_INFO; /* VBE get mode info */
  /* translate the buffer linear address to a far pointer */
  r.es = PB2BASE(buf); /* set a segment base */
  r.di = PB2OFF(buf); /* set the offset accordingly */
  r.cx = mode;
  r.intno = VBE_INT;

  if (sys_int86(&r) != OK){ 
  	printf("vbe_get_mode_inf: sys_int86() failed to obtain mode info \n");
  	lm_free(&map);
  	return 1;
  }

  if(r.ah != 0){
  	lm_free(&map);
  	return 1;
  }

  *vmi_p = *(vbe_mode_info_t *)map.virt;

  lm_free(&map);

  return 0;
}


int (vg_draw_hline)(uint16_t x, uint16_t y,uint16_t width,uint32_t color){      
  uint32_t dummyColor= color;
  dummyColor += 1;
  for(unsigned int i = 0; i < width; i++){
    if(x >= h_res) continue;
    if(coloring(x, y, color, dummyColor) != OK) return -1; //for each pixel calls coloring function to change it's color
    x++;
  }
  return 0;
}

int (draw_rectangle)(int x, int y,uint16_t width,uint16_t height,uint32_t color){
  for(unsigned int i= 0; i < height; i++){ 
    if(y>= v_res) break;
    if(vg_draw_hline(x, y, width, color) != OK) return -1; //for each line calls drawLIne
    y++;
  }
  return 0;
}


int set_mode(uint16_t mode){

  /************set_mode_info*******************/
  struct minix_mem_range mr;
   mmap_t mmap;
  int ra; 
  mr.mr_base = 0;	
  mr.mr_limit = 1<<20;  
  if( OK != (ra = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", ra);

    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = SET_VBE_MODE;// VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = VBE_INT;
    
    if( sys_int86(&r) != OK ) {
      printf("set_vbe_mode: sys_int86() failed \n");
      return -1;
    }
  /*********************************************/

  /****************get video mode ************/
    memset(&vmi_p, 0, sizeof(vbe_mode_info_t));
    if(vbe_get_info(mode, &vmi_p) != OK){
     lm_free(&mmap);
     return -1;
    }  
   /******************************************/
  
 
  lm_alloc(sizeof(vbe_mode_info_t), &mmap);

  /**********map the video memory ************/
  /* Allow memory mapping */
  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  phys_bytes videoRamSize = vmi_p.XResolution * vmi_p.YResolution * ceil(vmi_p.BitsPerPixel/8);
  mr.mr_base = (phys_bytes) vmi_p.PhysBasePtr;	
  mr.mr_limit = vmi_p.PhysBasePtr + videoRamSize;

  if( OK != (ra = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", ra);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base,  videoRamSize);
  buffer = malloc(videoRamSize);

  if(video_mem == MAP_FAILED)
     panic("couldn't map video memory");
  /******************************************/

  lm_free(&mmap);
  return 0;
}




int draw_xpm(Object *xpm){
  
  num_bytes = ceil(bits_per_pixel/8.0);
  if(bits_per_pixel == 15) num_bytes = 2;
  uint16_t total_pixels_failed = 0;

  uint32_t color = 0;
  uint32_t transpColor = xpm_transparency_color(EIGHT_BYTES_TYPE);
  int ant_x = xpm->posX;
  int y = xpm->posY;
  uint8_t *tempMap = xpm->map;
  double cos_value = cos(xpm->angle);
  double sin_value = sin(xpm->angle);

  for (unsigned int i = 0; i < xpm->height; i++,y++){
		
		for(int j = 0; j < xpm->width; j++, ant_x++){
		
      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(tempMap + byte) << (byte*8);
      }
      
        total_pixels_failed += coloring(rotateX(xpm->angle, ant_x, y, xpm->width, xpm->height,                                              xpm->centerX, xpm->centerY, cos_value, sin_value), 
                                        rotateY(xpm->angle, ant_x,y, xpm->width, xpm->height,               xpm->centerX, xpm->centerY, cos_value, sin_value),
                                        color,
                                        transpColor);
        tempMap += num_bytes;
		}
		ant_x = xpm->posX;
	}
  if(total_pixels_failed >= (round(xpm->width * xpm->height / 3))) {
    return -1;
  }
	else return 0;
}


int drawMenu(Object *xpm){
  num_bytes = ceil(bits_per_pixel/8.0);
  if(bits_per_pixel == 15) num_bytes = 2;
  uint16_t total_pixels_failed = 0;
  uint32_t color = 0;
  uint32_t transpColor = xpm_transparency_color(EIGHT_BYTES_TYPE);
  int ant_x = xpm->posX;
  int y = xpm->posY;
  uint8_t *tempMap = xpm->map;

  for (unsigned int i = 0; i < xpm->height; i++,y++){
		
		for(int j = 0; j < xpm->width; j++, ant_x++){
		
      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(tempMap + byte) << (byte*8);
      }
      total_pixels_failed += coloring(ant_x, y,
                                        color,
                                        transpColor);
        tempMap += num_bytes;
		}
		ant_x = xpm->posX;
	}
  if(total_pixels_failed >= (round(xpm->width * xpm->height / 2))) {
    return -1;
  }
	else return 0;
}

int clear_screen(){
  num_bytes = ceil(bits_per_pixel/8.0);
  if(bits_per_pixel == 15) num_bytes = 2;
   memset(buffer, 0, h_res * v_res * num_bytes);

  return 0;
}


int coloring(int x,int y, uint32_t color, uint32_t transpColor){
    //pixel outside screen
  if(x >= ((int) h_res) || x < 1 || y >= ((int) v_res) || y < 1){
    return 1;
  }
  if(color == transpColor) return 0;

  unsigned int bytes_by_pixel = ceil(bits_per_pixel/8.0);
  if(bits_per_pixel == 15) bytes_by_pixel = 2;
  uint8_t color_tmp = 0;
  unsigned int pos = 0;

  for(unsigned int i = 0; i < bytes_by_pixel; i++ ){
    color_tmp = color & 0xFF;
    *(buffer+(y*h_res* bytes_by_pixel) + (x * bytes_by_pixel) + pos) = color_tmp;
    color = color >> 8;
    pos++;
  }
  return 0;
}


void copyBuffToRam(){
   memcpy(video_mem, buffer, h_res * v_res * num_bytes);
}

void vg_default(){
  
  free(buffer);
  vg_exit();
}
