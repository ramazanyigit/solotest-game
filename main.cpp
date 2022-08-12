#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include <windows.h>

int quit = 0;
const int SCREEN_W     = 640;
const int SCREEN_H     = 640;
const int SCREEN_BPP   = 32;
static SDL_Surface* GM_Screen = NULL;
SDL_Event GM_Event;

int tick1,tick2;

static void GM_Init();
static void GM_Draw();
static SDL_Surface* GM_LoadIMG(char*);
static void GM_ShowSurface(SDL_Surface*,SDL_Surface*,int,int);
static void GM_KeyControl();
static void GM_Close();
void PiyonKontrol();

SDL_Surface* Content[4096];
int menuselected = 0;
int menutotaloption = 0;
int menuactive = 0;
char* MenuNames[512];
int Squares[7][7];
int sesqx=-1,sesqy=-1;
int PiyonSayi=32;
int finished = 0,finished2 = 0;

int main(int argc, char **argv) {
    GM_Init();
    while(!quit) {
        GM_KeyControl();
        GM_Draw();
    }
    GM_Close();
}

static void GM_Init() {
    if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
        MessageBox(0,"SDL Grafik Kütüphanesi yüklenemedi!","Hata",MB_OK);
        exit(1);
    }
    
    /*if(TTF_Init() == -1) {
        MessageBox(0,"SDL_TTF yüklenemedi!","Hata",MB_OK);
        exit(1);         
    }*/
    
    GM_Screen = SDL_SetVideoMode( SCREEN_W,SCREEN_H,SCREEN_BPP,SDL_SWSURFACE | SDL_DOUBLEBUF);
    if(GM_Screen == NULL) {
        MessageBox(0,"SDL Grafik Ekraný oluþturulamadý!","Hata",MB_OK);
        exit(1);             
    }
    SDL_WM_SetCaption("SoloTest 1.0.001 Alpha","");
    
    // Init Codes
    //Content[0] = GM_LoadIMG("./res/startscreen.png");
    //font = TTF_OpenFont("./res/Market_Deco.ttf",24);
    //menuactive = 1;
    Content[0] = GM_LoadIMG("./res/empty.png");
    Content[1] = GM_LoadIMG("./res/full.png");
    Content[2] = GM_LoadIMG("./res/full2.png");
    Content[3] = GM_LoadIMG("./res/empty2.png");
    Content[10] = GM_LoadIMG("./res/background.png");
    
    for(int i=0; i < 7; i++) {
        for(int x=0; x < 7; x++) {
			Squares[i][x] = 1;
		}
    }
    
    Squares[0][0] = Squares[0][1] = Squares[1][0] = Squares[1][1] = -1;
    Squares[0][5] = Squares[1][5] = Squares[0][6] = Squares[1][6] = -1;
    Squares[5][0] = Squares[6][0] = Squares[5][1] = Squares[6][1] = -1;
    Squares[5][5] = Squares[6][5] = Squares[6][6] = Squares[5][6] = -1;
    Squares[3][3] = 0;
    PiyonSayi=32;
}

int MenuAddItem(char *str) {
    MenuNames[menutotaloption] = str;
    menutotaloption++;    
}

