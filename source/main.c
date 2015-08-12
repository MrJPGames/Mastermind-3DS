#include <3ds.h>
#include <stdlib.h>
#include "touch.h"
#include "render.h"

//Sorry for the mess XD, doesn't look as nice from the inside as it does from the outside...

int full_field[28]={}; //Field on top screen
int full_judgement[28]={}; //All black white/empty markers of judgement
int local_field[]={0,0,0,0}; //Colors for current move on bottom screen 0=none
int ans[4]={};
int cur_field=0; //Selected field on bottom screen
int cur_row=0; //0 is first row
bool cur_visable=true;
int win=0;
int cheat=0;
int state=1; //0: Mode select 1: CPU game 2: 2-Player code selector 3: 2-Player guess
int difficulty=0; //0: easy 1: normal 2: hard
int modeselector=0;
int col_amount=8;

void resetGame(){
	//Init all vars
	u64 timeInSeconds = osGetTime() / 1000;
	srand (timeInSeconds);
	cur_field=0;
	cur_row=0;
	win=0;
	int x;
	for (x=0; x < 4; x++){
		local_field[x]=0;
		ans[x]=(rand() % col_amount)+1;
	}
	for (x=0; x < 28; x++){
		full_field[x]=0;
		full_judgement[x]=0;
	}
}

void startGame(int mode){
	resetGame();
	if (difficulty == 0){
		col_amount=6;
	}else if (difficulty == 1){
		col_amount=7;
	}else{
		col_amount=8;
	}
	int x;
	int y;
	for (x=0; x < 4; x++){
		y=(rand() % col_amount)+1;
		ans[x]=y;
	}
	if (mode == 1)
		state=2;
	if (mode == 2)
		state=1;
}

void submitTry(){
	if (state != 2){
		if (local_field[0] > 0 && local_field[1] > 0 &&  local_field[2] > 0 &&  local_field[3] > 0 && cur_row < 7 && !win){
			int black=0;
			int white=0;
			int x;
			int checked[]={0,0,0,0};
			for (x=0; x < 4; x++){
				full_field[cur_row*4+x]=local_field[x];
				if (local_field[x]==ans[x]){
					black+=1;
					if (checked[x] == 1)
						white-=1;
					checked[x]=1;
				}else{
					int y;
					for (y=0; y<4; y++){
						if (local_field[x]==ans[y] && checked[y] != 1){
							white+=1;
							checked[y]=1;
							break;
						}
					}
				}
				local_field[x]=0;
			}
			if (black == 4)
				win=1;
			int z;
			for (z=0; z<4; z++){
				if (black > 0){
					full_judgement[cur_row*4+z]=1;
					black-=1;
				}else if (white > 0){
					full_judgement[cur_row*4+z]=2;
					white-=1;
				}
			}
			cur_row+=1;
		}
	}else{
		state=3;
		cheat=0;
		int x;
		for (x=0; x<4; x++){
			ans[x]=local_field[x];
			local_field[x]=0;
		}
	}
}

bool secretCode(void)
{
	static const u32 secret_code[] =
	{
		KEY_UP,
		KEY_UP,
		KEY_DOWN,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_B,
		KEY_A,
	};

	static u32 state   = 0;
	static u32 timeout = 30;
	u32 down = hidKeysDown();

	if(down & secret_code[state])
	{
		++state;
		timeout = 30;

		if(state == sizeof(secret_code)/sizeof(secret_code[0]))
		{
			state = 0;
			return true;
		}
	}

	if(timeout > 0 && --timeout == 0)
	state = 0;

	return false;
}

touchPosition touch;

int main(){
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInitDefault();
	resetGame();
	int x;
	while (aptMainLoop()){
		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		//u32 kHeld = hidKeysHeld(); Not used an otherwise you'll get warning during compile, this makes compile more pleasant to look at yea!
		if (state != 0 && cur_row != 7){
			if (secretCode() && state != 3)
				cheat=1;
			if (kDown & KEY_TOUCH){
				cur_visable=false;
			}

			if (kDown & KEY_RIGHT){
				cur_field+=1;
				if (cur_field > 3)
					cur_field=0;
				cur_visable=true;
			}
			if (kDown & KEY_LEFT){
				cur_field-=1;
				if (cur_field < 0)
					cur_field=3;
				cur_visable=true;
			}
			if (kDown & KEY_UP){
				local_field[cur_field]+=1;
				if (local_field[cur_field] > col_amount)
					local_field[cur_field]=1;
				cur_visable=true;
			}
			if (kDown & KEY_DOWN){
				local_field[cur_field]-=1;
				if (local_field[cur_field] < 1)
					local_field[cur_field]=col_amount;
				cur_visable=true;
			}
			if (kDown & KEY_A)
				submitTry();
			if (touchInBox(touch,231,163,84,44))
				submitTry();

			if (touchInBox(touch,7,28,71,16)){
				local_field[0]+=1;
				if (local_field[0] > col_amount)
					local_field[0]=1;
			}
			if (touchInBox(touch,85,28,71,16)){
				local_field[1]+=1;
				if (local_field[1] > col_amount)
					local_field[1]=1;
			}
			if (touchInBox(touch,163,28,71,16)){
				local_field[2]+=1;
				if (local_field[2] > col_amount)
					local_field[2]=1;
			}
			if (touchInBox(touch,241,28,71,16)){
				local_field[3]+=1;
				if (local_field[3] > col_amount)
					local_field[3]=1;
			}
			if (touchInBox(touch,7,119,71,16)){
				local_field[0]-=1;
				if (local_field[0] <= 0)
					local_field[0]=col_amount;
			}
			if (touchInBox(touch,85,119,71,16)){
				local_field[1]-=1;
				if (local_field[1] <= 0)
					local_field[1]=col_amount;
			}
			if (touchInBox(touch,163,119,71,16)){
				local_field[2]-=1;
				if (local_field[2] <= 0)
					local_field[2]=col_amount;
			}
			if (touchInBox(touch,241,119,71,16)){
				local_field[3]-=1;
				if (local_field[3] <= 0)
					local_field[3]=col_amount;
			}
			if (state != 2){
				for (x=0; x < 4; x++){
					full_field[cur_row*4+x]=local_field[x];
				}
			}
		}else if (state == 0){
			if (touchInBox(touch,65,66,98,34))
				modeselector=1;
			if (touchInBox(touch,65,102,98,34))
				modeselector=2;
			if (modeselector != 0){
				if (touchInBox(touch,168,65,88,22)){
					difficulty=0;
					startGame(modeselector);
				}
				if (touchInBox(touch,168,90,88,22)){
					difficulty=1;
					startGame(modeselector);
				}
				if (touchInBox(touch,168,125,88,22)){
					difficulty=2;
					startGame(modeselector);
				}
			}
		}

		if (touchInBox(touch,0,211,320,30)){
			resetGame();
			state=0;
			modeselector=0;
		}
		if (touchInBox(touch,0,0,74,21))
				break;

		render(full_field, full_judgement, local_field, cur_field, cur_row, cur_visable, ans, win, cheat, state, modeselector);
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForEvent(GSPEVENT_VBlank0, false);
	}
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}