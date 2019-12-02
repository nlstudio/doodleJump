#pragma once
#include <Windows.h>
#include "struct_def.h"
HANDLE hOutBuf;//控制台屏幕缓冲区句柄 
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

void print_frame(struct settings* set)
// 游戏开始时，打印边框 
{
	for (int i = 1; i <= set->map_height + 1; i++)
	{
		coord.X = 0; coord.Y = i; WriteConsoleOutputCharacterA(hOutBuf, "|", 1, coord, &bytes);
		coord.X = set->map_width + 1; coord.Y = i; WriteConsoleOutputCharacterA(hOutBuf, "|", 1, coord, &bytes);
	}
	for (int i = 1; i <= set->map_width; i++)
	{
		coord.X = i; coord.Y = 0; WriteConsoleOutputCharacterA(hOutBuf, "_", 1, coord, &bytes);
		coord.X = i; coord.Y = set->map_height + 1; WriteConsoleOutputCharacterA(hOutBuf, "_", 1, coord, &bytes);
	}
}

void render_boards(struct board* head, struct settings* set, char*** Map)
// 将链表生成数组Map 
{
	struct board* p = head;
	for (int i = 1; i <= set->map_height; i++)
		for (int j = 1; j <= set->map_width; j++)
		{
			(*Map)[i][j] = ' ';
		}
	while (p != NULL)
	{
		if (set->map_height - p->line_id + set->line > 0 && set->line <= p->line_id)
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
