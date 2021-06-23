#include "play.h"

int runGame(){
	effect = Mix_LoadWAV("img/1.wav");
	if(effect==NULL)
		printf("Effect could not loaded: %s\n",Mix_GetError());

	TTF_Font *font = TTF_OpenFont("img/num.ttf",30);
	TTF_Font *font1 = TTF_OpenFont("img/all.ttf",20);
	if(font==NULL || font1==NULL) printf("Load font:%s\n",SDL_GetError());
	//----------------Game play option---------------//
	int timerOpt = 0;
	SDL_Texture *playOption;
	playOption = loadTex("img/playoption.jpg");
	applyDisplay(playOption);
	while(true){
		SDL_PumpEvents();
		if(mouseClick(211,511,230,530))
			break;
		if(mouseClick(776,1076,230,530)){
			timerOpt = 1;
			break;
		}
	}
	SDL_DestroyTexture(playOption);
	//-------------------Obj-----------------//
	obj timer;
	timer.num = 120;
	timer.rect = {WIN_WIDTH-60,80,50,20};
	timer.tex = intTotext(120,font);

	obj distance;
	distance.num = 0;
	distance.rect = {10,10,30,30};
	distance.tex = intTotext(0,font);

	obj speed;
	speed.num = 80;
	speed.rect = {(WIN_WIDTH-50)/2,10,50,50};
	speed.tex = intTotext(0,font);

	obj score;
	score.num = 0;
	score.rect = {WIN_WIDTH-60,25,50,30};
	score.tex = intTotext(0,font);

	obj bg[2];
	bg[0].rect = bg[1].rect = {0,0,WIN_WIDTH,WIN_HEIGHT};
	bg[0].tex = loadTex("img/newbg.jpg");
	bg[1].tex = loadTex("img/newbgr.jpg");
	float x_pos_bg1 = bg[0].rect.x;
	float x_pos_bg2 = WIN_WIDTH;

	obj lives;
	lives.num = 3;
	lives.rect = {0,45,30,30};
	lives.tex = loadTex("img/heart.png");

	obj crash;
	crash.tex = loadTex("img/crash.png");

	obj heli;
	heli.rect = {WIN_WIDTH/3,(WIN_HEIGHT-50)/2,130,50};
	heli.tex = loadTex("img/heli.png");
	float yph = heli.rect.y;
	
	obj wall[4];
	wall[1].rect = {(WIN_WIDTH-20)/2,0,20,240};
	wall[1].tex = loadTex("img/wall.jpg");
	wall[2] = wall[3] = wall[1];
	wall[2].rect.x += WIN_WIDTH/2;
	wall[3].rect.x += WIN_WIDTH;
	wall[2].rect.y = WIN_HEIGHT-wall[2].rect.h-42;
	wall[3].rect.y = (WIN_HEIGHT-wall[3].rect.h-42)/2;
	float xp[4],yp[4];
	for(int i=1;i<4;i++){
		xp[i] = wall[i].rect.x;
		yp[i] = wall[i].rect.y;
	}
	int consp = wall[3].rect.x;

	obj score_text;
	score_text.tex = showText("Score",font1);
	score_text.rect = {WIN_WIDTH-110,0,100,20};

	
	//-----------------------Main gameloop---------------------//
	int start_time = SDL_GetTicks(),end_time;

	srand(time(0));
	int wpc = 1,round = 4,cb=80;
	round += rand()%4;
	wpc += rand()%(round-3);

	float SCROLL_SPEED = 8, H_SPEED = 13, km=0;
	int close = 3,cycle=0,gameover=0,score_s,score_e,s_flag=1,cf=0,res = 1,ls,le;

	while(close){
		if(effectState==1)Mix_PlayChannel(-1,effect,-1);
		if(Mix_PausedMusic()==0)
			Mix_PauseMusic();

		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				score.num = -3;
				close = 0; //Go to Main Menu
			}
			else if(e.type==SDL_KEYDOWN){
				switch(e.key.keysym.scancode){
					case SDL_SCANCODE_UP:
						yph = max(0,yph-H_SPEED);
						break;
					case SDL_SCANCODE_DOWN:
						yph = min(WIN_HEIGHT-heli.rect.h, H_SPEED+yph);
						break;
					case SDL_SCANCODE_RIGHT:
						SCROLL_SPEED = min(18,SCROLL_SPEED+0.1);
						speed.num = min(speed.num+1,180);
						break;
					case SDL_SCANCODE_LEFT:
						SCROLL_SPEED = max(8,SCROLL_SPEED-0.3);
						speed.num = max(80,speed.num-3);
						break;
					case SDL_SCANCODE_SPACE:
						res = 0;
						if(effectState==1)Mix_HaltChannel(-1);
						while(true){
							while(SDL_PollEvent(&e)){
								if(e.type==SDL_QUIT){
									res = 1;
									score.num = -3;
									close = 0; //Go to Main Menu
								}
								if(e.type==SDL_KEYDOWN)
								if(e.key.keysym.scancode==SDL_SCANCODE_SPACE)res = 1; 
							}	
							if(res){
								if(effectState==1)Mix_PlayChannel(-1,effect,-1);
								break;   
							} 
						}
				}
			}
		}
		H_SPEED = SCROLL_SPEED*1.5;

		//------------Different position of walls.----------//
		if(cycle==round){
			cycle = 0;
			wpc = rand()%(round-3)+1;
			if(rand()%2) cb *= -1;
		}
		if(xp[1]+20<=0){
			xp[1] = consp;
			if(cycle==wpc+1)
				wall[1].rect.y += 80;
			else if(cycle==wpc+2)
				wall[1].rect.y -= 80;
		}
		if(xp[2]+20<=0){
			if(cycle==wpc){
				xp[2] = xp[1];
			}
			else if(cycle==wpc+1){
				xp[2] = consp + WIN_WIDTH/2;
				wall[2].rect.y -= 80;
			}
			else if(cycle==wpc+2){
				xp[2] = consp;
				wall[2].rect.y += 80;
			}
			else
				xp[2] = consp;
		}
		if(xp[3]+20<=0){
			cycle++;
			if(cycle==wpc){
				wall[3].rect.y -= cb;
			}
			if(cycle==wpc+1){
				wall[3].rect.y += cb;
			}
			xp[3] = consp;
		}
		
		if(x_pos_bg1 <= 0){
			x_pos_bg2 = WIN_WIDTH+x_pos_bg1;
		}
		if(x_pos_bg2 <=0){
			x_pos_bg1 = WIN_WIDTH+x_pos_bg2;
		}
		//----------conversion into rectangle Axis--------//
		heli.rect.y = (int) yph;
		for(int i=1;i<4;i++) wall[i].rect.x = (int) xp[i];
		bg[0].rect.x = (int) x_pos_bg1;
		bg[1].rect.x = (int) x_pos_bg2;

		//---------------Collision & Game Over-----------//
		end_time = SDL_GetTicks();
		if(timerOpt==1)
			if((end_time-start_time)>=120000)
				close = 0;
		le = SDL_GetTicks();
		if(le-ls>5000 || close==3){
			if(heli.rect.y<=0 || heli.rect.y+heli.rect.h>=WIN_HEIGHT-42
			||collision(wall[1],heli)||collision(wall[2],heli)||collision(wall[3],heli)){
				close--;
				crash.rect = {heli.rect.x+(heli.rect.w/2),heli.rect.y-10,70,70};
				render(1,crash);
				SDL_RenderPresent(rend);
				if(effectState==1)Mix_HaltChannel(-1);
				SDL_Delay(3000);
				if(effectState==1)Mix_PlayChannel(-1,effect,-1);
				SDL_PumpEvents();
				SDL_FlushEvent(SDL_KEYDOWN);
				yph = (WIN_HEIGHT-42-50)/2;
				ls = SDL_GetTicks();
			}
		}
		else{
			score_s = SDL_GetTicks();
		}
		//--------------------------Text-------------------//
		speed.tex = intTotext(speed.num,font);
		if(km>=5*1280){
			score.num += 25;
			km -= 5*1280;
			SDL_DestroyTexture(distance.tex);
			distance.num++;
			distance.tex = intTotext(distance.num,font);
		}
		if(speed.num>=140 && s_flag){
			score_s = SDL_GetTicks();
			s_flag = 0;
		}
		if(speed.num>=140){
			score_e = SDL_GetTicks();
			if(score_e-score_s>1000){
				score.num += speed.num/10;
				score_s = SDL_GetTicks();
			}

		}		
		if(speed.num<140){
			s_flag = 1;			
		}
		score.tex = intTotext(score.num,font);
		//----------------Rendering-------------------------------------//

		SDL_RenderClear(rend);
		render(10,bg[0],bg[1],wall[1],wall[2],wall[3],heli,distance,speed,score,
			score_text);

		for(int i=0;i<close;i++){
			if(!i) lives.num = 0;
			lives.rect.x = lives.num;
			render(1,lives);
			lives.num += 40;
		} 
		if(timerOpt==1){
			timer.num  = 120-(end_time-start_time)/1000;
			timer.tex = intTotext(timer.num,font);
			render(1,timer);
		}
		SDL_RenderPresent(rend);

			//------------Position changing----------------//
		x_pos_bg1 -= SCROLL_SPEED;
		x_pos_bg2 -= SCROLL_SPEED;
		for(int i=1;i<4;i++) xp[i] -= SCROLL_SPEED;
		km += SCROLL_SPEED;	
		SDL_Delay(1000/60);

		destroyTex(3,timer,speed,score);
	}
	
	destroyTex(13,bg[0],bg[1],wall[1],wall[2],wall[3],heli,lives,crash,timer,distance,
		speed,score,score_text);
	
	if(effectState==1)Mix_HaltChannel(-1);
	Mix_FreeChunk(effect);
	if(Mix_PausedMusic()==1)
		Mix_ResumeMusic();
	return score.num;
}