static void GM_Draw() {
    if( SDL_Flip(GM_Screen) == -1 ) {
		MessageBox(0,"Uygulamada bir problem oluþtu. Uygulama durduruldu!", "Hata", MB_OK);
        exit(1);    
    }
    // Draw Codes
    GM_ShowSurface(Content[10],GM_Screen,0,0);
    for(int i=0; i < 7; i++) {
             for(int x=0; x < 7; x++) {
                     if(Squares[i][x] != -1) GM_ShowSurface(Content[Squares[i][x]],GM_Screen,40+(80*i),40+(80*x));
             }
    }
    
    if(finished2 == 1 && SDL_GetTicks() - tick1 > 1000) {
        std::stringstream msg; std::string test;
        msg << "Oyun sona erdi!\n" << "Kalan Piyon:" << PiyonSayi;
        test = msg.str();
        const char * destPtr = test.c_str();
		MessageBox(0,destPtr,"Bitti",MB_OK);
		for(int i=0; i < 7; i++) {
			for(int x=0; x < 7; x++) {
				Squares[i][x] = 1;
			}
		}
		
		Squares[0][0] = Squares[0][1] = Squares[1][0] = Squares[1][1] = -1;
		Squares[0][5] = Squares[1][5] = Squares[0][6] = Squares[1][6] = -1;
		Squares[5][0] = Squares[6][0] = Squares[5][1] = Squares[6][1] = -1;
		Squares[5][5] = Squares[6][5] = Squares[6][6] = Squares[5][6] = -1;
		Squares[3][3] = 0;       
		sesqx = sesqy = -1;
		PiyonSayi=32;
		finished = 0;
        finished2 = 0;
    }
    
    /*if(menuactive == 1) {
              GM_ShowSurface(Content[0],GM_Screen,0,0);
              SDL_Color color1 = { 255, 255, 255 }; 
              SDL_Color color2 = { 255, 255, 0 }; 
              int i = 0;
              for(i=0; i < menutotaloption; i++) SDL_Surface* text = TTF_RenderText_Solid(font, MenuNames[i], i == menuselected ? color2 : color1);
              GM_ShowSurface(text,GM_Screen,(GM_Screen->w - text->w) / 2, ((GM_Screen->h - text->h) / 2) - 100);
    }*/
}

static void GM_KeyControl() {
    SDL_EnableKeyRepeat(0,0);
    SDL_PollEvent(&GM_Event);
    if( GM_Event.type == SDL_QUIT ) quit = 1;
    else if( GM_Event.type == SDL_KEYUP ) {
		switch( GM_Event.key.keysym.sym ) {
			default: { break; }
		}
	} else if( GM_Event.type == SDL_MOUSEBUTTONUP ) {
        //Get the mouse offsets
       int x,y;
        x = GM_Event.button.x;
        y = GM_Event.button.y;
		if(x >= 40 && y >= 40 && x <= 600 && y <= 600 && GM_Event.button.button == SDL_BUTTON_LEFT) {
			int sqx,sqy;
			sqx = (x-40)/80;
			sqy = (y-40)/80;
			if(Squares[sqx][sqy] == 1) { 
				if(sesqx != -1 || sesqy != -1) { 
					Squares[sesqx][sesqy] = 1;
				} 
				sesqx = sqx; sesqy = sqy; 
				Squares[sqx][sqy] = 2;
			} else if(Squares[sqx][sqy] == 2) { 
				Squares[sqx][sqy] = 1; 
				sesqx = sesqy = -1; 
			}
			else if(Squares[sqx][sqy] == 3) {
				if(sesqx+2 == sqx && sesqy == sqy) { Squares[sesqx+1][sesqy] = 0; }
				else if(sesqx == sqx && sesqy+2 == sqy) { Squares[sesqx][sesqy+1] = 0; }
				else if(sesqx-2 == sqx && sesqy == sqy) { Squares[sesqx-1][sesqy] = 0; }
				else if(sesqx == sqx && sesqy-2 == sqy) { Squares[sesqx][sesqy-1] = 0; }
				Squares[sesqx][sesqy] = 0;
				Squares[sqx][sqy] = 1;
				sesqx = sesqy = -1;
                PiyonSayi--;    
			}
		} else if(GM_Event.button.button == SDL_BUTTON_RIGHT) {
			if(sesqx != -1 || sesqy != -1) { 
				Squares[sesqx][sesqy] = 1;
				sesqx = sesqy = -1;
			}
        }
		
        PiyonKontrol();
    }
}

