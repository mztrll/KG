#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "stb-master/stb_easy_font.h"
#include "menu.h"
#include "textur.h"

using namespace std;

GLuint texture;
GLuint sprite;
GLuint background;

int width = 1080;
int height = 720;
float gravity = 0.3;
float speed = 2.5;
float currentframe=2;
float TypeOfAnim=0;
bool directional;
bool gameState=0;
float chSiX;
float chSiY;
int n=0;
float H=32;
float W=32;



string TileMap[34] = {
    /*12345678901234567890123456789012*/
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                B",
    "B                                B",
    "B                                B",
    "BBBBBB          BBBBB     BBBBBBBB",
    "B                                B",
    "B                                B",
    "B                                B",
    "B                BBBBBBBB        B",
    "B                                B",
    "BBBBBBBBBBBBB                    B",
    "B                                B",
    "B                                B",
    "B               BBBBBBBB         B",
    "B                                B",
    "B                                B",
    "B                   BBBBBBBB     B",
    "B                                B",
    "B   BBBBBBBB                     B",
    "B                                B"
};


typedef struct
{
    float left, right, top, bottom, textX, textY;
} sritCoord;

typedef struct
{
    float x, y, dx, dy;
    const float HERO_HEIGHT = 60.0f;
    const float HERO_WIDTH = 40.0f;
} Hero;

Hero pers;
sritCoord vrtcoord;
sritCoord BackGroundCoord;

void Hero_Init(Hero *obj, float x1, float y1, float dx1, float dy1)
{
    obj->x=x1;
    obj->y=y1;
    obj->dx=dx1;
    obj->dy=dy1;
}

void Reflect (float *da, float *a, bool cond, float wall)
{
    if (!cond) return;
    *da*=-0;
    *a=wall;
}

void Hero_Move(Hero *obj)
{
    obj->y+=obj->dy;
    obj->x+=obj->dx;
    Reflect(&obj->dy, &obj->y, obj->y<0,0);
    Reflect(&obj->dx,&obj->x, obj->x<0, 0);
    Reflect(&obj->dx,&obj->x, obj->x>width-chSiX-70, width-chSiX-70);
    obj->dy-=gravity;
    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TypeOfAnim=2;
        currentframe+=0.15;
        obj->dx-=speed;
        obj->x+=obj->dx;
        obj->dx = 0;
        if (currentframe>8) currentframe-=7;
        directional=1;
    }

    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TypeOfAnim=2;
        currentframe+=0.15;
        obj->dx+=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=7;
        directional=0;
    }

    if (GetKeyState(VK_UP)<0 && (obj->y<700) && gameState==1)
    {
        TypeOfAnim=1;
        obj->dy =speed*1.2;
        currentframe+=0.15;
        if (currentframe>8) currentframe-=7;
        obj->y+=obj->dy;
    }
}




void Sprite_animation(GLuint& texture, int n, Hero *obj)
{
    static float svertix[]= {0,0, 80,0, 80,80, 0,80};
    static float TexCord[]= {0,0, 0.12,0, 0.12,1, 0,1};
    static float line[]= {0,0, 80,0, 80,80, 0,80};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.7);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static float spriteXsize=800;
    static float spriteYsize=240;
    static float charsizey=80;
    static float charsizex=100;
    vrtcoord.left=(charsizex*n)/spriteXsize;
    vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
    vrtcoord.top=(charsizey*TypeOfAnim)/spriteYsize;
    vrtcoord.bottom=vrtcoord.top+(charsizey/spriteYsize);

    TexCord[1] = TexCord[3] = vrtcoord.bottom;
    TexCord[5] = TexCord[7] = vrtcoord.top;

    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }
    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }




    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_ALPHA_TEST);



    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    chSiX=charsizex;
    chSiY=charsizey;

}