int GameOver(int s, int high){
	TTF_Font *font = TTF_OpenFont("img/num.ttf",190);

	obj gameOver,score,highscore;
	gameOver.tex = loadTex("img/gameover.jpg");
	gameOver.rect = {0,0,WIN_WIDTH,WIN_HEIGHT};
	score.rect = {430,310,400,200};
	score.tex = intTotext(s,font);
	highscore.rect = {1050,255,100,35};
	highscore.tex = intTotext(high,font);
	
	while(true){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				return 2;
			}
		}
	 	SDL_RenderClear(rend);
		render(3,gameOver,score,highscore);
		SDL_RenderPresent(rend);

		if(mouseClick(370,540,616,657)) return 1;
		if(mouseClick(794,964,611,657)) return 2;

		SDL_Delay(1000/60);			
	}
	destroyTex(3,gameOver,score,highscore);
	return 0;
}

int highScore(int score){
	int high=0,j=1;
	FILE *file = fopen("file/highscore.bin", "rb");
	if(file==NULL){
		high = 0;
	}
	else{
		fread(&high,sizeof(high),1,file);
	}
	if(high<score){
		high = score;
		file = fopen("file/highscore.bin", "wb");
		fwrite(&high,sizeof(high),1,file);
	}
	fclose(file);
	return high;	
}

