#include "common.h"
#include "picframe.h"


int main() {
	int i=0;

	while (1) {
		for(i=150;i<700;i += 2) {
			picframe_set_backlight(i);
		}
		for(i=700;i>150;i -= 2){
			picframe_set_backlight(i);
		}
	}
}
