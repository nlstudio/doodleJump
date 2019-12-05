#pragma once
#include "struct_def.h"
#include <stdio.h>
settings set;
struct player player1,player2;
struct board* head = NULL;
struct board* tail = NULL;
struct score high_score[MAX_SCORE_NUMBER + 1];
int current_score = 0;
char** Map;