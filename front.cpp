#pragma once

#include"global_variable.cpp"

//int Map[set.map_height+1][set.map_width+1]={0,};

int py;//��Map��playerλ��Ϊ(player.x , py) 
void render_player(struct player* pla,struct settings* set)
{
	py = set->map_height - pla->y + set->line;
	while(py< set->map_height/2){
		
		set->line++;py=set->map_height - pla->y + set->line;
	}//������λ�ó�����Ļ����֮һ����Ļ���� 
}

void show(struct player* pla,struct settings* set)//��ӡ�����ң�ÿ��ˢ�� 
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
		{	case 1:for(int k=1;k<=set->map_board_length;k++){printf("=");}	break;
			case 2:for(int k=1;k<=set->map_board_length;k++){printf("=");}	break;
			case 3:for(int k=1;k<=set->map_board_length;k++){printf("=");}	break;
			case 4:for(int k=1;k<=set->map_board_length;k++){printf("=");}	break;
		}
		
	}
	
	for(int i=pla->x-set->player_width/2;i<=pla->x+set->player_width/2;i++)
	for(int j=py-set->player_height+1;j<=py;j++)
	{gotoxy( i,j);printf("*");
	}
}