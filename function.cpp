#include "function.h"


bool init(){
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)!=0){
		printf("Video, Timer, Audio: %s\n",SDL_GetError());
		return true;
	}
	if(!(IMG_Init(IMG_INIT_PNG))){
		printf("Image: %s\n",IMG_GetError());
		return true;
	}
	if(TTF_Init()<0){
		printf("SDL_ttf: %s\n",TTF_GetError());
	}
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0){
		printf("SDL_mixer: %s\n", Mix_GetError());
	}
	//Creating Window.
	win = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIN_WIDTH,WIN_HEIGHT,0);

	if(win == NULL){
		printf("Window: %s\n",SDL_GetError());
		SDL_Quit();
		return true;
	}

	Uint32 rend_f = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
	rend = SDL_CreateRenderer(win,-1,rend_f);

	if(rend==NULL){
		printf("Render: %s\n",SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return true;
	}
	return false;
}

void stop(){
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	exit(1);
}

float max(float x,float y){
	if(x>y)
		return x;
	return y;
}

float min(float x,float y){
	if(x<y)
		return x;
	return y;
}

void errorS(){
	if(surface==NULL){
		printf("Surfece error: %s\n",SDL_GetError());
		stop();	
	}
}

void errorT(SDL_Texture *tex){
	if(tex==NULL){
		printf("Texture Error: %s\n",SDL_GetError());
		stop();
	}	
}

SDL_Texture* loadTex(std::string path){
	text = NULL;
	surface = IMG_Load(path.c_str());
	errorS();
	text = SDL_CreateTextureFromSurface(rend,surface);
	SDL_FreeSurface(surface);
	errorT(text);
	return text;
}
SDL_Texture* showText(std::string path, TTF_Font *font){
	text = NULL;
	if(font==NULL)
		printf("Faild to load TTF :%s\n",SDL_GetError());
	surface = TTF_RenderText_Solid(font,path.c_str(),textColor);
	errorS();
	text = SDL_CreateTextureFromSurface(rend,surface);
	SDL_FreeSurface(surface);
	errorT(text);
	return text;
}
SDL_Texture* intTotext(int num, TTF_Font *font){
	text = NULL;
	std::string s = std::to_string(num);
	surface = TTF_RenderText_Solid(font,s.c_str(),textColor);
	errorS();
	text = SDL_CreateTextureFromSurface(rend,surface);
	SDL_FreeSurface(surface);
	errorT(text);
	return text;
}
void applyDisplay(SDL_Texture *x){
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend,x,NULL,NULL);
	SDL_RenderPresent(rend);
}

int mouseHover(int x,int y,int x1,int x2,int y1,int y2){
	if(x>=x1 && x<=x2 && y>=y1 && y<=y2)
		return 1;
	return 0;
} 

bool collision(obj R, obj S){
	if(!(S.rect.x+S.rect.w <= R.rect.x || S.rect.x >= R.rect.x+R.rect.w))
		if((S.rect.y>R.rect.y && S.rect.y<R.rect.y+R.rect.h)
			||(S.rect.y+S.rect.h>R.rect.y && S.rect.y+S.rect.h<R.rect.y+R.rect.h)){
			return true;
		}
	return false;
}

int mouseClick(int x1, int x2, int y1, int y2){
	int msx,msy;
	int buttons = SDL_GetMouseState(&msx,&msy);
			
	if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
		if(mouseHover(msx,msy,x1,x2,y1,y2))
			return 1;
	return 0;			
}

void render(int n,...){
	va_list list;
	va_start(list,n);
	for(int i=0;i<n;i++){
		obj object = va_arg(list,obj);
		SDL_RenderCopy(rend,object.tex,NULL,&object.rect);
	}
	va_end(list);
}

void destroyTex(int n,...){
	va_list list;
	va_start(list,n);
	for(int i=0;i<n;i++){
		obj object = va_arg(list,obj);
		SDL_DestroyTexture(object.tex);
	}
	va_end(list);
}