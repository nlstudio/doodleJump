#pragma once
struct settings {
	int map_height;               //��ͼ�߶�
	int map_width;                //��ͼ����
	int map_board_length;         //���峤��
	int player_height;            //��Ҹ߶�
	int player_width;             //��ҿ���
	unsigned long long dp_tpf;    //����tickˢ�»���
	int line;                     //��ǰ��Ϸ��׶������еı��
	/* �ٶȼ����ٶȵĵ�λΪ��ÿ���ÿ��ƽ�� */
	float velocity_LR;            //�����ƶ����ٶ�
	int remain_bounce_line;       //�����������������
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
	struct board* next;           //��һ���ڵ�
};