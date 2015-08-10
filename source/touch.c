#include <3ds.h>

bool touchInBox(touchPosition touch, int x, int y, int w, int h){
	int tx=touch.px;
	int ty=touch.py;
	u32 kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x+w && ty > y && ty < y+h){
		return true;
	}else{
		return false;
	}
}