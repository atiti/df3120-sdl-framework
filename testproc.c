#include "picframe.h"


int main() {
	SDL_Event event;
	int i;

	SDL_Surface *testimg = NULL;

	picframe_load_image(&testimg, "data/glyphicons_011_camera.png");
	picframe_init();
	
	SDL_Rect r;
	r.x = 20;
	r.y = 20;
	picframe_disp_surface(testimg, &r);

	while (1) {
		picframe_get_event(&event);
		switch (event.type) {
			case SDL_QUIT:
				printf("Quitting\n");
				goto cleanup;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == 'l') {
					i = picframe_get_lightsensor();
					printf("Light sensor: %d\n", i);
				}
				break;
			default:
				break;
		}
		picframe_update();
	}

cleanup:
	picframe_cleanup();
}
