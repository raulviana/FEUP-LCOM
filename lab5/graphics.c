#include <lcom/lcf.h>

#include "graphics.h"


#include <math.h>


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

int coloring(uint16_t x,uint16_t y, uint32_t color){
  if(x>=h_res || y >= v_res){
    printf("Triying to access inexistent pixel");
    return -1;
  }
  unsigned int bytes_by_pixel = bits_per_pixel/8;
  if(bits_per_pixel == 15) bytes_by_pixel = 2;
  uint8_t color_tmp = (uint8_t) color;
  unsigned int pos = 0;

  for(unsigned int i = 0; i < bytes_by_pixel; i++ ){
    *(video_mem+(y*h_res*bytes_by_pixel) + (x * bytes_by_pixel) + pos) = color_tmp;
    color = color >> 8;
    color_tmp = color;
    pos++;
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
  for(unsigned int i = 0; i < width; i++){
    if(x >= h_res) continue;
    if(coloring(x, y, color) != OK) return -1; //for each pixel calls coloring function to change it's color
    x++;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width,uint16_t height,uint32_t color){
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
  
  mmap_t mmap;
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

  if(video_mem == MAP_FAILED)
     panic("couldn't map video memory");
  /******************************************/

  lm_free(&mmap);
  return 0;
}


int draw_xpm( xpm_map_t xpm, uint16_t x, uint16_t y){
  
  uint16_t mode = MODE105; //setting mode
  if(set_mode(mode) != OK) {
    printf("Couldnt set %x video mode", mode);
  };

  vg_init(mode);

  xpm_image_t img;
  memset(&img, 0, sizeof(xpm_image_t));
  xpm_load(xpm, XPM_INDEXED, &img);

  uint16_t ant_x = x;
  for (unsigned int i = 0; i < img.height; i++,y++){
		if (y  >= v_res ){
				break;
		}
		for(int j = 0; j < img.width; j++, ant_x++){
			if (ant_x >= h_res ){
				continue;
			}
			if (coloring(ant_x, y, *(img.bytes + i * img.width + j)) != OK){
				return -1;
			}
		}
		ant_x = x;
	}
	return 0;
}




