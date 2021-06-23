#ifndef PLAY_H
#define PLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include "var.h"
#include "function.h"


int runGame();
int GameOver(int s, int high);
int highScore(int score);
void open_tutorial();
void createOption();

#endif