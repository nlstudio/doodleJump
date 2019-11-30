#pragma once

#include"global_variable.cpp"

//int Map[set.map_height+1][set.map_width+1]={0,};
int times=0;
int py;//在Map中player位置为(player.x , py) 
int render_player(struct player* pla,struct settings* set)
{	int flag=0;
	py = set->map_height - pla->y + set->line;
	while(py< set->map_height/2){
		set->line++;
		py=set->map_height - pla->y + set->line;
		flag++;
	}//如果玩家位置超过屏幕二分之一，屏幕上移 
	return flag;
}

void show_board(struct player* pla,struct settings* set)//打印板
{	for(int i=1;i<=set->map_height;i++)
	{	gotoxy(1,i);
		for(int j=1;j<=set->map_width;j++)
		{	printf(" ");
		}
	}
	for(int i=1;i<=set->map_height;i++)
	for(int j=1;j<=set->map_width;j++)
	{	gotoxy(j,i);
		switch(Map[i][j])
		{	case 1:printf("=");	break;
			case 2:printf("=");	break;
			case 3:printf("=");	break;
			case 4:printf("=");	break;
		}
	}
}
void show_player(struct player* pla,struct settings* set)//打印玩家，
{	static int last_x,last_y;
	if(times)
	{	for(int i=last_x-set->player_width/2;i<=last_x+set->player_width/2;i++)
		for(int j=last_y-set->player_height+1;j<=last_y;j++)
		{gotoxy( i,j);
			if(Map[j][i]){printf("=");}
			else {printf(" ");	}
		}
	}
	times++;
	
	for(int i=pla->x-set->player_width/2;i<=pla->x+set->player_width/2;i++)
	for(int j=py-set->player_height+1;j<=py;j++)
	{gotoxy( i,j);printf("*");
	}
	last_x=pla->x;
	last_y=py;
}
