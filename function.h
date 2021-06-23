#ifndef FUNCTION_H
#define FUNCTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include <string>
#include "var.h"


bool init();
void stop();
void errorS();
void errorT(SDL_Texture *tex);
void applyDisplay(SDL_Texture *x);
float max(float x, float y);
float min(float x, float y);
SDL_Texture* loadTex(std::string path);
SDL_Texture* showText(std::string path, TTF_Font *font);
SDL_Texture* intTotext(int num, TTF_Font *font);
int mouseHover(int x,int y,int x1,int x2,int y1,int y2);
bool collision(obj R, obj S);
int mouseClick(int x1, int x2, int y1, int y2);
void render(int n,...);
void destroyTex(int n,...);

#endif