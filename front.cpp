#pragma once
#include "global_variable.cpp"
#include "front_end.h"
#include <Windows.h>

//int Map[set.map_height+1][set.map_width+1]={0,};
int times = 0;
int py;		//在Map中player位置为 (player.x, py) 
int render_player(struct player* pla,struct settings* set)
{	
	int flag = 0;
	py = set->map_height - pla->y + set->line;
	// 如果玩家位置超过屏幕二分之一，屏幕上移
	while (py < set->map_height/2)
	{
		set->line++;
		py = set->map_height - pla->y + set->line;
		flag++;
	} 
	return flag;
}

void show(struct player* pla,struct settings* set)
//打印板
{
	for(int i=pla->x-set->player_width/2;i<=pla->x+set->player_width/2;i++)
	for(int j=py-set->player_height+1;j<=py;j++)
	{   Map[j][i]='*';
	}
	
	for (int i = 1; i <=set->map_height; i++)
    {
        coord.Y = i;coord.X = 1;
        WriteConsoleOutputCharacterA(hOutBuf, Map[i], set->map_width, coord, &bytes);
    }
    coord.Y = set->map_height/2;coord.X = set->map_width+5;
    char s[15]={"your score:"};
    /*static int score;
	score=(pla->y > score ? pla->y:score);*/
	s[12] = (set->line / 100 == 0 ? 32 : 48 + set->line / 100);
	s[13] = (char)(48 + (set->line % 100) / 10);
	s[14] = (char)(48 + set->line % 10);
	WriteConsoleOutputCharacterA(hOutBuf, s, 15, coord, &bytes);
	//设置新的缓冲区为活动显示缓冲
    SetConsoleActiveScreenBuffer(hOutBuf);
}

void gameover(struct settings* set,struct player* pla)
{
	for(int i=set->map_height/3+1;i<=set->map_height*2/3+1;i++) 
	{	
	coord.X = 0;coord.Y = i;WriteConsoleOutputCharacterA(hOutBuf, "|", 1, coord, &bytes);
	coord.X = set->map_width+1;coord.Y = i;	WriteConsoleOutputCharacterA(hOutBuf,"|", 1, coord, &bytes);
	}
	for(int i=1;i<=set->map_width;i++) 
	{	coord.X = i;coord.Y =set->map_height/3;WriteConsoleOutputCharacterA(hOutBuf, "_", 1, coord, &bytes);
		coord.X = i;coord.Y = set->map_height*2/3+1;WriteConsoleOutputCharacterA(hOutBuf, "_", 1, coord, &bytes);
	}
	for(int j=set->map_height/3+1;j<=set->map_height*2/3;j++)
	for(int i=1;i<=set->map_width;i++)
	{	coord.X = i;coord.Y = j;
		WriteConsoleOutputCharacterA(hOutBuf, " ", 1, coord, &bytes);
	}

	coord.X = set->map_width/3;coord.Y = set->map_height/2;
	WriteConsoleOutputCharacterA(hOutBuf, "gameover", 9, coord, &bytes);
	
	coord.X = set->map_width/3;coord.Y = set->map_height/2+1;
	char s[15]={"your score:"};
    int score=pla->y+14;
	s[12]=(score/100==0?32:48+score/100);
	s[13]=(char)(48+(score%100)/10);
	s[14]=(char)(48+score%10); 
	WriteConsoleOutputCharacterA(hOutBuf, s, 15, coord, &bytes);
	
	coord.X = set->map_width/3;coord.Y = set->map_height/2+2;
	WriteConsoleOutputCharacterA(hOutBuf, "按ENTER键继续", 14, coord, &bytes);

	SetConsoleActiveScreenBuffer(hOutBuf);
	//设置新的缓冲区为活动显示缓冲
}

