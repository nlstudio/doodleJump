#pragma once
#include "global_variable.cpp"
#include "front_end.h"
#include "back_end.h"
#include <Windows.h>
#include<MMSystem.h>
#pragma comment(lib,"winmm.lib")
#include <conio.h>
#include <string.h>
//int Map[set.map_height+1][set.map_width+1]={0,};
int times = 0;
int py;		//在Map中player位置为 (player.x, py) 
void print_score(HANDLE handle) { return; }
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
	print_frame(set,hOutBuf);
	for(int i=pla->x-set->player_width/2;i<=pla->x+set->player_width/2;i++)
	for(int j=py-set->player_height+1;j<=py;j++)
	{   Map[j][i]='*';
	}
	
	for (int i = 1; i <=set->map_height; i++)
    {
        coord.Y = i;coord.X = 1;
        WriteConsoleOutputCharacterA(hOutBuf, Map[i], set->map_width, coord, &bytes);
    }
    
    coord.Y = 1;coord.X = set->map_width-20;
    
    char s[16]={"your score:"};
    /*static int score;
	score=(pla->y > score ? pla->y:score);*/
	s[12] = (set->line / 1000 == 0 ? 32 : 48 + set->line / 1000);
	s[13] = ((set->line / 100) < 1 ? 32 : 48 + (set->line / 100)%10);
	s[14] = (char)(48 + (set->line % 100) / 10);
	s[15] = (char)(48 + set->line % 10);
	WriteConsoleOutputCharacterA(hOutBuf, s, 16, coord, &bytes);
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
	char s[16]={"your score:"};
    int score=pla->y+1;
    s[12]=(score/1000 == 0?32 : 48+ score/1000 );
	s[13]=((score/100) <1?32 : 48+ (score/100)%10 );
	s[14]=(char)(48+(score%100)/10);
	s[15]=(char)(48+score%10); 
	WriteConsoleOutputCharacterA(hOutBuf, s, 16, coord, &bytes);
	
	coord.X = set->map_width/3;coord.Y = set->map_height/2+2;
	WriteConsoleOutputCharacterA(hOutBuf, "按ENTER键继续", 14, coord, &bytes);

	SetConsoleActiveScreenBuffer(hOutBuf);
	//设置新的缓冲区为活动显示缓冲
	
	PlaySound("a.wav",NULL,SND_FILENAME | SND_ASYNC);
	Sleep(3700); 
	PlaySound(NULL,NULL,SND_FILENAME | SND_ASYNC);
}

void intro()
{
	char s[13][60]={"Description","把红旗插上无穷的高峰！","向上跳跃，不要坠入虚空！","Howto play ","用方向键操纵小人左右移动，当小人落在跳板上时会向上弹起。","左移： A 或 ←","右移： D 或 →","Main Contributors","@lhy ：Backend Developer","@lz ：Frontend Developer","@Jonbgua ：Program Manager, Software Architect"};
	HANDLE hOut;
	hOut = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
	CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOut, &cci);
    coord.X = 3;
	coord.Y = 3;
    WriteConsoleOutputCharacterA(hOut, s[0], 50, coord, &bytes);
    coord.Y = 5;
    WriteConsoleOutputCharacterA(hOut, s[1], 50, coord, &bytes);
    coord.Y = 6;
    WriteConsoleOutputCharacterA(hOut, s[2], 50, coord, &bytes);
    coord.Y = 10;
    WriteConsoleOutputCharacterA(hOut, s[3], 50, coord, &bytes);
    coord.Y = 12;
    WriteConsoleOutputCharacterA(hOut, s[4], 60, coord, &bytes);
    coord.Y = 13;
    WriteConsoleOutputCharacterA(hOut, s[5], 50, coord, &bytes);
    coord.Y = 14;
    WriteConsoleOutputCharacterA(hOut, s[6], 50, coord, &bytes);
    coord.Y = 18;
    WriteConsoleOutputCharacterA(hOut, s[7], 50, coord, &bytes);
    coord.Y = 20;
    WriteConsoleOutputCharacterA(hOut, s[8], 50, coord, &bytes);
    coord.Y = 21;
    WriteConsoleOutputCharacterA(hOut, s[9], 50, coord, &bytes);
    coord.Y = 22;
    WriteConsoleOutputCharacterA(hOut, s[10], 50, coord, &bytes);
    
    SetConsoleActiveScreenBuffer(hOut);
    fflush(stdin);
    _getch();
    return ;
}