void PiyonKontrol() {
     finished = 1;
	for(int i=0; i < 7; i++) {
		for(int x=0; x < 7; x++) {
			if(Squares[i][x] == 3) Squares[i][x] = 0;
			/*if(Squares[i][x] == 1) {
				if(Squares[i+1][x] == 1 && ( Squares[i+2][x] == 0 || Squares[i+2][x] == 3)) { if(i+2 >= 0 && x >= 0 && i+2 <= 6 && x <= 6) finished = 0; }  
				if(Squares[i][x+1] == 1 && ( Squares[i][x+2] == 0 || Squares[i][x+2] == 3)) { if(i >= 0 && x+2 >= 0 && i <= 6 && x+2 <= 6) finished = 0; }  
				if(Squares[i-1][x] == 1 && ( Squares[i-2][x] == 0 || Squares[i-2][x] == 3)) { if(i-2 >= 0 && x >= 0 && i-2 <= 6 && x <= 6) finished = 0; }  
				if(Squares[i][x-1] == 1 && ( Squares[i][x-2] == 0 || Squares[i][x-2] == 3)) { if(i >= 0 && x-2 >= 0 && i <= 6 && x-2 <= 6) finished = 0; }    
			}*/
		}
	}

	if(sesqx != -1 && sesqy != -1) {
		if(Squares[sesqx+1][sesqy] == 1 && Squares[sesqx+2][sesqy] == 0) { 
			if(sesqx+2 >= 0 && sesqy >= 0 && sesqx+2 <= 6 && sesqy <= 6)
			Squares[sesqx+2][sesqy] = 3;
		}  
		if(Squares[sesqx][sesqy+1] == 1 && Squares[sesqx][sesqy+2] == 0) {
			if(sesqx >= 0 && sesqy+2 >= 0 && sesqx <= 6 && sesqy+2 <= 6)
				Squares[sesqx][sesqy+2] = 3;
			}  
		if(Squares[sesqx-1][sesqy] == 1 && Squares[sesqx-2][sesqy] == 0) { 
			if(sesqx-2 >= 0 && sesqy >= 0 && sesqx-2 <= 6 && sesqy <= 6)
				Squares[sesqx-2][sesqy] = 3;
			}  
		if(Squares[sesqx][sesqy-1] == 1 && Squares[sesqx][sesqy-2] == 0) { 
			if(sesqx >= 0 && sesqy-2 >= 0 && sesqx <= 6 && sesqy-2 <= 6) 
				Squares[sesqx][sesqy-2] = 3;
		}    
	} else {
     
	for(int i=0; i < 7; i++) {
		for(int x=0; x < 7; x++) {
			if(Squares[i][x] == 1) {
				if(Squares[i+1][x] == 1 && ( Squares[i+2][x] == 0 || Squares[i+2][x] == 3)) { 
					if(i+2 >= 0 && x >= 0 && i+2 <= 6 && x <= 6)
						finished = 0; 
				}
				if(Squares[i][x+1] == 1 && ( Squares[i][x+2] == 0 || Squares[i][x+2] == 3)) { 
					if(i >= 0 && x+2 >= 0 && i <= 6 && x+2 <= 6)
						finished = 0;
				}  
				if(Squares[i-1][x] == 1 && ( Squares[i-2][x] == 0 || Squares[i-2][x] == 3)) {
					if(i-2 >= 0 && x >= 0 && i-2 <= 6 && x <= 6)
						finished = 0; 
				}  
				if(Squares[i][x-1] == 1 && ( Squares[i][x-2] == 0 || Squares[i][x-2] == 3)) {
					if(i >= 0 && x-2 >= 0 && i <= 6 && x-2 <= 6)
						finished = 0;
				}    
			}
		}
	}
     
	if(finished == 1) {
        finished2 = 1;
        tick1 = SDL_GetTicks();
	}
}
}

static void GM_Close() {
	//TTF_CloseFont(font);
	//TTF_Quit();
	SDL_Quit();
	exit(0);       
}

static SDL_Surface* GM_LoadIMG(char* src) {
	SDL_Surface* imgtemp = NULL; SDL_Surface* imgretn = NULL;
	imgtemp = IMG_Load(src);
	if(imgtemp != NULL) {
		imgretn = SDL_DisplayFormatAlpha(imgtemp);
        SDL_FreeSurface(imgtemp);          
	}
    return imgretn;
}

static void GM_ShowSurface(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
	SDL_Rect the_rect;
	the_rect.x = x;
	the_rect.y = y;
	SDL_BlitSurface(src, NULL, dst, &the_rect);
}
