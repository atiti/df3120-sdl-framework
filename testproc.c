#include "picframe.h"
#include <time.h>

void second_window_setup() {
	struct LList_t *second_window = NULL;
	SDL_Rect tmp;
	int i=0;
	char *media_btn_array[] = { "data/glyphicons_172_rewind.png",
				    "data/glyphicons_173_play.png",
				    "data/glyphicons_174_pause.png", 
				    "data/glyphicons_175_stop.png" };
	Element_t media_btns[4];
	second_window = picframe_add_window();
	tmp.x = 60;
	tmp.y = 160;
	for(i=0;i<4;i++) {
		picframe_add_button(&media_btns[i], &tmp, media_btn_array[i], media_btn_array[i]);
		picframe_add_element_to_window(second_window, &media_btns[i]);
		tmp.x += 40;
	}	


}


int main() {
	time_t now, prev=0;
	struct tm* tm;
	char buff[50];
	SDL_Event event;
	int i, lightlevel, brightness, curr_window_idx = 1;
	Element_t left_arrow_icon, right_arrow_icon;
	struct LList_t *curr_window = NULL, *tmp_window = NULL;

	picframe_init();
	curr_window = picframe_get_window(1);

	second_window_setup();

	picframe_set_backlight(500);

	SDL_Surface *sometext = NULL;
	SDL_Rect tmp;

	tmp.x = 20;
	tmp.y = 20;
	picframe_add_button(&left_arrow_icon, &tmp, "data/glyphicons_210_left_arrow.png", "data/glyphicons_210_left_arrow_selected.png");
	picframe_add_element_to_window(curr_window, &left_arrow_icon);

	tmp.x = 320-40;
	tmp.y = 20;
	right_arrow_icon.selected = 1;
	picframe_add_button(&right_arrow_icon, &tmp, "data/glyphicons_211_right_arrow.png", "data/glyphicons_211_right_arrow_selected.png");
	picframe_add_element_to_window(curr_window, &right_arrow_icon);


	picframe_load_font("fonts/Ubuntu-L.ttf", 80);

	SDL_Color fg = {0,0,0,0};
	SDL_Color bg = {255,255,255,0};
	picframe_gen_text(&sometext, fg,bg, "00:00");

	SDL_Rect inforect, fr;
	fr.x = 5;
	fr.y = 50;
	picframe_disp_surface(sometext, &fr);

	inforect.x = 10;
	inforect.y = 200;

	while (1) {
		while (picframe_get_event(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					printf("Quitting\n");
					goto cleanup;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_LEFT) {
						left_arrow_icon.selected = 1;
						curr_window_idx--;
						if (curr_window_idx < 1) curr_window_idx = 1;
						tmp_window = picframe_get_window(curr_window_idx);
						if (tmp_window) curr_window = tmp_window;
						else curr_window_idx++;

						printf("Current window: %d\n", curr_window_idx);
					}
	
				        if (event.key.keysym.sym == SDLK_RIGHT) {
	                                        right_arrow_icon.selected = 1;
						curr_window_idx++;
						if (curr_window_idx > 5) curr_window_idx = 5;
						tmp_window = picframe_get_window(curr_window_idx);
						if (tmp_window) curr_window = tmp_window;
	                               		else curr_window_idx--;
						printf("Current window: %d\n", curr_window_idx);
					 }
	
					if (event.key.keysym.sym == SDLK_RETURN) {
						i = picframe_get_lightsensor();
						printf("Light sensor: %d\n", i);
					}
					break;
				case SDL_KEYUP:
		                      	if (event.key.keysym.sym == SDLK_LEFT) {
       		                                 left_arrow_icon.selected = 0;
       		                         }
	
       	        	                 if (event.key.keysym.sym == SDLK_RIGHT) {
       		                                  right_arrow_icon.selected = 0;
       	               		          }

					break; 
				default:
					break;
			}
		}

		now = time(0);
		if (now > prev) {
			tm = localtime(&now);
			sprintf(buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
		        picframe_load_font("fonts/Ubuntu-L.ttf", 80);

			picframe_gen_text(&sometext, fg, bg,buff);
			picframe_disp_surface(sometext, &fr);
			SDL_FreeSurface(sometext);
			prev = now;

			lightlevel = picframe_get_lightsensor();
			brightness = picframe_get_backlight();

			sprintf(buff, "Light level: %d\tBrightness: %d", lightlevel, brightness);
		        picframe_load_font("fonts/Ubuntu-L.ttf", 20);
			picframe_gen_text(&sometext, fg, bg, buff);
			picframe_disp_surface(sometext, &inforect);
			SDL_FreeSurface(sometext);

		}
		picframe_update(curr_window);
		SDL_Delay(1);
	}

cleanup:
	picframe_cleanup();
}