void Main_Init()
{
    Hero_Init(&pers, width/2.0,32,0,0);
    Load_Texture("CyberChel.png", &sprite, GL_CLAMP, GL_CLAMP, GL_NEAREST);
    Load_Texture("b3.jpg", &background, GL_REPEAT, GL_CLAMP, GL_NEAREST);
}

void Menu_Init(){
    Menu_AddButton("Start",0,0,100,30,2);
    Menu_AddButton("Stop",100,0,100,30,2);
    Menu_AddButton("Quit",200,0,100,30,2);
}




void Show_Background(GLuint texture)
{
    static float svertix[]= {-1080,0, -1080,700, 1080*2,700, 1080*2,0};
    static float TexCord[]= {-1,1, -1,0, 2,0, 2,1 };

    glClearColor(0,0,0,0);

    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if(buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    switch (buttonId)
    {
    case 0:
        if (gameState==0) gameState=1;
        break;
    case 1:
        if (gameState==1) gameState=0;
        break;
    case 2:
        PostQuitMessage(0);
        break;

    }
}



void Collision(Hero& hero)
{
    const float tile_size = 31.5f;

    float left   = hero.x;
    float right  = hero.x + hero.HERO_WIDTH;
    float top    = hero.y;
    float bottom = hero.y + hero.HERO_HEIGHT;


    int left_tile   = (int)(left / tile_size);
    int right_tile  = (int)(right / tile_size);
    int top_tile    = (int)(top / tile_size);
    int bottom_tile = (int)(bottom / tile_size);


    for (int i = top_tile; i <= bottom_tile; i++)
    {
        for (int j = left_tile; j <= right_tile; j++)
        {
            if (TileMap[i][j] == 'B')
            {

                float tile_left   = j * tile_size;
                float tile_right  = tile_left + tile_size;
                float tile_top    = i * tile_size;
                float tile_bottom = tile_top + tile_size;


                if (bottom > tile_top && top < tile_bottom)
                {

                    if (hero.dy > 0 && bottom - hero.dy <= tile_top)
                    {
                        hero.dy = 0;
                        hero.y = tile_top - hero.HERO_HEIGHT;
                    }

                    else if (hero.dy < 0 && top - hero.dy >= tile_bottom)
                    {
                        hero.dy = 0;
                        hero.y = tile_bottom;
                    }
                }


                if (right > tile_left && left < tile_right)
                {


                    if (left < tile_right && hero.dy < 0)
                    {
                        hero.dx = 0;
                        hero.x = tile_right;
                    }


                    else if (right > tile_left && hero.dx + speed > 0 && hero.dy < 0)
                    {
                        hero.dx = 0;
                        hero.x = tile_left;
                    }
                }
            }
        }
    }
}

void draw_map()
{
    const float tsize = 31.5f;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < TileMap[i].length(); j++)
        {
            if (TileMap[i][j] == 'B')
            {

                float x1 = j * tsize + tsize;
                float y1 = i * tsize + tsize;
                float x2 = j * tsize + tsize;
                float y2 = i * tsize;
                float x3 = j * tsize;
                float y3 = i * tsize;
                float x4 = j * tsize;
                float y4 = i * tsize + tsize;


                float tile[] = { x1, y1, x2, y2, x3, y3, x4, y4 };


                glVertexPointer(2, GL_FLOAT, 0, tile);
                glEnableClientState(GL_VERTEX_ARRAY);

                glColor3f(1.0f, 0.2f, 0.2f);

                glDrawArrays(GL_QUADS, 0, 4);
                glDisableClientState(GL_VERTEX_ARRAY);

            }
            else if (TileMap[i][j] == ' ')
            {
                continue;
            }
        }
    }
}


void Hero_Show(Hero *obj)
{
    glPushMatrix();
    glTranslatef(obj->x,obj->y,0);
    Sprite_animation(sprite, currentframe, &pers);
    glPopMatrix();
    Collision(*obj);
    draw_map();
}



#endif // MAIN_H_INCLUDED
