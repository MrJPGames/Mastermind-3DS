#include "render.h"
#include "gfx.h"
#include "black_numbers_bin.h"
#include "bottom_bin.h"
#include "bottom_arrows_bin.h"
#include "bottom_colors_bin.h"
#include "bottom_circle_bin.h"
#include "bottom_difselect_bin.h"
#include "bottom_modeselect_bin.h"
#include "bottom_modeselectbuttons_bin.h"
#include "bottom_currentplayer_bin.h"
#include "grey_numbers_bin.h"
#include "top_bin.h"
#include "top_arrow_bin.h"
#include "top_colors_bin.h"
#include "top_pins_bin.h"
#include "top_gameover_bin.h"
#include "top_win_bin.h"
#include "top_instruction_bin.h"

//void gfxDrawSpriteAlphaBlend(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y);
void render(int full_field[], int full_judgement[], int local_field[], int cur_field, int cur_row, bool cur_visable, int ans[], int win, int cheat, int state, int modeselector){
	//render bgs
	gfxDrawSprite(GFX_TOP,GFX_LEFT, (u8*)top_bin, 240, 400, 0, 0);
	gfxDrawSprite(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_bin, 240, 320, 0, 0);

	//top
	int x;
	int y;
	int z;
	for (x=0; x<7; x++){
		for (y=0; y<4; y++){
			if (full_field[x*4+y]>0)
				gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_colors_bin+(full_field[x*4+y]-1)*1600, 20, 20, 11+x*33, 145+y*30);
		}
	}
	for (x=0; x<7; x++){
		for (y=0; y<2; y++){
			for(z=0;z<2; z++){
				if (full_judgement[y*2+z+x*4] == 1)
					gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_pins_bin+196, 7, 7, 22-8*y+33*x, 262+8*z);
				if (full_judgement[y*2+z+x*4] == 2)
					gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_pins_bin, 7, 7, 22-8*y+33*x, 262+8*z);
			}
		}
	}
	for (x=0; x<7; x++){
		if (cur_row == x){
			gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)black_numbers_bin+336*x, 12, 7, 15+33*x, 128);
			gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_arrow_bin, 10, 7, 15+33*x, 115);
		}else{
			gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)grey_numbers_bin+336*x, 12, 7, 15+33*x, 128);
		}
	}
	if (cur_row == 7){
		gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_gameover_bin, 80, 200, 100, 100);
		for (x=0; x<4; x++){
			gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_colors_bin+(ans[x]-1)*1600, 20, 20, 108, 123+45*x);
		}
	}
	if (win == 1){
		gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_win_bin, 60, 200, 90, 100);
	}
	if (cheat){
		for (y=0; y<4; y++){
			gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_colors_bin+(ans[y]-1)*1600, 20, 20, 0, y*27);
		}
	}
	//bottom
	if (local_field[0] != 0)
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_colors_bin+11664*(local_field[0]-1), 54, 54, 132, 16);
	if (local_field[1] != 0)
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_colors_bin+11664*(local_field[1]-1), 54, 54, 132, 94);
	if (local_field[2] != 0)
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_colors_bin+11664*(local_field[2]-1), 54, 54, 132, 172);
	if (local_field[3] != 0)
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_colors_bin+11664*(local_field[3]-1), 54, 54, 132, 250);

	if (cur_visable && cur_row != 7)
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_circle_bin, 72, 72, 123, 7+78*cur_field);

	if (state == 0){
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselect_bin, 80, 200, 100, 60);
		if (modeselector == 0){
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin, 34, 98, 141, 64);
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin+26656, 34, 98, 105, 64);
		}else if (modeselector == 1){
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin+13328, 34, 98, 141, 64);
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin+26656, 34, 98, 105, 64);
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_difselect_bin, 72, 88, 104, 167);
		}else{
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin, 34, 98, 141, 64);
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_modeselectbuttons_bin+39984, 34, 98, 105, 64);
			gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_difselect_bin, 72, 88, 104, 167);
		}
	}else if (state == 2){
		gfxDrawSpriteAlphaBlend(GFX_TOP,GFX_LEFT, (u8*)top_instruction_bin, 60, 200, 90, 100);
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_currentplayer_bin, 19, 111, 221, 209);
	}else if (state == 3){
		gfxDrawSpriteAlphaBlend(GFX_BOTTOM,GFX_LEFT, (u8*)bottom_currentplayer_bin+8436, 19, 111, 221, 209);
	}
}