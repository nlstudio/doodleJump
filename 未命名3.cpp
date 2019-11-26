#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
/*int getkeystate()
{
	Sleep(8);
	if( GetAsyncKeyState(VK_LEFT)&0x25 && player.x != 1)return -1;
	if( GetAsyncKeyState('A')&0x41 && player.x != 1)return -1;
	if( GetAsyncKeyState(VK_RIGHT)&0x27 && player.x != set.map_width-1)return 1;
	if( GetAsyncKeyState('D')&0x44 && player.x != set.map_width-1)return 1;
	return 0;
}
*/ 
void gotoxy(int x, int y)
{	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD pos;pos.X=x;pos.Y=y;
	SetConsoleCursorPosition(handle, pos);
}



int Map[set.map_height+1][set.map_width+1]={0,};



int py;//在Map中player位置为(player.x , py) 
void render_player(struct player* pla)
{	py=set.map_height - pla.y + set.line;
	while(py> set.map_height/2){set.line++;	}//如果玩家位置超过屏幕二分之一，屏幕上移 
}

void render_boards()//将链表生成数组Map 
{
	struct board* p=head;
	while(p!=NULL)
	{	for(int j=p->x;j<p->x+set.map_board_length;j++)
		{	if(set.map_height - p->line_id + line>0 &&   line<=p->line_id )
			Map[set.map_height - p->line_id + line][j]=(int)(p->type);
		}
	p=p->next;
	}
	
}



void show(struct player* pla,struct settings* set)//打印板和玩家，每秒刷新 
{	for(int i=1;i<=set.map_height;i++)
	{	gotoxy(1,i);
		for(int j=1;j<=set.map_width;j++) 
		{	printf(" ");	
		}
	}
	for(int i=1;i<=set.map_height;i++) 
	for(int j=1;j<=set.map_width;j++) 
	{	switch(Map[i][j])
		{	case 1:for(int k=1;k<=set.map_board_length;k++){printf("=");}	break; 
			case 2:for(int k=1;k<=set.map_board_length;k++){printf("=");}	break;
			case 3:for(int k=1;k<=set.map_board_length;k++){printf("=");}	break;
			case 4:for(int k=1;k<=set.map_board_length;k++){printf("=");}	break;	
		}
		gotoxy(j,i);
	}
	
	for(int i=pla.x-set.player_width/2;i<=pla.x+set.player_width/2;i++)
	for(int j=py-set.player_height+1;j<=py;j++)
	{gotoxy( i,j);printf("*");
	}
}

void print_frame()//游戏开始时，打印边框 
{	for(int i=1;i<=set.map_height+1;i++) 
	{	gotoxy(0,i);printf("|");gotoxy(set.map_width+1,i);printf("|");
	}
	for(int i=1;i<=set.map_width;i++) 
	{	gotoxy(i,0);printf("_");gotoxy(i,set.map_height+1);printf("_");
	}
	
} 



