#include "picframe.h"

int picframe_init() {
	/* SDL Setup */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
	exit(1);
	}
	atexit(SDL_Quit);

	_screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32,
                               SDL_HWSURFACE );
	if ( _screen == NULL )
	{
		fprintf(stderr, "Could not setup screen to resolution %dx%d : %s\n",
		WIDTH, HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_FillRect(_screen, &_screen->clip_rect, SDL_MapRGB(_screen->format, 255, 255, 255));

	return 0;
}


int picframe_get_event(SDL_Event *event) {
	SDL_PollEvent(event);
	
}

int picframe_update() {
	SDL_Flip(_screen);
}

int picframe_get_lightsensor() {
	FILE *fp = NULL;
	int val = 0;

	fp = fopen("/sys/devices/platform/s3c24xx-adc/s3c-hwmon/adc0_raw", "r");
	if (!fp) return -1;

	fscanf(fp, "%d", &val);
	fclose(fp);
	
	return val;
}

int picframe_load_image(SDL_Surface **target, char *path) {
	SDL_Surface *tmp = NULL;
	tmp = IMG_Load(path);
	if (!tmp) {
		fprintf(stderr, "Failed to load image %s\n", path);
		return -1;
	}
	*target = tmp; //SDL_DisplayFormat(tmp);
	//SDL_FreeSurface(tmp);
	return 0;
}

int picframe_disp_surface(SDL_Surface *src, SDL_Rect *rect) {
	SDL_BlitSurface(src, NULL, _screen, rect);
	return 0;
}

void picframe_cleanup() {

}
