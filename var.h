#ifndef VAR_H
#define VAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


#define WIN_WIDTH (1280)
#define WIN_HEIGHT (760)

extern SDL_Window *win;
extern SDL_Renderer *rend;
extern SDL_Surface* surface;
extern SDL_Color textColor;
extern SDL_Texture *text;
extern Mix_Music *music;
extern Mix_Chunk *effect;
extern int effectState;
extern int musicState;



typedef struct obj{
	int num=0;
	SDL_Rect rect;
	SDL_Texture *tex=NULL;

}obj;


#endif