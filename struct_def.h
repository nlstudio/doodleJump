#pragma once
#define MAX_SCORE_NUMBER 100      //�洢�����ٸ���ҵ���߷�����¼
#define MAX_SCORE_SERVER 20       //������������¼����������

struct settings {
	int map_height;               //��ͼ�߶�
	int map_width;                //��ͼ���
	int map_board_length;         //���峤��
	int player_height;            //��Ҹ߶�
	int player_width;             //��ҿ��
	unsigned long long dp_tpf;    //����tickˢ�»���
	int line;                     //��ǰ��Ϸ��׶������еı��
	/* �ٶȼ����ٶȵĵ�λΪ��ÿ���ÿ��ƽ�� */
	float velocity_LR;            //�����ƶ����ٶ�
	int remain_bounce_line;       //�����������������
	char ip_address[16];          //��������IP��ַ
};

struct player {
	float x;
	int y;                        //���λ��,��������¶˵��м�λ��Ϊ��׼
	unsigned long long pre_time;  //��һ����ײ��ʱ��
	int pre_board;                //��һ����ײ�İ��������
	int pre_board_type;           //��һ����ײ�İ������
	int remain_bounce_line;       //�����������������
};

struct board {
	int line_id;                  //��������
	int x;                        //��˵�λ��
	char type;                    //������ͣ�Ĭ��Ϊ1
	int if_moveable;              //�Ƿ���ƶ�
	int move_velocity;            //�ƶ��ٶȣ�����count�ƶ�һ�Σ�
	int move_count;               //��ʣ�����ƶ�
	int move_direction;           //����Ϊ-1������Ϊ1
	struct board* next;           //��һ���ڵ�
};

struct score {
	char player_name[128];        //�������
	int player_score;             //��ҵ÷�
	int if_upload;                //�Ƿ��ϴ���
};