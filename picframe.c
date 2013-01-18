#include "picframe.h"

int picframe_init() {
	_font = NULL;
	/* SDL Setup */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
	exit(1);
	}
	atexit(SDL_Quit);
	/* Initialize our screen */
	_screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32,
                               SDL_HWSURFACE );
	if ( _screen == NULL )
	{
		fprintf(stderr, "Could not setup screen to resolution %dx%d : %s\n",
		WIDTH, HEIGHT, SDL_GetError());
		exit(1);
	}
	/* Hide the mouse */
	SDL_ShowCursor(0); 
	/* Solid white background */
	SDL_FillRect(_screen, &_screen->clip_rect, SDL_MapRGB(_screen->format, 255, 255, 255));
	/* Initialize the fonts */
	if ( TTF_Init() == -1 ) {
		fprintf(stderr, "Could not initalize SDL_ttf!\n");
		exit(1);
	}
	/* Load a default font */
	picframe_load_font("fonts/Ubuntu-L.ttf", 20);

	/* Aight, lets make our first default window */
	_num_windows = 0;
	struct LList_t *node = picframe_add_window();

	return 0;
}

struct LList_t *picframe_add_window() {
	struct LList_t *node = NULL;

	_num_windows++;
	node = llist_add_node(&_windows, _num_windows, NULL);

	return node;
}

struct LList_t *picframe_get_window(int id) {
	struct LList_t *node = NULL;
	node = llist_find_node(_windows, id);
	return node; 
}

struct LList_t *picframe_add_element_to_window(struct LList_t *window, Element_t *data) {
	struct LList_t *node = NULL;
	if (window == NULL) return NULL;
	
	node = llist_add_node((struct LList_t **)&window->data, 1, (void *)data);
	
	return node;
}


int picframe_get_event(SDL_Event *event) {
	//SDL_WaitEvent(event);
	return SDL_PollEvent(event);
}

int picframe_update(struct LList_t *window) {
	struct LList_t *curr = window->data;
	Element_t *element = NULL;
	SDL_Surface *ts;
	while (curr) {
		if (curr->data) {
			element = (Element_t *)curr->data;
			if (!element) continue;

			//printf("Disp surface %p and rect.x: %d rect.y: %d\n", element->surface, element->rect.x, element->rect.y);
			if (element->selected) ts = element->surface_selected;
			else ts = element->surface;

			picframe_disp_surface(ts, &element->rect);		
		}
		curr = curr->next;
	}

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

int picframe_get_backlight() {
	FILE *fp = NULL;
	int val = 0;
	fp = fopen("/sys/class/backlight/pwm-backlight.0/brightness", "r");
	if (!fp) return -1;
	fscanf(fp, "%d", &val);
	fclose(fp);
	
	return val;
}

int picframe_set_backlight(int i) {
	FILE *fp = NULL;
	fp = fopen("/sys/class/backlight/pwm-backlight.0/brightness", "w");
	if (!fp) return -1;
	
	fprintf(fp, "%d", i);
	fclose(fp);

	return 1;
}

int picframe_load_font(char *path, int size) {
	if (_font) TTF_CloseFont(_font);

	_font = TTF_OpenFont(path, size);
	if (_font == NULL){
		fprintf(stderr, "Unable to load font: %s %s \n", path, TTF_GetError());
		// Handle the error here.
		return -1;
	}
    	return 0;
}

int picframe_gen_text(SDL_Surface **target, SDL_Color fgcolor, SDL_Color bgcolor, char *text) {
	*target = TTF_RenderText_Shaded(_font, text, fgcolor, bgcolor);

	return 0;
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

int picframe_add_button(Element_t *b, SDL_Rect *rect, char *path, char *selected) {
	SDL_Surface *tmp, *tmp2 = NULL;
	if (picframe_load_image(&tmp, path) == -1) {
		fprintf(stderr, "Failed to create button %s!\n", path);
		return -1;
	}

	if (picframe_load_image(&tmp2, selected) == -1) {
		fprintf(stderr, "Failed to create button %s\n", selected);
		return -1;
	}

	b->surface = tmp;
	b->surface_selected = tmp2;
	memcpy(&(b->rect), rect, sizeof(SDL_Rect));
	b->selected = 0;

	return 0;	
}


void picframe_cleanup() {

}
