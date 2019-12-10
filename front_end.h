#pragma once
#include <Windows.h>
#include "struct_def.h"
HANDLE hOutput ,hOutBuf;//控制台屏幕缓冲区句柄 
COORD coord = { 0,0 };//双缓冲处理显示 
DWORD bytes = 0;

int get_key_state()
{
	if (GetAsyncKeyState(VK_LEFT)) return -1;
	if (GetAsyncKeyState('A')) return -1;
	if (GetAsyncKeyState(VK_RIGHT)) return 1;
	if (GetAsyncKeyState('D')) return 1;
	return 0;
}

void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos; pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void print_frame(struct settings* set,HANDLE handle)
// 游戏开始时，打印边框 
{
	for (int i = 1; i <= set->map_height + 1; i++)
	{
		coord.X = 0; coord.Y = i;
		WriteConsoleOutputCharacterA(handle, "|", 1, coord, &bytes);
		coord.X = set->map_width + 1;
		coord.Y = i; WriteConsoleOutputCharacterA(handle, "|", 1, coord, &bytes);
	}
	for (int i = 1; i <= set->map_width; i++)
	{
		coord.X = i; coord.Y = 0;
		WriteConsoleOutputCharacterA(handle, "_", 1, coord, &bytes);
		coord.X = i; coord.Y = set->map_height + 1;
		WriteConsoleOutputCharacterA(handle, "_", 1, coord, &bytes);
	}
}

void main_iterface()
{
	char string[12][35]={"    ___                _ _       ","   /   \\___   ___   __| | | ___  ","  / /\\ / _ \\ / _ \\ / _` | |/ _ \\ "," / /_// (_) | (_) | (_| | |  __/ ","/___,' \\___/ \\___/ \\__,_|_|\\___| ","  __   ","  \\ \\ _   _ _ __ ___  _ __  ","   \\ \\ | | | '_ ` _ \\| '_ \\ ","/\\_/ / |_| | | | | | | |_) |","\\___/ \\__,_|_| |_| |_| .__/ ","                     |_|    "};
	for(int i=0;i<=4;i++)
	{	coord.X = 4;coord.Y = i+2;
		WriteConsoleOutputCharacterA(hOutput, string[i], 35, coord, &bytes);
	}
	for(int i=5;i<=11;i++)
	{	coord.X = 26;coord.Y = i+4;
		WriteConsoleOutputCharacterA(hOutput, string[i], 35, coord, &bytes);
	}
	
	coord.X = 10;coord.Y = 18;
	WriteConsoleOutputCharacterA(hOutput, "1.开始游戏", 11, coord, &bytes);
	coord.X = 35;coord.Y = 18;
	WriteConsoleOutputCharacterA(hOutput, "2.游戏说明", 11, coord, &bytes);
	coord.X = 10;coord.Y = 21;
	WriteConsoleOutputCharacterA(hOutput,"3.退出游戏", 11, coord, &bytes);
	coord.X = 35;coord.Y = 21;
	WriteConsoleOutputCharacterA(hOutput,"4.排行榜！", 11, coord, &bytes);
	coord.X = 10;coord.Y = 24;
	WriteConsoleOutputCharacterA(hOutput, "请选择【1 2 3】：【  】", 24, coord, &bytes);
} 

void render_boards(struct board* head, struct settings* set, char*** Map)
// 将链表生成数组Map 
{
	struct board* p = head;
	for (int i = 0; i <= set->map_height; i++)
		for (int j = 0; j <= set->map_width; j++)
		{
			(*Map)[i][j] = ' ';
		}
	while (p != NULL)
	{
		if (set->map_height - p->line_id + set->line > 1 && set->line <= p->line_id)
		{
			for (int j = p->x; j < p->x + set->map_board_length; j++)
			{
				switch (p->type)
				{
				case 1:
					(*Map)[set->map_height - p->line_id + set->line][j] = '=';
					break;
				case 2:
					(*Map)[set->map_height - p->line_id + set->line][j] = '~';
					break;
				case 3:
					(*Map)[set->map_height - p->line_id + set->line][j] = '.';
					break;
				case 4:
					(*Map)[set->map_height - p->line_id + set->line][j] = '#';
					break;
				default:
					break;
				}
			}
		}
		p = p->next;
	}
}

void modeset(int w,int h,HANDLE hOut) {
	COORD size = {w, h};
	SetConsoleScreenBufferSize(hOut,size);
	SMALL_RECT rc = {1,1, w, h};
	SetConsoleWindowInfo(hOut ,true ,&rc);
	return;
}
