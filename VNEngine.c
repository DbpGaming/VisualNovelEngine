#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* Texture;
SDL_Renderer *renderer;
SDL_Rect Resolution;
long lSize;
char* script;
char* activeScript;
char* c;
char line[255];
size_t result;
float messageX = 0.5;
float messageY = 0.9;
TTF_Font* Font;
short scriptNum = 1;
char config[] = "config";
char fontName[255];
char scripts[255];
char windowTitle[255];
char images[255];
char text[255];
char fileError[] = "Error opening file:";
char memoryError[] = "Error with memory:";
char readError[] = "Error reading file:";

void renderimage(SDL_Surface* Surface, float y, float x, float w, float h)
{
	SDL_Rect position;
	position.h = (h/255 * Surface->h) / 1440 * Resolution.h * 2;
	position.y = y/255 * (Resolution.h) - position.h / 2;
	position.w = (w/255 * Surface->w) / 1440 * Resolution.h * 2;
	position.x = (x/255 * Resolution.w)-position.w/2;
	Texture = SDL_CreateTextureFromSurface(renderer, Surface);
	SDL_FreeSurface(Surface);
	SDL_RenderCopy(renderer, Texture, NULL, &position);
}

void error(char* message, char* file, int code)
{
		fputs(strcat(message, file),stderr); 
		exit(code);
}

char* readLine(char fileName[], int LN)
{
	FILE* file = fopen(fileName, "r");
	if (file==NULL)
		error(fileError, fileName, 1);
	for(int i = 0; i < LN; i++)
	{
		fgets(line, sizeof(line), file);
		if (line==NULL)
			error(memoryError, fileName, 2);
		if (line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = '\0';
	}
	printf("Data from the file:%s\n", line);
	for (int i = 0; i < strlen(line); i++)
	{
		if (line[i-1] == 0x5C && line[i] == 0x6e)
		{
			line[i-1] = 0x0D;
			line[i] = 0x0A;
		}
	}
	fclose(file);
	return line;
}

void openScript(int scriptID, char* &buffer)
{
	char* script;
	script = readLine(scripts, scriptID);
	FILE* file = fopen(script,"rb");
	if (file==NULL)
		error(fileError, script, 1);
	fseek (file , 0 , SEEK_END);
	lSize = ftell (file);
	rewind (file);
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer==NULL)
		error(memoryError, script, 2);
	result = fread (buffer,1,lSize,file);
	if (result != lSize)
		error(readError, script, 3);
	fclose (file);
}

char readComm(int &i)
{
	i++;
	return activeScript[i-1];
}

SDL_Surface* TextSurface(TTF_Font* Font, char* Text, char b, char g, char r, int res)
{
	return TTF_RenderText_Blended_Wrapped(Font, Text, {r, g, b, 255}, res);
}

void initValues(){
	Resolution.w = 1280;
	Resolution.h = 720;

	strcpy(fontName, readLine(config, 2));
	strcpy(windowTitle, readLine(config, 3));
	strcpy(scripts, readLine(config, 4));
	strcpy(images, readLine(config, 5));
	strcpy(text, readLine(config, 6));
}

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Font = TTF_OpenFont(fontName, 30);
	SDL_Window *window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Resolution.w, Resolution.h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 128, 255, 255);
	SDL_RenderClear(renderer);	
}

int main(int argc, char *argv[])
{
	initValues();
	init();

	openScript(1, script);
	activeScript = script;
	char r = 1;
	for (int i = 0; i < lSize;)
	{
		if(r){
			switch(readComm(i))
			{
				case 0x00:
					//char filename[255];
					//strcpy(filename, readLine(readLine(text, scriptNum), readComm(i)));
					//renderimage(TextSurface(Font, testing, 255, 255, 255, 720), 200, 128, 255, 255);
					r = 0;
				break;

				case 0x01:
					//renderimage(TextSurface(Font, readLine(readLine(text, scriptNum), readComm(i)), readComm(i), readComm(i), readComm(i), 720), 200, 128, 255, 255);
					r = 0;
				break;

				case 0x02:
					//char filename[255];
					//readLine(images, readComm(i)));
					//renderimage(IMG_Load(filename), 128, 128, 128, 128);
				break;

				case 0xFE:
					openScript(readComm(i), script);
					scriptNum = activeScript[i-1];
					activeScript = script;
					i = 0;
				break;
			}
		}
		else
		{
			SDL_Delay(10000);
			r=1;
			SDL_RenderClear(renderer);	
		}
		SDL_RenderPresent(renderer);
	}
	free (script);
	return 0;
}