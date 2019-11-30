#pragma once
#include "global_variable.cpp"

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
    //设置新的缓冲区为活动显示缓冲
    SetConsoleActiveScreenBuffer(hOutBuf);
}
