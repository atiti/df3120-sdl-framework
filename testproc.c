#include "picframe.h"
#include <time.h>

#define NUM_WINDOWS 3

Element_t left_arrow_icon, right_arrow_icon, window_label;
Element_t time_disp, info_disp;
Element_t media_btns[5];
char *window_labels[NUM_WINDOWS] = { "Clock", "Media", "Weather" };
struct LList_t *curr_window = NULL;
int curr_window_idx=1,prev_window_idx = 1;
SDL_Event event;
SDL_Color fg = {0,0,0,0};
SDL_Color bg = {255,255,255,0};

int handle_input() {
	int i=0;
	struct LList_t *tmp_window = NULL;
	while (picframe_get_event(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				printf("Quitting\n");
                                return 1;
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
                                        if (curr_window_idx > NUM_WINDOWS) curr_window_idx = NUM_WINDOWS;
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
		if (prev_window_idx != curr_window_idx) {
			prev_window_idx = curr_window_idx;
			return 2;
		}
		prev_window_idx = curr_window_idx;
	}
	return 0;
}



void first_window_setup() {
        SDL_Rect tmp;

        /* Left arrow */
        tmp.x = 20;
        tmp.y = 20;
        picframe_add_button(&left_arrow_icon, &tmp, "data/glyphicons_210_left_arrow.png", "data/glyphicons_210_left_arrow_selected.png");
        picframe_add_element_to_window(curr_window, &left_arrow_icon);

        /* Right arrow */
        tmp.x = 320-40;
        tmp.y = 20;
        right_arrow_icon.selected = 1;
        picframe_add_button(&right_arrow_icon, &tmp, "data/glyphicons_211_right_arrow.png", "data/glyphicons_211_right_arrow_selected.png");
        picframe_add_element_to_window(curr_window, &right_arrow_icon);

	window_label.surface = NULL;
	window_label.rect.x = 110;
	window_label.rect.y = 10;
	window_label.selected =0;
	window_label.dynamic = 0;
	picframe_add_element_to_window(curr_window, &window_label);	


        picframe_load_font("fonts/Ubuntu-L.ttf", 80);

        /* Dynamic watch label */
        time_disp.surface = NULL; //sometext;
        time_disp.rect.x = 5;
        time_disp.rect.y = 50;
        time_disp.selected = 0;
        time_disp.dynamic = 1; /* Free up the surface after use, otherwise memory leak! */
        picframe_add_element_to_window(curr_window, &time_disp);

        /* Dynamic info label */
        info_disp.surface = NULL;
        info_disp.rect.x = 10;
        info_disp.rect.y = 200;
        info_disp.selected = 0;
        info_disp.dynamic = 1;
        picframe_add_element_to_window(curr_window, &info_disp);
}

int first_window_loop() {
        time_t now, prev=0;
        struct tm* tm;
        char buff[50];
	int ret, lightlevel, brightness;

	SDL_FreeSurface(window_label.surface);
        picframe_load_font("fonts/Ubuntu-L.ttf", 30);
	picframe_gen_text(&window_label.surface, fg, bg, window_labels[0]);

	while (1) {
		ret = handle_input();
		if (ret) return ret;

		now = time(0);
                if (now > prev) {
                        tm = localtime(&now);
                        sprintf(buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
                        picframe_load_font("fonts/Ubuntu-L.ttf", 80);

                        picframe_gen_text(&time_disp.surface, fg, bg,buff);
                        debug_printf("Setting time_disp surface to: %p\n", time_disp.surface);
                        prev = now;

                        lightlevel = picframe_get_lightsensor();
                        brightness = picframe_get_backlight();

                        sprintf(buff, "Light level: %d\tBrightness: %d", lightlevel, brightness);
                        picframe_load_font("fonts/Ubuntu-L.ttf", 20);
                        picframe_gen_text(&info_disp.surface, fg, bg, buff);
                        debug_printf("Setting info_disp surface to: %p\n", info_disp.surface);
                }

		picframe_update(curr_window);
		SDL_Delay(1);
	}
	return 0;
}


void second_window_setup() {
	struct LList_t *second_window = NULL;
	SDL_Rect tmp;
	int i=0;
	char *media_btn_array[] = { "data/glyphicons_172_rewind.png",
				    "data/glyphicons_173_play.png",
				    "data/glyphicons_174_pause.png", 
				    "data/glyphicons_175_stop.png",
				    "data/glyphicons_176_forward.png" };
	second_window = picframe_add_window();
	tmp.x = 50;
	tmp.y = 160;
	for(i=0;i<5;i++) {
		picframe_add_button(&media_btns[i], &tmp, media_btn_array[i], media_btn_array[i]);
		picframe_add_element_to_window(second_window, &media_btns[i]);
		tmp.x += 50;
	}

        picframe_add_element_to_window(second_window, &left_arrow_icon);
        picframe_add_element_to_window(second_window, &right_arrow_icon);	
        picframe_add_element_to_window(second_window, &window_label);
}

int second_window_loop() {
	int ret;

	SDL_FreeSurface(window_label.surface);
        picframe_load_font("fonts/Ubuntu-L.ttf", 30);
        picframe_gen_text(&window_label.surface, fg, bg, window_labels[1]);

        while (1) {
                ret = handle_input();
		if (ret) return ret;

                picframe_update(curr_window);
                SDL_Delay(1);
        }
	return 0;
}

void third_window_setup() {
	struct LList_t *third_window = NULL;
	third_window = picframe_add_window();

        picframe_add_element_to_window(third_window, &left_arrow_icon);
        picframe_add_element_to_window(third_window, &right_arrow_icon);
        picframe_add_element_to_window(third_window, &window_label);
}

int third_window_loop() {
	int ret;
	
	SDL_FreeSurface(window_label.surface);
        picframe_load_font("fonts/Ubuntu-L.ttf", 30);
	picframe_gen_text(&window_label.surface, fg, bg, window_labels[2]);

	while (1) {
		ret = handle_input();
		if (ret) return ret;
			
		picframe_update(curr_window);
		SDL_Delay(1);
	}
	return 0;	
}

int main() {
	int ret=0;

	picframe_init();
	curr_window = picframe_get_window(1);

	first_window_setup();
	second_window_setup();
	third_window_setup();

	picframe_set_backlight(500);

	while (1) {
		picframe_clear_screen();
		switch(curr_window_idx) {
			case 1:
				ret = first_window_loop();
				break;
			case 2:
				ret = second_window_loop();
				break;
			case 3:
				ret = third_window_loop();
				break;
			default:
				break;
		}	
		if (ret == 1) goto cleanup;

		picframe_update(curr_window);
		SDL_Delay(1);
	}

cleanup:
	picframe_cleanup();

	return 0;
}
