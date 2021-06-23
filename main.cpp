#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "var.h"
#include "play.h"
#include "function.h"


//----------------------------Main Function-------------------------//

int main(int agrc, char* argv[]){
	
	if(init()) return -1;

	FILE *file = fopen("file/opt.txt", "r");
	if(file!=NULL){
		fscanf(file,"%d%d",&musicState,&effectState);
		fclose(file);
	} 
	music = Mix_LoadMUS("img/music.mp3");
	if(music==NULL)
		printf("Music could not loaded: %s\n",Mix_GetError());
	if(musicState==1)Mix_PlayMusic(music,-1);

	SDL_Texture *menu,*play,*tutorial,*option,*quit;
	menu = loadTex("img/menu.jpg");
	play = loadTex("img/play.jpg");
	tutorial = loadTex("img/tutorial.jpg");
	option = loadTex("img/option.jpg");
	quit = loadTex("img/quit.jpg");
	
	applyDisplay(menu);

	int close = 0;
	while(!close){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT)
				close = 1;
		}
		int msx,msy;
		int buttons = SDL_GetMouseState(&msx,&msy);
		
		if(mouseHover(msx,msy,1010,1158,212,250)){
			applyDisplay(play);
			if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
				while(true){
					int opt,score,high;
					score = runGame();
					if(score == -3)
						break;
					if(score<0){
						close = 1;
						continue;
					} 
					high = highScore(score);
					opt = GameOver(score,high);
					if(opt==1)
						continue;
					else if(opt==2)
						break;
					else if(opt<0){
						close = 1;
						continue;
					}
				}
			}
		}
		else if(mouseHover(msx,msy,932,1236,305,343)){
			applyDisplay(tutorial);
			if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
				open_tutorial();
			}
		}
		else if(mouseHover(msx,msy,969,1198,396,434)){
			applyDisplay(option);
			if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
				createOption();
			}  	
		}
		else if(mouseHover(msx,msy,1010,1158,489,527)){
			applyDisplay(quit);
			if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
				close = 1;
		}
		else{
			applyDisplay(menu);
		}
	}
	file = fopen("file/opt.txt", "w");
	fprintf(file,"%d %d",musicState,effectState);
	fclose(file);
	SDL_DestroyTexture(menu);
	SDL_DestroyTexture(play);
	SDL_DestroyTexture(tutorial);
	SDL_DestroyTexture(option);
	SDL_DestroyTexture(quit);
	Mix_HaltMusic(); 
	Mix_FreeMusic(music);
	stop();
	return 0;
}