void print_score(struct score* _score,HANDLE hOut)
{	
	char s[10][4] = {0}, a[10][15] = {0};
	for(int i=0;i<10;i++)
	{	strncpy(a[i],(_score+i)->player_name,15);
		int sc=(_score+i)->player_score;
		if (sc == -1) break;
		s[i][0]=(sc/1000==0?32:48+sc/1000);
		s[i][1]=( sc/100 <1?32:48+(sc/100)%10);
		s[i][2]=(char)(48+(sc%100)/10);
		s[i][3]=(char)(48+sc%10);
	}
	
	coord.X = 9;
	coord.Y = 10;
	for(int i=0;i<5;i++)
	{	WriteConsoleOutputCharacterA(hOut, a[i], 15, coord, &bytes);
		coord.Y +=2;
	}
	coord.X = 34;
	coord.Y = 10;
	for(int i=5;i<10;i++)
	{	WriteConsoleOutputCharacterA(hOut, a[i], 15, coord, &bytes);
		coord.Y +=2;
	}
	
    
	coord.X = 23;
	coord.Y = 10;
	for(int i=0;i<5;i++)
	{	WriteConsoleOutputCharacterA(hOut, s[i], 4, coord, &bytes);
		coord.Y +=2;
	}
	coord.X = 48;
	coord.Y = 10;
	for(int i=5;i<10;i++)
	{	WriteConsoleOutputCharacterA(hOut, s[i], 4, coord, &bytes);
		coord.Y +=2;
	}
    
    
	return;
}


void game_rank()
{
	char s[5][60]={" __                    _                         _ ","/ _\\ ___ ___  _ __ ___| |__   ___   __ _ _ __ __| |","\\ \\ / __/ _ \\| '__/ _ \\ '_ \\ / _ \\ / _` | '__/ _` |","_\\ \\ (_| (_) | | |  __/ |_) | (_) | (_| | | | (_| |","\\__/\\___\\___/|_|  \\___|_.__/ \\___/ \\__,_|_|  \\__,_|"};
	HANDLE hOut,hOut2;
	hOut = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOut2 = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOut, &cci);
    SetConsoleCursorInfo(hOut2, &cci);
    coord.X = 5;
	coord.Y = 3;
    WriteConsoleOutputCharacterA(hOut, s[0], 52, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, s[0], 52, coord, &bytes);
    coord.Y = 4;
    WriteConsoleOutputCharacterA(hOut, s[1], 52, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, s[1], 52, coord, &bytes);
    coord.Y = 5;
    WriteConsoleOutputCharacterA(hOut, s[2], 52, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, s[2], 52, coord, &bytes);
    coord.Y = 6;
    WriteConsoleOutputCharacterA(hOut, s[3], 52, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, s[3], 52, coord, &bytes);
    coord.Y = 7;
    WriteConsoleOutputCharacterA(hOut, s[4], 52, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, s[4], 52, coord, &bytes);
    
    
    coord.X = 6;
	coord.Y = 20;
    WriteConsoleOutputCharacterA(hOut, "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _", 50, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut2, "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _", 50, coord, &bytes);

    
    coord.X = 14;
	coord.Y = 23;
    WriteConsoleOutputCharacterA(hOut2, "[ U ]  Upload your score", 25, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut, "[ G ]  Global", 14, coord, &bytes);
	coord.Y = 25;
    WriteConsoleOutputCharacterA(hOut2, "[ L ]  Local", 13, coord, &bytes);
    WriteConsoleOutputCharacterA(hOut, "按 ENTER 返回", 14, coord, &bytes);
    coord.Y = 27;
    WriteConsoleOutputCharacterA(hOut2, "按 ENTER 返回", 14, coord, &bytes);
    
    struct score globle_score[MAX_SCORE_SERVER + 1];
    
	print_score(high_score,hOut);
    l_score:
    {
    	SetConsoleActiveScreenBuffer(hOut);
    	fflush(stdin);
    	char a=_getch();
    	switch(a)
		{
			case 'g':	goto g_score;break;
			case  13:	return;
			default:	break;
		}
    	goto l_score;	
	}
    
    g_score:
    {	download_score(&set, globle_score);
		print_score(globle_score,hOut2);
		SetConsoleActiveScreenBuffer(hOut2);
		fflush(stdin);
    	char d=_getch();
    	switch(d)
		{
			case 'u':	upload_process(&set, high_score, &current_score); break;
			case 'l':	goto l_score;break;
			case  13:	return;
			default:	break;
		}
		goto g_score;	
	}
    
    
    
    
} 
void in_name()
{	char name[20];
	HANDLE hOut;
	hOut = CreateConsoleScreenBuffer(
        GENERIC_WRITE,//定义进程可以往缓冲区写数据
        FILE_SHARE_WRITE,//定义缓冲区可共享写权限
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
	CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 1;
    cci.dwSize = 1;
    SetConsoleCursorInfo(hOut, &cci);


	print_frame(&set,hOut);
	coord.X = 2;coord.Y = 6;
	WriteConsoleOutputCharacterA(hOut, "input your name:",17, coord, &bytes);
	
	SetConsoleActiveScreenBuffer(hOut);
	
	coord.X = 8;coord.Y = 8;
	SetConsoleCursorPosition(hOut, coord);
	SetConsoleTextAttribute(hOut, 12);
	
	fflush(stdin);
	//gets(name);
	scanf("%s", name);
	save_score(high_score, &current_score, name, set.line);

	return ;
}
