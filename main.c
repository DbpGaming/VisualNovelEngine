#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

SDL_Texture* Test;
SDL_Texture* TestText;
SDL_Renderer *renderer;
SDL_Rect Resolution;

void renderimage(char *image, float y, float x, float w, float h) {
	SDL_Rect position;
	position.h = h * Resolution.h;
	position.y = y * (Resolution.h) - position.h / 2;
	position.w = w * Resolution.h;
	position.x = (x * Resolution.w)-position.w/2;
	SDL_Surface* surf = IMG_Load(image);
	Test = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	SDL_RenderCopy(renderer, Test, NULL, &position);
}

void rendertext(char *text, float y, float x, char *font, int fontsize, short r, short g, short b, short a) {
	SDL_Rect position;
	TTF_Font *fonttest = TTF_OpenFont(font, fontsize);
	SDL_Color texcolour = { r, g, b, a };
	SDL_Surface* surf = TTF_RenderText_Blended(fonttest, text, texcolour);
	position.y = (y * Resolution.h) - surf->h/2;
	position.x = (x * Resolution.w) -  surf->w/2;
	TestText = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(TestText, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(renderer, TestText, NULL, &position);
}

void rendertextboxtext(char *text, float y, float x, char *font, int fontsize, short r, short g, short b, short a, int textwidth, float size) {
	SDL_Rect position;
	TTF_Font *fonttest = TTF_OpenFont(font, fontsize);
	SDL_Color texcolour = { r, g, b, a };
	SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(fonttest, text, texcolour, textwidth);
	position.y = (y*Resolution.h)-surf->h/2;
	position.x = ((x*Resolution.h*16/9)+(Resolution.w-Resolution.h*16/9)/2)-surf->w/2;
	TestText = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	SDL_QueryTexture(TestText, NULL, NULL, &position.w, &position.h);
	SDL_RenderCopy(renderer, TestText, NULL, &position);
}

int main(int argc, char *argv[])
{
	
	Resolution.w = 1920;
	Resolution.h = 1080;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_Window *window = SDL_CreateWindow("bah", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Resolution.w, Resolution.h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderClear(renderer);
	renderimage("amane.png", 0.5, 0.25, 0.396, 0.766);
	renderimage("miharu.png", 0.5, 0.75, 0.447, 0.875);
	rendertext("test", 0.1, 0.2, "Arial.ttf", 16, 0xFF, 0xFF, 0xFF, 0xFF);
	rendertext("FUCK YOU BEAR", 0.1, 0.1, "Arial.ttf", 160, 0, 0xFF, 0, 0xFF);
	rendertextboxtext("just a really long ass thing to test wrapping because i need to see how well sdl handles wrapping on its own lol, \nhopefully its decent and hopefully it if it does something like center the text i can change that", 0.9, 0.5, "Arial.ttf", 40, 0, 0xFF, 0, 0xFF, 1280, 0.1);
	SDL_RenderPresent(renderer);
	SDL_Delay(30000);

	return 0;
}