void open_tutorial(){
	TTF_Font *font = TTF_OpenFont("img/Demonized.ttf",35);
	char tutorial_str[5000];
	std::string s = "";

	FILE *file = fopen("file/tutorial.bin", "rb");
	fread(tutorial_str,sizeof(tutorial_str),1,file);
	fclose(file);
	
	for(int i=0;i<strlen(tutorial_str);i++){
		s += tutorial_str[i];
	}
	
	obj tutorial,bg;
	bg.tex = loadTex("img/white.png");
	bg.rect = {0,0,WIN_WIDTH,WIN_HEIGHT};
	tutorial.rect = {100,50,1100,600};
	surface = TTF_RenderText_Blended_Wrapped(font,tutorial_str,textColor,1100);
	errorS();
	tutorial.tex = SDL_CreateTextureFromSurface(rend,surface);
	SDL_FreeSurface(surface);
	errorT(tutorial.tex);

	SDL_RenderClear(rend);
	render(2,bg,tutorial);
	SDL_RenderPresent(rend);

	int close = 0;
	while(!close){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				close = 1;
			}
		}
	}
	SDL_DestroyTexture(tutorial.tex);
}

void createOption(){
	TTF_Font *font = TTF_OpenFont("img/Demonized.ttf",30);

	obj sound,mus,comment,bg;
	bg.tex = loadTex("img/white.png");
	bg.rect = {0,0,WIN_WIDTH,WIN_HEIGHT};
	sound.rect = mus.rect = {WIN_WIDTH/2-175,100,250,40};
	sound.tex = showText("Sound: ",font);
	mus.tex = showText("Music: ",font);
	comment.tex = showText("Click on ON or OFF to change settings",font);
	comment.rect = {20,10,500,40};
	mus.rect.y += 50;

	SDL_Texture *on,*off;
	on = showText("ON",font);
	off = showText("OFF",font);

	SDL_Rect x1,x2;
	x2 = x1 = {WIN_WIDTH/2+175,100,50,40};
	x2.y += 50;

	int close = 0;
	while(!close){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				close = 1;
			}
			if(mouseClick(WIN_WIDTH/2+175,WIN_WIDTH/2+225,100,140)){
			effectState *= -1;
			sound.num *= -1;
			} 
			if(mouseClick(WIN_WIDTH/2+170,WIN_WIDTH/2+230,140,200)){
				musicState *= -1;
				if(Mix_PlayingMusic()==1)
					Mix_HaltMusic();
				else
					Mix_PlayMusic(music,-1);
			} 
		}	
		SDL_RenderClear(rend);
		render(4,bg,mus,comment,sound);

		if(effectState==1)
			SDL_RenderCopy(rend,on,NULL,&x1);
		else 
			SDL_RenderCopy(rend,off,NULL,&x1);
		if(musicState==1)
			SDL_RenderCopy(rend,on,NULL,&x2);
		else	
			SDL_RenderCopy(rend,off,NULL,&x2);
		SDL_RenderPresent(rend);

		SDL_Delay(1000/60);	
	}
	destroyTex(3,mus,comment,sound);
	SDL_DestroyTexture(on);
	SDL_DestroyTexture(off);
}
