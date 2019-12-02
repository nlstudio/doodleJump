#pragma once
struct settings {
	int map_height;               //地图高度
	int map_width;                //地图宽度
	int map_board_length;         //跳板长度
	int player_height;            //玩家高度
	int player_width;             //玩家宽度
	float player_drop_acc;        //玩家下落加速度
	unsigned long long dp_tpf;    //多少tick刷新画面
	int dp_tpl;                   //多少tick刷新一行
	int line;                     //当前游戏最底端所在行的编号
	/* 速度及加速度的单位为行每秒或每秒平方 */
	float velocity_UD;            //向上弹起的初速度
	float velocity_LR;            //左右移动的速度
	int remain_bounce_line;       //玩家能向上跳多少行
};

struct player {
	float x;
	int y;                        //玩家位置,以玩家最下端的中间位置为基准
	unsigned long long pre_time;  //上一次碰撞的时间
	int pre_board;                //上一次碰撞的板的所在行
	int pre_board_type;           //上一次碰撞的板的类型
	int remain_bounce_line;       //玩家能向上跳多少行
};

struct board {
	int line_id;                  //板所在行
	int x;                        //左端点位置
	char type;                    //板的类型，默认为1
	struct board* next;           //下一个节点
};