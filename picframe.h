#ifndef __PICFRAME_FRAMEWORK__
#define __PICFRAME_FRAMEWORK__

#define WIDTH 320
#define HEIGHT 240

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

/* SDL specific stuff */
SDL_Surface *_screen;
TTF_Font *_font;

/* Picture frame GUI stuff */
struct LList_t *_windows;
int _num_windows;

typedef struct {
	SDL_Surface *surface;
	SDL_Surface *surface_selected;
	SDL_Rect rect;
	int selected;
} Element_t;


int picframe_init();
struct LList_t *picframe_add_window();
struct LList_t *picframe_get_window();
struct LList_t *picframe_add_element_to_window(struct LList_t *window, Element_t *data);
int picframe_get_event(SDL_Event *event);
int picframe_get_lightsensor();
int picframe_get_backlight();
int picframe_set_backlight(int i);
int picframe_load_font(char *path, int size);
int picframe_gen_text(SDL_Surface **target, SDL_Color fgcolor, SDL_Color bgcolor, char *text);
int picframe_load_image(SDL_Surface **target, char *path);
int picframe_disp_surface(SDL_Surface *src, SDL_Rect *rect);
int picframe_add_button(Element_t *b, SDL_Rect *rect, char *path, char *selected);
int picframe_update();
void picframe_cleanup();

#endif
