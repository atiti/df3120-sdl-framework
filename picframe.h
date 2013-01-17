#ifndef __PICFRAME_FRAMEWORK__
#define __PICFRAME_FRAMEWORK__

#define WIDTH 320
#define HEIGHT 240

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SDL_Surface *_screen;

int picframe_init();
int picframe_get_event(SDL_Event *event);
int picframe_get_lightsensor();
int picframe_load_image(SDL_Surface **target, char *path);
int picframe_update();
void picframe_cleanup();

#endif
