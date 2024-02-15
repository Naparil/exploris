#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <process.h>
#pragma comment(lib, "winmm")
#define _CRT_SEQURE_NO_WARNINGS
#define ENTER (13)
#define LEFT (75)
#define RIGHT (77)
#define UP (72)
#define DOWN (80)
#define SPACE (32)
#define SCORE_DROP (50)
#define SCORE_DELETE (1000)
#define SCORE_PERFECT (5000)
#define NUM_BLOCK_TO_DELETE_LINE (10)
#define NEED_LINE_TO_LEVELUP (5) // 레벨업에 필요한 지운 줄 수

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//함수 선언
void Draw_board(void); //기본 배경 출력하는 함수 fadsfsqwedf  sd
void Draw_recentblock(int); // 현재 블록 그리는 함수
void Draw_setcolor(int); // 글씨색 설정 함수
int Setting_blockcolor(int); // 블록 색 정하는 함수
void Star_setting(int, int); // *(보조선) 생성 함수
void Clear(void); //*(보조선), 목적지 블록 지우는 함수
void Select_block(void); // 블록을 랜덤으로 생성하는 함수
void Select_block_next(void); // 다음블록을 랜덤으로 생성하는 함수
void Nextblock_to_now(void); // 다음블록을 현재블록으로 갖고 오는 함수
void Draw_nextblock(int); // 다음블록 그리는 함수
int Calcul_block_width(int, int); // 블록의 가로길이를 계산하는 함수 (블록 종류, 블록 회전)
int Calcul_block_height(int, int); // 블록의 세로길이를 계산하는 함수 (블록 종류, 블록 회전)
void Set_fake_block(); // 목적지블록을 board 배열 안에 넣는 함수
void go(int, int); // 커서를 (x,y) 위치로 이동시키는 함수
void Shoot_block(void); //블록을 발사시키는 함수
void Set_value(int); //board에 특정 값을 넣는 함수
void Press_check(int); //입력값 체크하는 함수
void Press_check_bomb(void); //폭탄 쏠 떄 입력값 체크 함수
void Rot_move(int, int); //회전 보정 함수
void Block_clear(); // 블록을 board 배열에서 지우는 함수
void Place_block_desti_y(void); // 목적지 블록의 y위치를 정하는 함수
void Delete_line(void); // 라인 지우는 함수
int Collision_check(int, int, int, int); //블록이 다른 공간과 충돌하는지 확인하는 함수 (종류, 회전, x위치, y위치)
void Draw_start(void); // 시작화면 드로우
void Check_level(void); // 레벨 올리는 함수
void Check_score_cipher(void); // 점수 자릿수 체크 함수
void Place_block_desti_y_up(void);//위키 눌렀을 떄 블록 y위치 정하는 함수
void Place_block_desti_y_down(void); //아래키 눌렀을 때 블록 y위치 정하는 함수
int Place_bomb_y(int);
void Check_game_over(void); // 게임 오버 체크 함수
void Game_restart(void); // 게임 재시작 함수
void Shoot_bomb(void); //폭탄 쏘는 함수
void Star_setting_bomb(int, int); //폭탄 보조선을 만드는 함수

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//전역변수 선언
int board[20][20] = { 0 }; //맵
int block[6] = { 0 }; //현재 블록 (블록 종류(0), 회전(1), 도착 x 위치(2), 도착 y위치(3), x위치(4), y위치(5))
// O미노 : 0   /  I미노 : 1  /  Z미노 : 2  / S미노 : 3  /  J미노 : 4  /  L미노 : 5  /  T미노 : 6
// 0도 : 0  / 90도 : 1  / 180도 : 2  /  270도 : 3
int block_desti[4] = { 0 }; // 현재 목적지 블록 (블록 종류(0), 회전(1), 도착 x 위치(2), 도착 y위치(3))
int block_next[4] = { 0 }; //다음 블록
int game_situation = -2;  // 현재 게임 상황 (-2: 인트로  -1: 발사 처리중 0: 조준중  1: 발사  2 : 운동 중  3 : 게임오버  4: 폭탄 발사)
int press = 0; //사용자의 입력값
int stop = 0; // 움직이는 블록의 착지 유무
int last_x = 0; //목적지 블록을 회전 보정시킬 때 그 전 x위치값을 저장시키는 변수
int move_block_x[20] = { 0 }; //움직이는 블록의 x 경로
int move_block_x_inde[20] = { 0 }; //움직이는 블록의 x 경로의 증감
int move_bomb_x[20] = { 0 }; //움직이는 폭탄의 x 경로
int block_list[7] = { 0 }; //블록이 중복되지 않게 이전 블록을 저장하는 배열
int condition_s = -1; // 왼쪽 : 0, 오른쪽 : 1
int limit_time=100; //제한시간
int time_act = 0; //시간 표시해야할 떄 (1) / 발사 중(0)
int option_color = 0; //색약 모드
int option_sfx = 1; //효과음 on off
int delete_line_num = 0; //지운 줄 개수
int score = 0; // 점수
int level = 1; // 레벨
int len = 1; //점수 자릿수
int game_over = 0; //게임오버 여부
int bomb_num = 3; //폭탄 개수
int doing_shoot = 0; // 블록 쏘는 동안 1 아니면 0
int doing_bomb = 0; //폭탄 위치 선정 중엔 1 , 아닐 땐 0
int bomb[2] = { 0 }; //폭탄 x,y위치
int perfect_clear = 0; //퍼펙트 클리어시, 화면에 출력하기 위한 변수. (1이면 퍼펙트 클리어)
double time_to_deter[20] = {10,9.5,9,8.5,8,7.5,7,6.5,6,5.5,5,4.5,4,3.5,3,2.5,2,1.5,1,0.5 }; //레벨당 둘 제한시간(초)

//드로우 스레드
unsigned _stdcall Thread_draw(void *arg)
{
	int i, j, k, f;

	while (1)
	{
		if (game_over == 1)
		{
			game_situation = 3;
			system("cls");
			int temp;

			go(5, 8);
			printf("┌──────────────────────────┐ ");
			for (int i = 0; i < 8; i++)
			{
				go(5, 9 + i);
				printf("│                          │ ");
			}
			go(5, 17);
			printf("└──────────────────────────┘ ");

			go(15, 10);
			printf("Game Over!");

			go(8, 13);
			printf("SCORE : %d", score);

			go(8, 15);
			printf("Z : Restart / X : exit");

			temp = _getch();

			if (temp=='z') {
				Game_restart();
			}
			if (temp == 'x') {
				system("cls");
				go(1, 1);
				exit(0);
			}
			
		}

		if (game_situation == 0) { Star_setting(block_desti[2] + round((double)Calcul_block_width(block_desti[0], block_desti[1])/2.000000), block_desti[3]); }
		else if (game_situation == 4) {
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++)
				{
					if (board[j][i] == 0 || board[j][i] == 4) { go(3 + j * 2, 2 + i); printf("  "); } // 빈 공간
				}
			}
			Star_setting_bomb(bomb[0] , bomb[1]);
		}
		else if (game_situation == 5) {
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++)
				{
					if (board[j][i] == 0) { go(3 + j * 2, 2 + i); printf("  "); } // 빈 공간
				}
			}
		}

		for (i = 0; i < 20; i++) {
			for (j = 0; j < 20; j++)
			{
				if (board[j][i] == 0) { go(3 + j * 2, 2 + i); printf("  "); } // 빈 공간
				else if (board[j][i] == 1) { Draw_setcolor(Setting_blockcolor(block_desti[0])); go(3 + j * 2, 2 + i); printf("■"); } // 블록 공간
				else if (board[j][i] == 2 && (game_situation ==0 || game_situation==4)) { Draw_setcolor(8); go(3 + j * 2, 2 + i); printf("*"); } // 안내선 공간
				else if (board[j][i] == 3) { Draw_setcolor(8); go(3 + j * 2, 2 + i); printf("▣"); } //이미 놓여진 블록
				else if (board[j][i] == 4 && game_situation==0) { Draw_setcolor(Setting_blockcolor(block_desti[0])); go(3 + j * 2, 2 + i); printf("▒"); } // 목적지 블록
				else if (board[j][i] == 5) { Draw_setcolor(15); go(3 + j * 2, 2 + i); printf("★"); } // 줄 지워질 떄 이펙트
				else if (board[j][i] == 6 && doing_bomb==1) { Draw_setcolor(8); go(3 + j * 2, 2 + i); printf("⊙"); } // 폭탄 목적지
				else if (board[j][i] == 7 && doing_bomb == 1) { Draw_setcolor(15); go(3 + j * 2, 2 + i); printf("⊙"); } // 폭탄
			}
		}

		if (game_situation == 0)
		{
			//제한시간
			for (i = 0; i < limit_time; i++)
			{
				go(3 + 2 * i, 24);
				if (option_color == 0)
				{
					Draw_setcolor(15);
				}
				else {
					Draw_setcolor(7);
				}
				printf("●");
			}
			for (j = limit_time; j < 20; j++)
			{
				go(3 + 2 * j, 24);
				printf("  ");
			}
		}
		if (game_situation == -1 || game_situation == 1 || game_situation == 2 || game_situation==4 || game_situation == 5)
		{
			for (i = 0; i < 20; i++)
			{
				go(3 + 2 * i, 24);
				Draw_setcolor(15);
				printf("  ");
			}
		}

		switch (bomb_num)
		{
		case 0:
			go(48, 18);
			if (option_color == 0)
			{
				Draw_setcolor(15);
			}
			else {
				Draw_setcolor(7);
			}
			printf("   ◇ ◇ ◇");
			break;
		case 1:
			go(48, 18);
			if (option_color == 0)
			{
				Draw_setcolor(15);
			}
			else {
				Draw_setcolor(7);
			}
			printf("   ◈ ◇ ◇");
			break;
		case 2:
			go(48, 18);
			if (option_color == 0)
			{
				Draw_setcolor(15);
			}
			else {
				Draw_setcolor(7);
			}
			printf("   ◈ ◈ ◇");
			break;
		case 3:
			go(48, 18);
			if (option_color == 0)
			{
				Draw_setcolor(15);
			}
			else {
				Draw_setcolor(7);
			}
			printf("   ◈ ◈ ◈");
			break;
		}

		go(54, 26);
		if (level < 10) {
			printf("0%d", level);
		}
		else {
			printf("%d", level);
		}
		go(48, 22);
		printf("             ");

		for (k = 0; k < 8 - len; k++)
		{
			go(51 + k, 22);
			printf("0");
		}
		go(59 - len, 22);
		printf("%d", score);

		//현재 블록 드로우
		Draw_recentblock(block_desti[0]);
		Draw_nextblock(block_next[0]);

		if (perfect_clear == 1)
		{
			Draw_setcolor(11);
			go(66, 10);
			printf("┌────────────────────┐ ");
			go(66, 11);
			printf("│       PERFECT      │ ");
			go(66, 12);
			printf("│        CLEAR       │ ");
			go(66, 13);
			printf("└────────────────────┘ ");
		}
		else {
			for (f = 0; f < 4; f++)
			{
				go(66, 10+f);
				printf("                       ");
			}
		}
	
	}
	_endthread();
}

//제한시간 스레드
unsigned _stdcall Thread_time(void *arg)
{
	if (game_situation == 0) {
		limit_time = 20;
		while (limit_time > 0)
		{
			if (game_over == 1)
			{
				_endthread();
			}

			limit_time -= 1;
			if (doing_shoot == 1)
			{
				_endthread();
			}
			if (level < 20)
			{
				Sleep((int)((double)50 * time_to_deter[level - 1]));
			}
			else
			{
				Sleep(25);
			}
		}
	}
	else
	{
		limit_time = 0;
	}
	_endthread();
}

//퍼펙트 클리어 체크 스레드
unsigned _stdcall Thread_perfect(void *arg)
{
		int temp_per = 0;
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (board[i][j] == 3) { temp_per = 1; }
			}
		}
		if (temp_per == 0)
		{
			perfect_clear = 1;
			score += SCORE_PERFECT;
			Sleep(500);
			perfect_clear = 0;
			_endthread();
		}
		_endthread();
}

int main(void)
{
	Draw_start();
	system("cls");
	if (option_color == 0)
	{
		Draw_setcolor(15);
	}
	else {
		Draw_setcolor(8);
	}
	Draw_board();
	Select_block();
	Select_block_next();
	_beginthreadex(NULL, 0, Thread_draw, 0, 0, NULL);

	while (1) {
		if (game_situation == -1) {
			Block_clear();
			Set_value(3);
			Check_game_over();
			Nextblock_to_now();
			Select_block_next();

			for (int b = 0; b < 20; b++) {
				for (int c = 0; c < 20; c++) {
					if (board[b][c] == 1) { board[b][c] = 0; }
				}
			}

			Set_fake_block();
			Clear();
			Delete_line();
			_beginthreadex(NULL, 0, Thread_perfect, 0, 0, NULL);
			Check_level();
			Place_block_desti_y();
			Star_setting(block_desti[2] + round((double)Calcul_block_width(block_desti[0], block_desti[1]) / 2.000000), block_desti[3]);
			Set_fake_block();
			Clear();
			time_act = 0;
			limit_time = 20;
			game_situation = 0;
			doing_shoot = 0;
		}

		if (game_situation == 0) {
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (board[i][j] == 5 || board[i][j] == 6 || board[i][j] == 7) { board[i][j] = 0; }
				}
			}
			Set_fake_block();

			if (time_act == 0) {
				_beginthreadex(NULL, 0, Thread_time, 0, 0, NULL);
				time_act = 1;
			}

			if (limit_time > 0) 
			{
				while (_kbhit())
				{
					press = _getch();
					Clear();
					Press_check(press);
					Set_fake_block();
					press = '\0';
				}
			}
			else 
			{
				score += SCORE_DROP;
				Check_score_cipher();
				game_situation = 1;
			}
		}

		if (game_situation == 1) {
			Clear();
			game_situation = 2;
			Shoot_block();
		}

		if (game_situation == 4) {
			Press_check_bomb();

		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//[[[[[게임]]]]]

//처음블록을 랜덤으로 생성하는 함수
void Select_block(void)
{
	srand((int)time(NULL));
	block_desti[0] = rand() % 7;
	block_list[block_desti[0]] = 1;
	if (block_desti[0] == 2 || block_desti[0] == 3) { block_desti[1] = 1; }
	else if (block_desti[0] == 6) { block_desti[1] = 2; }
	else { block_desti[1] = 0; }

	switch (Calcul_block_width(block_desti[0], block_desti[1]))
	{
	case 1:
		block_desti[2] = 10;
		break;

	case 2:
		block_desti[2] = 9;
		break;

	case 3:
		block_desti[2] = 9;
		break;

	case 4:
		block_desti[2] = 8;
		break;
	}

	block_desti[3] = 20 - Calcul_block_height(block_desti[0], block_desti[1]);;
}

//다음블록을 랜덤으로 생성하는 함수
void Select_block_next(void)
{
	int comp = 0;
	int temp = 0;

	for (int i = 0; i < 7; i++)
	{
		if (block_list[i] == 1) { comp++; }
	}

	if (comp == 7)
	{
		for (int j = 0; j < 7; j++)
		{
			block_list[j] = 0;
		}
	}

	do {
		temp = rand() % 7;
	} while (block_list[temp] == 1);

	block_list[temp] = 1;
	block_next[0] = temp;

	if (block_next[0] == 2 || block_next[0]==3) { block_next[1] = 1; }
	else if (block_next[0] == 6) { block_next[1] = 2; }
	else { block_next[1] = 0; }

	switch (Calcul_block_width(block_next[0], block_next[1]))
	{
	case 1:
		block_next[2] = 10;
		break;

	case 2:
		block_next[2] = 9;
		break;

	case 3:
		block_next[2] = 9;
		break;

	case 4:
		block_next[2] = 8;
		break;
	}

	block_next[3] = 20 - Calcul_block_height(block_next[0], block_next[1]);;
}

//다음블록을 현재블록으로 가져오는 함수
void Nextblock_to_now(void)
{
	for (int i = 0; i < 3; i++)
	{
		block_desti[i] = block_next[i];
	}
	Place_block_desti_y();
}

//블록 쏘는 함수
void Shoot_block(void)
{
	if (block_desti[3] > 0)
	{
		if (board[10][2] == 3 || board[11][2] == 3)
		{
			for (int i = 0; i < 4; i++)
			{
				block[i] = block_desti[i];
			}

			switch (Calcul_block_width(block[0], block[1]))
			{
			case 1:
				block[4] = 10;
				break;

			case 2:
				block[4] = 9;
				break;

			case 3:
				block[4] = 9;
				break;

			case 4:
				block[4] = 8;
				break;
			}
			block[5] = 0; //블록 초기 위치
			Set_value(3);
			Sleep(1000);
			game_over = 1;
		}

		int i;
		for (i = 0; i < 4; i++)
		{
			block[i] = block_desti[i];
		}
		
		switch (Calcul_block_width(block[0], block[1]))
		{
		case 1:
			block[4] = 10;
			break;

		case 2:
			block[4] = 9;
			break;

		case 3:
			block[4] = 9;
			break;

		case 4:
			block[4] = 8;
			break;
		}
		block[5] = 0; //블록 초기 위치

		if (block_desti[2] < 10) { condition_s = 0; }
		else { condition_s = 1; }

		int s_time = 20; //블록이 한칸 이동하는데 걸리는 시간
		int move_num = 0; //이동 횟수
		int condition = -1; //x 이동 방향
		int move_y = 0;

		while (stop == 0)
		{
			Block_clear();
			Set_value(1);

			if (Collision_check(block[0], block[1], move_block_x[move_num], block[5] + 1) == 0)
			{
				move_block_x_inde[move_num] = move_block_x[move_num] - block[4];
				block[4] = move_block_x[move_num];
				block[5]++;
				if (move_block_x[move_num] > 0) { condition = 0; }
				if (move_block_x[move_num] < 0) { condition = 1; }
				move_num++;
			}
			else { stop = 1; }

			if (block[5] == block[3])
			{
				stop = 1;
			}

			Sleep(s_time);
		}  //첫번째 운동 (하강)

		if (option_sfx == 1)
		{
			PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
		}
		int max_move_num = move_num;

		int temp[20] = { 0 };
		for (int k = 0; k < max_move_num; k++)
		{
			temp[k] = move_block_x_inde[max_move_num - k];
		} //배열 뒤집기

		move_num = 0;

		while (stop == 1)
		{
			Block_clear();
			Set_value(1);

			if (temp[move_num] < 0 && block[4] + temp[move_num] < 0)
			{
				if (Collision_check(block[0], block[1], block[4] - temp[move_num], block[5] - 1) == 0)
				{
					block[4] -= temp[move_num];
					condition = 1;
					if (option_sfx == 1)
					{
						PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
					}
				}
				else { stop = 2; break; }
			}
			else if (temp[move_num] > 0 && block[4] + temp[move_num] + Calcul_block_width(block[0], block[1]) > 19)
			{
				if (Collision_check(block[0], block[1], block[4] - temp[move_num], block[5] - 1) == 0)
				{
					block[4] -= temp[move_num];
					condition = 1;
					if (option_sfx == 1)
					{
						PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
					}
				}
				else { stop = 2; break; }
			}
			else
			{
				if (condition == 0) {
					if (Collision_check(block[0], block[1], block[4] + temp[move_num], block[5] - 1) == 0)
					{
						block[4] += temp[move_num];
					}
					else { stop = 2; break; }
				}
				if (condition == 1) {
					if (Collision_check(block[0], block[1], block[4] - temp[move_num], block[5] - 1) == 0)
					{
						block[4] -= temp[move_num];
					}
					else { stop = 2; break; }
				}
			}

			block[5] -= 1;

			move_num++;

			if (block[5] == 0)
			{
				stop = 2; break;
			}
			Sleep(s_time);
		}	//두번째 운동 (상승)

		if (option_sfx == 1)
		{
			PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
		}
		move_num = 0;

		while (stop == 2)
		{
			Block_clear();
			Set_value(1);

			if (move_block_x_inde[move_num] < 0 && block[4] + move_block_x_inde[move_num] < 0)
			{
				if (Collision_check(block[0], block[1], block[4] - move_block_x_inde[move_num], block[5] + 1) == 0)
				{
					block[4] -= move_block_x_inde[move_num];
					condition = 0;
					if (option_sfx == 1)
					{
						PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
					}
				}
				else { stop = 3; break; }
			}
			else if (move_block_x_inde[move_num] > 0 && block[4] + move_block_x_inde[move_num] + Calcul_block_width(block[0], block[1]) > 19)
			{
				if (Collision_check(block[0], block[1], block[4] - move_block_x_inde[move_num], block[5] + 1) == 0)
				{
					block[4] -= move_block_x_inde[move_num];
					condition = 1;
					if (option_sfx == 1)
					{
						PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
					}
				}
				else { stop = 3; break; }
			}
			else
			{
				if (condition == 1) {
					if (Collision_check(block[0], block[1], block[4] - move_block_x_inde[move_num], block[5] + 1) == 0)
					{
						block[4] -= move_block_x_inde[move_num];
					}
					else { stop = 3; break; }
				}
				if (condition == 0) {
					if (Collision_check(block[0], block[1], block[4] + move_block_x_inde[move_num], block[5] + 1) == 0)
					{
						block[4] += move_block_x_inde[move_num];
					}
					else { stop = 3; break; }
				}
			}

			block[5] += 1;
			move_num++;
			if (block[5] == 20 - Calcul_block_height(block[0], block[1]))
			{
				stop = 3;
				break;
			}

			Sleep(s_time);
		}  //세번째 운동 (하강)

		if (option_sfx == 1)
		{
			PlaySound(TEXT("bound.wav"), NULL, SND_ASYNC);
		}

		if (stop == 3)
		{
			move_y = 20 - block[5] - Calcul_block_height(block[0], block[1]);
			int a = 1;
			while (a <= move_y)
			{
				if (Collision_check(block[0], block[1], block[4], block[5] + 1) == 0)
				{
					block[5]++;
					a++;
				}
				else
				{
					break;
				}
			}


			stop = 0;
			for (int a = 0; a < 20; a++) {
				move_block_x[a] = 0;
				move_block_x_inde[a] = 0;
			}
			if (option_sfx == 1)
			{
				PlaySound(TEXT("land.wav"), NULL, SND_ASYNC);
			}
			game_situation = -1;
		}
	}

	else {
	for (int i = 0; i < 4; i++)
	{
		block[i] = block_desti[i];
	}
	block[4] = 10;
	block[5] = 0; //블록 초기 위치
	Set_value(3);
	Sleep(1000);
	if (option_sfx == 1)
	{
		PlaySound(TEXT("land.wav"), NULL, SND_ASYNC);
	}
	game_over = 1;
	} //만약 목적지 블록의 y위치가 0이었던 경우
}

//키보드 입력값에 따른 행동을 취하는 함수
void Press_check(int press)
{
	int temp;
	if (press == RIGHT) {
		if (block_desti[2] < 20 - Calcul_block_width(block_desti[0], block_desti[1])) {
			block_desti[2]++;
			last_x = block_desti[2];
			Place_block_desti_y();
		}
	} //오른쪽 눌렀을 떄

	if (press == LEFT) {
		if (block_desti[2] > 0) {
			block_desti[2]--;
			last_x = block_desti[2];
			Place_block_desti_y();
		}
	} //왼쪽 눌렀을 떄

	if (press == UP) {
		Place_block_desti_y_up();
	}

	if (press == DOWN) {
		Place_block_desti_y_down();
	}

	if (press == 'z') {
		temp = block_desti[1];
		if (block_desti[1] > 0) { 
				block_desti[1]--;
		}
		else {
			block_desti[1] = 3;
		}
		if (block_desti[3] + 1 < Calcul_block_height(block_desti[0], block_desti[1]))
		{
			block_desti[1] = temp;
		}
		Rot_move(block_desti[0], block_desti[1]);
		Place_block_desti_y();
	} //반시계방향

	if (press == 'x') {
		temp = block_desti[1];
		if (block_desti[1] < 3) {
			block_desti[1]++;
		}
		else {
			block_desti[1] = 0;
		}
		if (block_desti[3] + 1 < Calcul_block_height(block_desti[0], block_desti[1]))
		{
			block_desti[1] = temp;
		}
		Rot_move(block_desti[0], block_desti[1]);
		Place_block_desti_y();
	} //시계방향

	if (press == 'c') {
		if (bomb_num > 0)
		{
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (board[i][j] == 5 || board[i][j] == 6 || board[i][j] == 7) { board[i][j] = 0; }
				}
			}
			doing_bomb = 1;
			game_situation = 4;
			bomb_num--;
			bomb[0] = 10;
			for (int i = 0; i <20; i++)
			{
				if (board[10][i] != 3) { bomb[1] = i; }
			}
			board[bomb[0]][bomb[1]] = 6;		
		}
	} //폭탄

	if (press == SPACE && game_situation==0 && doing_shoot==0) {
		score += SCORE_DROP+limit_time;
		Check_score_cipher();
		doing_shoot = 1;
		game_situation = 1;
	} //스페이스바(드롭)
}

//폭탄 쏠 때 키보드 입력값 체크
void Press_check_bomb(void)
{
	int y_check = 0;
	int bomb_stop = 0;
	int temp_save_y = 0;

	while (_kbhit())
	{
		press = _getch();

		if (press == RIGHT) {
			if (bomb[0] < 19) {
				temp_save_y = bomb[1];
				do {
					if (y_check < 19 - bomb[0])
					{
						y_check++;
					}
					else
					{
						bomb_stop = 1;
						break;
					}
				} while (Place_bomb_y(y_check)==0);

				if (bomb_stop == 0)
				{
					if (board[bomb[0] + y_check][bomb[1]] == 0)
					{
						board[bomb[0]][temp_save_y] = 0;
						bomb[0] += y_check;
						board[bomb[0]][bomb[1]] = 6;
					}
				}
			}
		} //오른쪽 눌렀을 떄

		if (press == LEFT) {
			if (bomb[0] > 0) {
				temp_save_y = bomb[1];
				do {
					if (y_check < bomb[0])
					{
						y_check++;
					}
					else
					{
						bomb_stop = 1;
						break;
					}
				} while (Place_bomb_y(-y_check) == 0);

				if (bomb_stop == 0)
				{
					if (board[bomb[0] - y_check][bomb[1]] == 0)
					{
						board[bomb[0]][temp_save_y] = 0;
						bomb[0] -= y_check;
						board[bomb[0]][bomb[1]] = 6;
					}
				}
			}
		} //왼쪽 눌렀을 떄

		if (press == UP) {
			if (bomb[1] > 0) {
				for (int i = bomb[1] - 1; i > -1; i--)
				{
					if (board[bomb[0]][i]==0)
					{
						board[bomb[0]][bomb[1]] = 0;
						bomb[1]=i;
						board[bomb[0]][bomb[1]] = 6;
						break;
					}
				}
			}
		}

		if (press == DOWN) {
			if (bomb[1] < 19) {
				for (int i = bomb[1] + 1; i < 20; i++)
				{
					if (board[bomb[0]][i] == 0)
					{
						board[bomb[0]][bomb[1]] = 0;
						bomb[1] = i;
						board[bomb[0]][bomb[1]] = 6;
						break;
					}
				}
			}
		}

		if (press == SPACE && game_situation==4) {
			board[bomb[0]][bomb[1]] = 0;
			game_situation = 5;
			Clear();
			Shoot_bomb();
		}
		Clear();
		press = '\0';
	}
}

//블록 회전 보정
void Rot_move(int kind, int rot)
{
	int num = 0;
	if (block_desti[2] + Calcul_block_width(block_desti[0], block_desti[1]) > 19)
	{
		num = block_desti[2] + Calcul_block_width(block_desti[0], block_desti[1]) - 20;
		last_x = block_desti[2];
		block_desti[2] -= num;
	}
	else if (block_desti[2] + Calcul_block_height(block_desti[0], block_desti[1]) > 19)
	{
		block_desti[2] = last_x;
		last_x = block_desti[2];
	}
	else
	{
		last_x = block_desti[2];
	}
}

//라인 지우는 함수
void Delete_line(void)
{
	int num, eff=0 , n = 0;
	int line_del[20] = { 0 };
	for (int i = 0; i < 20; i++)
	{
		num = 0;

		for (int j = 0; j < 20; j++)
		{
			if (board[j][i] == 3) { num++; }
		}
		if (num >= NUM_BLOCK_TO_DELETE_LINE)
		{
			for (int k = 0; k < 20; k++)
			{
				board[k][i] = 0;
			}
			line_del[n] = i;
			n++;
		}
	}

	int h, a; //for문을 위한 임시변수

	if (n > 0) {
		while (eff < 2) {
			for (h = 0; h < 20; h++)
			{
				for (a = 0; a < n; a++)
				{
					board[h][line_del[a]] = 5;
				}
			}
			Sleep(150);

			for (h = 0; h < 20; h++)
			{
				for (a = 0; a < n; a++)
				{
					board[h][line_del[a]] = 0;
				}
			}
			Sleep(150);
			eff++;
		}

		delete_line_num += n;
		score += SCORE_DELETE * n;
		Check_score_cipher();
		if (option_sfx == 1)
		{
			PlaySound(TEXT("delete.wav"), NULL, SND_ASYNC);
		}

		for (int f = 0; f < n + 1; f++)
		{
			for (int l = line_del[f] - 1; l > -1; l--)
			{
				for (int m = 0; m < 20; m++)
				{
					board[m][l + 1] = board[m][l];
				}
			}
		}
	}
}

//레벨 올리는 함수
void Check_level(void)
{
	if (delete_line_num >= level * NEED_LINE_TO_LEVELUP)
	{
		if (level < 20)
		{
			if (option_sfx == 1)
			{
				PlaySound(TEXT("levelup.wav"), NULL, SND_ASYNC);
			}
			if (bomb_num < 3)
			{
				bomb_num++;
			}
			level++;
		}
	}
}

//게임 재시작 함수
void Game_restart(void)
{
	system("cls");

	if (option_color == 0)
	{
		Draw_setcolor(15);
	}
	else {
		Draw_setcolor(8);
	}

	game_over = 0;
	level = 1;
	score = 0;
	len = 1;
	delete_line_num = 0;
	bomb_num = 3;
	doing_shoot = 0;
	doing_bomb = 0;
	stop = 0;
	time_act = 0;
	for (int i = 0; i < 6; i++)
	{
		block[i] = 0;
	}
	for (int k = 0; k < 5; k++)
	{
		block_desti[k] = 0;
		block_next[k] = 0;
	}
	for (int p = 0; p < 7; p++)
	{
		block_list[p] = 0;
	}
	block_desti[3] = 10;


	game_situation = 0;

	//초기 판 생성
	Draw_board();
	for (int i = 0; i < 20; i++)
	{
		for (int k = 0; k < 20; k++)
		{
			board[i][k] = 0;
		}
	}
	Select_block();
	Select_block_next();
}

//게임 오버 체크 함수
void Check_game_over(void)
{
	int temp = 0;
	for (int rot = 0; rot < 4; rot++)
	{
		if (Collision_check(block_next[0], rot, block_next[2], 0) == 1)
		{
			temp++;
		}
	}
	if (temp == 4)
	{
		Sleep(1000);
		game_over = 1;
	}
}

//폭탄 쏘는 함수
void Shoot_bomb(void)
{
	Clear();
	int s_time = 20; //폭탄이 한칸 이동하는데 걸리는 시간
	int temp_x = 10;
	int temp_y = 0;
	int move_num = 0;
	int s = 1;

	while (s==1)
	{
		board[temp_x][temp_y] = 0;

		if (board[move_bomb_x[0]][temp_y+1] != 3)
		{
			temp_x = move_bomb_x[move_num];
			temp_y++;
			move_num++;
		}
		else {  s=0; }

		if (temp_y==bomb[1])
		{
			s = 0;
		}
		board[temp_x][temp_y] = 7;
		Sleep(s_time);
	}  //폭탄의 운동

	board[temp_x][temp_y] = 5;
	board[temp_x][temp_y - 1] = 5;
	board[temp_x][temp_y - 2] = 5;
	if (temp_x > 1)
	{
		board[temp_x - 2][temp_y - 2] = 5;
		board[temp_x - 2][temp_y - 1] = 5;
		board[temp_x - 2][temp_y] = 5;
	}
	if (temp_x > 0)
	{
		board[temp_x - 1][temp_y - 2] = 5;
		board[temp_x - 1][temp_y - 1] = 5;
		board[temp_x - 1][temp_y] = 5;
	}
	if (temp_x < 19)
	{
		board[temp_x + 1][temp_y - 2] = 5;
		board[temp_x + 1][temp_y - 1] = 5;
		board[temp_x + 1][temp_y] = 5;
	}
	if (temp_x < 18)
	{
		board[temp_x + 2][temp_y - 2] = 5;
		board[temp_x + 2][temp_y - 1] = 5;
		board[temp_x + 2][temp_y] = 5;
	}
	if (temp_y < 19)
	{
		board[temp_x - 2][temp_y + 1] = 5;
		board[temp_x - 1][temp_y + 1] = 5;
		board[temp_x][temp_y + 1] = 5;
		board[temp_x + 1][temp_y + 1] = 5;
		board[temp_x + 2][temp_y + 1] = 5;
	}
	if (temp_y < 18)
	{
		board[temp_x - 2][temp_y + 2] = 5;
		board[temp_x - 1][temp_y + 2] = 5;
		board[temp_x][temp_y + 2] = 5;
		board[temp_x + 1][temp_y + 2] = 5;
		board[temp_x + 2][temp_y + 2] = 5;
	}
	if (option_sfx == 1)
	{
		PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
	}
	Sleep(1000);

	game_situation = 6;

	board[temp_x][temp_y] = 0;
	board[temp_x][temp_y - 1] = 0;
	board[temp_x][temp_y - 2] = 0;
	if (temp_x > 1)
	{
		board[temp_x - 2][temp_y - 2] = 0;
		board[temp_x - 2][temp_y - 1] = 0;
		board[temp_x - 2][temp_y] = 0;
	}
	if (temp_x > 0)
	{
		board[temp_x - 1][temp_y - 2] = 0;
		board[temp_x - 1][temp_y - 1] = 0;
		board[temp_x - 1][temp_y] = 0;
	}
	if (temp_x < 19)
	{
		board[temp_x + 1][temp_y - 2] = 0;
		board[temp_x + 1][temp_y - 1] = 0;
		board[temp_x + 1][temp_y] = 0;
	}
	if (temp_x < 18)
	{
		board[temp_x + 2][temp_y - 2] = 0;
		board[temp_x + 2][temp_y - 1] = 0;
		board[temp_x + 2][temp_y] = 0;
	}
	if (temp_y < 19)
	{
		board[temp_x - 2][temp_y + 1] = 0;
		board[temp_x - 1][temp_y + 1] = 0;
		board[temp_x][temp_y + 1] = 0;
		board[temp_x + 1][temp_y + 1] = 0;
		board[temp_x + 2][temp_y + 1] = 0;
	}
	if (temp_y < 18)
	{
		board[temp_x - 2][temp_y + 2] = 0;
		board[temp_x - 1][temp_y + 2] = 0;
		board[temp_x][temp_y + 2] = 0;
		board[temp_x + 1][temp_y + 2] = 0;
		board[temp_x + 2][temp_y + 2] = 0;
	}

	Place_block_desti_y();
	Star_setting(block_desti[2] + round((double)Calcul_block_width(block_desti[0], block_desti[1]) / 2.000000), block_desti[3]);
	for (int a = 0; a < 20; a++) {
		move_bomb_x[a] = 0;
	}
	for (int b = 0; b < 20; b++)
	{
		for (int c = 0; c < 20; c++)
		{
			if (board[b][c] == 2 || board[b][c] == 5 || board[b][c] == 6 || board[b][c] == 7) { board[b][c] = 0; }
		}
	}
	Set_fake_block();
	Clear();
	time_act = 0;
	limit_time = 20;
	doing_bomb = 0;
	game_situation = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//[[[[[드로우]]]]]

//초기화면 드로우 함수
void Draw_board(void)
{
	int i;

	go(1, 1);
	printf("■■■■■■■■■■■■■■■■■■■■■■\n");
	for (i = 0; i < 20; i++)
	{
		go(1, i + 2);
		printf("■                                        ■\n");
	}

	go(1, 22);
	printf("■■■■■■■■■■■■■■■■■■■■■■\n");
	go(1, 23);
	printf("■                제한시간                ■\n");
	go(1, 24);
	printf("■                                        ■\n");
	go(1, 25);
	printf("■■■■■■■■■■■■■■■■■■■■■■\n");
	go(46, 1);
	printf("┌───────────────┐ ");
	go(46, 2);
	printf("│    현재블록   │ ");
	go(46, 3);
	printf("├───────────────┤ ");
	go(46, 4);
	printf("│               │ ");
	go(46, 5);
	printf("│               │ ");
	go(46, 6);
	printf("│               │ ");
	go(46, 7);
	printf("│               │ ");
	go(46, 8);
	printf("├───────────────┤ ");
	go(46, 9);
	printf("│    다음블록   │ ");
	go(46, 10);
	printf("├───────────────┤ ");
	go(46, 11);
	printf("│               │ ");
	go(46, 12);
	printf("│               │ ");
	go(46, 13);
	printf("│               │ ");
	go(46, 14);
	printf("│               │ ");
	go(46, 15);
	printf("├───────────────┤ ");
	go(46, 16);
	printf("│      폭탄     │ ");
	go(46, 17);
	printf("├───────────────┤ ");
	go(46, 18);
	printf("│               │ ");
	go(46, 19);
	printf("├───────────────┤ ");
	go(46, 20);
	printf("│      점수     │ ");
	go(46, 21);
	printf("├───────────────┤ ");
	go(46, 22);
	printf("│               │ ");
	go(46, 23);
	printf("├───────────────┤ ");
	go(46, 24);
	printf("│      레벨     │ ");
	go(46, 25);
	printf("├───────────────┤ ");
	go(46, 26);
	printf("│               │ ");
	go(46, 27);
	printf("└───────────────┘ ");

	if (option_color == 0)
	{
		Draw_setcolor(7);
	}
	else {
		Draw_setcolor(8);
	}
	go(66, 1);
	printf("┌────────────────────┐ ");
	go(66, 2);
	printf("│        조작법      │ ");
	go(66, 3);
	printf("│                    │ ");
	go(66, 4);
	printf("│     ◀▶▲▼ 이동  │ ");
	go(66, 5);
	printf("│                    │ ");
	go(66, 6);
	printf("│      Z / X 회전    │ ");
	go(66, 7);
	printf("│                    │ ");
	go(66, 8);
	printf("│      SPACE 드롭    │ ");
	go(66, 9);
	printf("└────────────────────┘ ");
}

//*(보조선) 생성 함수
void Star_setting(int end_x, int end_y)
{
	int i, res; //end_x와 end_y는 보조선이 끝나는 위치.

	for (i = 0; i < end_y; i++)
	{

		res = round(10 + (int)(i*(end_x - 10.5) / end_y));
		if (board[res][i] == 0) { board[res][i] = 2; }\
		move_block_x[i] = res;
	}
	for (i = end_y; i < 20; i++)
	{
		move_block_x[i] = move_block_x[end_y];
	}
	for (i = 0; i < end_y; i++)
	{
		move_block_x[i] +=  block_desti[2] - res;
	}
}

// 폭탄 보조선 생성 함수
void Star_setting_bomb(int end_x, int end_y)
{
	int i, res; //end_x와 end_y는 보조선이 끝나는 위치.

	for (i = 0; i < end_y; i++)
	{

		res = round(10 + (int)(i*(end_x - 9.5) / end_y));
		if (board[res][i] == 0) { board[res][i] = 2; }
			move_bomb_x[i] = res;
	}
}

//*(보조선), 목적지 블록 지우는 함수
void Clear(void)
{
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[i][j] == 2 || board[i][j] == 4) { board[i][j] = 0; }
		}
	}
}

//실제 블록 지우는 함수
void Block_clear()
{
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (board[i][j] == 1) { board[i][j] = 0; }
		}
	}
}

//현재 블록 그리는 함수
void Draw_recentblock(int a)
{
	switch (a) {
	case 0:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf(" ■■  \n");
		go(52, 6);
		printf(" ■■  \n");
		go(52, 7);
		printf("        \n");
		break;

	case 1:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("  ■    \n");
		go(52, 5);
		printf("  ■    \n");
		go(52, 6);
		printf("  ■    \n");
		go(52, 7);
		printf("  ■    \n");
		break;

	case 2:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf("■■    \n");
		go(52, 6);
		printf("  ■■  \n");
		go(52, 7);
		printf("        \n");
		break;


	case 3:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf("  ■■\n");
		go(52, 6);
		printf("■■  \n");
		go(52, 7);
		printf("        \n");
		break;

	case 4:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf("■      \n");
		go(52, 6);
		printf("■■■  \n");
		go(52, 7);
		printf("        \n");
		break;

	case 5:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf("    ■  \n");
		go(52, 6);
		printf("■■■  \n");
		go(52, 7);
		printf("        \n");
		break;

	case 6:
		Draw_setcolor(Setting_blockcolor(block_desti[0]));
		go(52, 4);
		printf("        \n");
		go(52, 5);
		printf("  ■    \n");
		go(52, 6);
		printf("■■■  \n");
		go(52, 7);
		printf("        \n");
		break;
	}
}

//다음 블록 그리는 함수
void Draw_nextblock(int a)
{
	switch (a) {
	case 0:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf(" ■■  \n");
		go(52, 13);
		printf(" ■■  \n");
		go(52, 14);
		printf("        \n");
		break;

	case 1:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("  ■    \n");
		go(52, 12);
		printf("  ■    \n");
		go(52, 13);
		printf("  ■    \n");
		go(52, 14);
		printf("  ■    \n");
		break;

	case 2:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf("■■    \n");
		go(52, 13);
		printf("  ■■  \n");
		go(52, 14);
		printf("        \n");
		break;


	case 3:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf("  ■■\n");
		go(52, 13);
		printf("■■  \n");
		go(52, 14);
		printf("        \n");
		break;

	case 4:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf("■      \n");
		go(52, 13);
		printf("■■■  \n");
		go(52, 14);
		printf("        \n");
		break;

	case 5:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf("    ■  \n");
		go(52, 13);
		printf("■■■  \n");
		go(52, 14);
		printf("        \n");
		break;

	case 6:
		Draw_setcolor(Setting_blockcolor(block_next[0]));
		go(52, 11);
		printf("        \n");
		go(52, 12);
		printf("  ■    \n");
		go(52, 13);
		printf("■■■  \n");
		go(52, 14);
		printf("        \n");
		break;
	}
}

//블록의 색을 정해주는 함수
int Setting_blockcolor(int kind)
{
	switch (kind)
	{
	case 0:
		return 14;
		break;

	case 1:
		return 11;
		break;

	case 2:
		return 12;
		break;

	case 3:
		return 10;
		break;

	case 4:
		return 9;
		break;

	case 5:
		return 6;
		break;

	case 6:
		return 13;
		break;

	default:
		exit(0);
		break; //에러
	}
}

//커서를 이동시키는 함수
void go(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//글씨색 바꾸는 함수
void Draw_setcolor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//인트로 화면 드로우
void Draw_start(void)
{
	int pr, option_pr,instruction_pr,option_temp, num=0;

	//커서 지우기
	CONSOLE_CURSOR_INFO Curinfo;
	Curinfo.dwSize = 1;
	Curinfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Curinfo);
	system("cls");

	go(29, 4);
	Draw_setcolor(15);
	printf("터뜨리스");

	go(14, 7);
	Draw_setcolor(15);
	printf("────────────────────────────────────── ");

	Draw_setcolor(15);
	go(28, 10);
	printf("Game Start");

	Draw_setcolor(8);
	go(30, 13);
	printf("Option");

	Draw_setcolor(8);
	go(28, 16);
	printf("Instruction");

	Draw_setcolor(8);
	go(31, 19);
	printf("Quit");

	Draw_setcolor(6);
	go(20, 24);
	printf("▲ ▼ :Move  /  Enter : Select");


	while (1)
	{
		go(29, 4);
		Draw_setcolor(15);
		printf("터뜨리스");

		go(14, 7);
		Draw_setcolor(15);
		printf("────────────────────────────────────── ");

		Draw_setcolor(6);
		go(20, 24);
		printf("▲ ▼ :Move  /  Enter : Select");

		pr = _getch();

		if (pr == 72)
		{
			if (num > 0) { num--; }
		}
		if (pr == 80)
		{
			if (num < 3) { num++; }
		}
		if (pr == ENTER)
		{
			if (num == 0)
			{
				game_situation = 0;
				break;
			} //Game start

			if (num == 1)
			{
				system("cls");
				option_temp = 0;
				go(4, 5);
				Draw_setcolor(15);
				printf("색약 모드");

				if (option_color == 0)
				{
					go(15, 5);
					Draw_setcolor(15);
					printf("□");
				}
				else {
					go(15, 5);
					Draw_setcolor(15);
					printf("■");
				}

				go(4, 8);
				Draw_setcolor(8);
				printf("효과음");

				if (option_sfx == 0)
				{
					go(12, 8);
					Draw_setcolor(8);
					printf("□");
				}
				else {
					go(12, 8);
					Draw_setcolor(8);
					printf("■");
				}


				go(20, 24);
				Draw_setcolor(6);
				printf("▲ ▼ :Move  /  Enter : Change Option  /  Z : Exit option");

				while (1)
				{
					option_pr = _getch();

					if (option_pr == ENTER)
					{
						if (option_temp == 0)
						{
							if (option_color == 0)
							{
								option_color = 1;
							}
							else {
								option_color = 0;
							}
						}
						if (option_temp == 1)
						{
							if (option_sfx == 0)
							{
								option_sfx = 1;
							}
							else {
								option_sfx = 0;
							}
						}
					}
					if (option_pr == 'z')
					{
						system("cls");
						break;
					}

					if (option_pr == UP)
					{
						if (option_temp > 0)
						{
							option_temp--;
						}
					}
					if (option_pr == DOWN)
					{
						if (option_temp < 1)
						{
							option_temp++;
						}
					}

					go(4, 5);
					if (option_temp == 0) { Draw_setcolor(15); }
					else { Draw_setcolor(8); }
					printf("색약 모드");

					if (option_color == 0)
					{
						go(15, 5);
						if (option_temp == 0) { Draw_setcolor(15); }
						else { Draw_setcolor(8); }
						printf("□");
					}
					else {
						go(15, 5);
						if (option_temp == 0) { Draw_setcolor(15); }
						else { Draw_setcolor(8); }
						printf("■");
					}

					go(4, 8);
					if (option_temp == 1) { Draw_setcolor(15); }
					else { Draw_setcolor(8); }
					printf("효과음");

					if (option_sfx == 0)
					{
						go(12, 8);
						if (option_temp == 1) { Draw_setcolor(15); }
						else { Draw_setcolor(8); }
						printf("□");
					}
					else {
						go(12, 8);
						if (option_temp == 1) { Draw_setcolor(15); }
						else { Draw_setcolor(8); }
						printf("■");
					}

				}
			} //option

			if (num == 2)
			{
				while (1)
				{
					instruction_pr = 0;

					system("cls");

					if (option_color == 0) { Draw_setcolor(15); }
					else { Draw_setcolor(8); }
					go(3, 3);
					printf("1. 기존 테트리스와 진행방식은 동일합니다. 다만 블록을 쏠 뿐이죠.");
					go(3, 5);
					printf("2. 방향키로 블록의 목적지를 정하고, z/x 키로 회전시킵니다. 그리고 스페이스바로 쏩니다!");
					go(3, 7);
					printf("3. 위험할 땐 폭탄을 씁시다. c키를 누르면 폭탄을 쏠 수 있습니다.");
					go(3, 9);
					printf("4. 한 줄 당 10칸에 블록이 쌓이면 줄이 사라지고 점수를 얻습니다.");
					go(3, 11);
					printf("5. 5줄을 없앨 때 마다 레벨이 1씩 오릅니다.");

					go(20, 24);
					Draw_setcolor(6);
					printf("Z : Exit instruction");


					instruction_pr = _getch();

					if (instruction_pr == 'z')
					{
						system("cls");
						break;
					}

				}
			} //introduction

			if (num == 3)
			{
				system("cls");
				go(1, 1);
				exit(0);
			} //quit
		}

		if (num == 0) {	Draw_setcolor(15); } else { Draw_setcolor(8); }
		go(28, 10);
		printf("Game Start");

		if (num == 1) { Draw_setcolor(15); } else { Draw_setcolor(8); }
		go(30, 13);
		printf("Option");

		if (num == 2) { Draw_setcolor(15); } else { Draw_setcolor(8); }
		go(28, 16);
		printf("Instruction");

		if (num == 3) { Draw_setcolor(15); } else { Draw_setcolor(8); }
		go(31, 19);
		printf("Quit");
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//[[[[[기타연산]]]]]

//블록의 가로길이를 계산하는 함수
int Calcul_block_width(int kind, int rot)
{
	int res;

	switch (kind)
	{
	case 0:
		res = 2;
		break;

	case 1:
		if (rot == 0 || rot == 2) { res = 1; }
		else { res = 4; }
		break;

	case 2:
		if (rot == 0 || rot == 2) { res = 2; }
		else { res = 3; }
		break;

	case 3:
		if (rot == 0 || rot == 2) { res = 2; }
		else { res = 3; }
		break;

	case 4:
		if (rot == 0 || rot == 2) { res = 3; }
		else { res = 2; }
		break;

	case 5:
		if (rot == 0 || rot == 2) { res = 3; }
		else { res = 2; }
		break;

	case 6:
		if (rot == 0 || rot == 2) { res = 3; }
		else { res = 2; }
		break;
	}

	return res;
}

//블록의 세로길이를 계산하는 함수
int Calcul_block_height(int kind, int rot)
{
	int res;

	switch (kind)
	{
	case 0:
		res = 2;
		break;

	case 1:
		if (rot == 0 || rot == 2) { res = 4; }
		else { res = 1; }
		break;

	case 2:
		if (rot == 0 || rot == 2) { res = 3; }
		else { res = 2; }
		break;

	case 3:
		if (rot == 0 || rot == 2) { res = 3; }
		else { res = 2; }
		break;

	case 4:
		if (rot == 0 || rot == 2) { res = 2; }
		else { res = 3; }
		break;

	case 5:
		if (rot == 0 || rot == 2) { res = 2; }
		else { res = 3; }
		break;

	case 6:
		if (rot == 0 || rot == 2) { res = 2; }
		else { res = 3; }
		break;
	}

	return res;
}

//목적지 블록을 board배열 안에 넣는 함수
void Set_fake_block()
{
	if (game_situation == 0)
	{
		int i, j;

		if (block_desti[0] == 0)
		{
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					board[block_desti[2] + i][block_desti[3] + j] = 4;
				}
			}
		}

		if (block_desti[0] == 1)
		{
			if (block_desti[1] == 0 || block_desti[1] == 2) {
				for (i = 0; i < 1; i++)
				{
					for (j = 0; j < 4; j++)
					{
						board[block_desti[2] + i][block_desti[3] + j] = 4;
					}
				}
			}
			if (block_desti[1] == 1 || block_desti[1] == 3) {
				for (i = 0; i < 4; i++)
				{
					for (j = 0; j < 1; j++)
					{
						board[block_desti[2] + i][block_desti[3] + j] = 4;
					}
				}
			}
		}

		if (block_desti[0] == 2)
		{
			if (block_desti[1] == 0 || block_desti[1] == 2) {
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 0][block_desti[3] + 2] = 4;

			}
			if (block_desti[1] == 1 || block_desti[1] == 3) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 1] = 4;
			}
		}

		if (block_desti[0] == 3)
		{
			if (block_desti[1] == 0 || block_desti[1] == 2) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 2] = 4;

			}
			if (block_desti[1] == 1 || block_desti[1] == 3) {
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 0] = 4;
			}
		}

		if (block_desti[0] == 4)
		{
			if (block_desti[1] == 0) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 1] = 4;
			}
			if (block_desti[1] == 1) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 0][block_desti[3] + 2] = 4;
			}
			if (block_desti[1] == 2) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 2][block_desti[3] + 0] = 4;
				board[block_desti[2] + 2][block_desti[3] + 1] = 4;
			}
			if (block_desti[1] == 3) {
				board[block_desti[2] + 0][block_desti[3] + 2] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 2] = 4;
			}
		}

		if (block_desti[0] == 5)
		{
			if (block_desti[1] == 0) {
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 0] = 4;
			}
			if (block_desti[1] == 1) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 0][block_desti[3] + 2] = 4;
				board[block_desti[2] + 1][block_desti[3] + 2] = 4;
			}
			if (block_desti[1] == 2) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 2][block_desti[3] + 0] = 4;
			}
			if (block_desti[1] == 3) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 2] = 4;
			}
		}

		if (block_desti[0] == 6)
		{
			if (block_desti[1] == 0) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 2][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
			}
			if (block_desti[1] == 1) {
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 2] = 4;
			}
			if (block_desti[1] == 2) {
				board[block_desti[2] + 1][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
				board[block_desti[2] + 2][block_desti[3] + 1] = 4;
			}
			if (block_desti[1] == 3) {
				board[block_desti[2] + 0][block_desti[3] + 0] = 4;
				board[block_desti[2] + 0][block_desti[3] + 1] = 4;
				board[block_desti[2] + 0][block_desti[3] + 2] = 4;
				board[block_desti[2] + 1][block_desti[3] + 1] = 4;
			}
		}
	}
}

//board 배열 안에 블럭에 따라 값을 넣는 함수
void Set_value(int val)
{
	int i, j;

	if (block[0] == 0)
	{
		for (i = 0; i < 2; i++)
		{
			for (j = 0; j < 2; j++)
			{
				board[block[4] + i][block[5] + j] = val;
			}
		}
	}

	if (block[0] == 1)
	{
		if (block[1] == 0 || block[1] == 2) {
			for (i = 0; i < 1; i++)
			{
				for (j = 0; j < 4; j++)
				{
					board[block[4] + i][block[5] + j] = val;
				}
			}
		}
		if (block[1] == 1 || block[1] == 3) {
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 1; j++)
				{
					board[block[4] + i][block[5] + j] = val;
				}
			}
		}
	}

	if (block[0] == 2)
	{
		if (block[1] == 0 || block[1] == 2) {
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 0][block[5] + 2] = val;

		}
		if (block[1] == 1 || block[1] == 3) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 1] = val;
		}
	}

	if (block[0] == 3)
	{
		if (block[1] == 0 || block[1] == 2) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 2] = val;

		}
		if (block[1] == 1 || block[1] == 3) {
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 0] = val;
		}
	}

	if (block[0] == 4)
	{
		if (block[1] == 0) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 1] = val;
		}
		if (block[1] == 1) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 0][block[5] + 2] = val;
		}
		if (block[1] == 2) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 2][block[5] + 0] = val;
			board[block[4] + 2][block[5] + 1] = val;
		}
		if (block[1] == 3) {
			board[block[4] + 0][block[5] + 2] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 2] = val;
		}
	}

	if (block[0] == 5)
	{
		if (block[1] == 0) {
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 0] = val;
		}
		if (block[1] == 1) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 0][block[5] + 2] = val;
			board[block[4] + 1][block[5] + 2] = val;
		}
		if (block[1] == 2) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 2][block[5] + 0] = val;
		}
		if (block[1] == 3) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 2] = val;
		}
	}

	if (block[0] == 6)
	{
		if (block[1] == 0) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 2][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
		}
		if (block[1] == 1) {
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 2] = val;
		}
		if (block[1] == 2) {
			board[block[4] + 1][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 1][block[5] + 1] = val;
			board[block[4] + 2][block[5] + 1] = val;
		}
		if (block[1] == 3) {
			board[block[4] + 0][block[5] + 0] = val;
			board[block[4] + 0][block[5] + 1] = val;
			board[block[4] + 0][block[5] + 2] = val;
			board[block[4] + 1][block[5] + 1] = val;
		}
	}
}

//블록이 (x,y)위치일 때 실제 블록과 충돌하는지 반환하는 함수
int Collision_check(int kind, int rot, int x, int y)
{
	if (kind == 0)
	{
		if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3) { return 0; }
		else { return 1; }
	}

	if (kind == 1)
	{
		if (rot == 0 || rot == 2) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 0][y + 2] != 3 && board[x + 0][y + 3] != 3) { return 0; }
			else { return 1; }
		}

		if (rot == 1 || rot == 3) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 2][y + 0] != 3 && board[x + 3][y + 0] != 3) { return 0; }
			else { return 1; }
		}
	}

	if (kind == 2)
	{
		if (rot == 0 || rot == 2) {
			if (board[x + 1][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3 && board[x + 0][y + 2] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 1 || rot == 3) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 1][y + 1] != 3 && board[x + 2][y + 1] != 3) { return 0; }
			else { return 1; }
		}
	}

	if (kind == 3)
	{
		if (rot == 0 || rot == 2) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3 && board[x + 1][y + 2] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 1 || rot == 3) {
			if (board[x + 0][y + 1] != 3 && board[x + 1][y + 0] != 3 && board[x + 1][y + 1] != 3 && board[x + 2][y + 0] != 3) { return 0; }
			else { return 1; }
		}
	}

	if (kind == 4)
	{
		if (rot == 0) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3 && board[x + 2][y + 1] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 1) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 0][y + 2] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 2) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 2][y + 0] != 3 && board[x + 2][y + 1] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 3) {
			if (board[x + 0][y + 2] != 3 && board[x + 1][y + 0] != 3 && board[x + 1][y + 1] != 3 && board[x + 1][y + 2] != 3) { return 0; }
			else { return 1; }
		}
	}

	if (kind == 5)
	{
		if (rot == 0) {
			if (board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3 && board[x + 2][y + 1] != 3 && board[x + 2][y + 0] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 1) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 0][y + 2] != 3 && board[x + 1][y + 2] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 2) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 0] != 3 && board[x + 2][y + 0] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 3) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 1][y + 1] != 3 && board[x + 1][y + 2] != 3) { return 0; }
			else { return 1; }
		}
	}

	if (kind == 6)
	{
		if (rot == 0) {
			if (board[x + 0][y + 0] != 3 && board[x + 1][y + 0] != 3 && board[x + 2][y + 0] != 3 && board[x + 1][y + 1] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 1) {
			if (board[x + 0][y + 1] != 3 && board[x + 1][y + 0] != 3 && board[x + 1][y + 1] != 3 && board[x + 1][y + 2] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 2) {
			if (board[x + 1][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 1][y + 1] != 3 && board[x + 2][y + 1] != 3) { return 0; }
			else { return 1; }
		}
		if (rot == 3) {
			if (board[x + 0][y + 0] != 3 && board[x + 0][y + 1] != 3 && board[x + 0][y + 2] != 3 && board[x + 1][y + 1] != 3) { return 0; }
			else { return 1; }
		}
	}
}

//목적지블록의 y위치를 정하는 함수
void Place_block_desti_y(void)
{
	int i,temp;
	int comp = 0;

	for (i = 20 - Calcul_block_height(block_desti[0], block_desti[1]); i > -1; i--)
	{
		if (Collision_check(block_desti[0], block_desti[1], block_desti[2], i) == 0)
		{
			block_desti[3] = i;
			comp = 1;
			break;
		}
	}
	if (comp == 0)
	{
		temp = 1;
		for (int rot = 0; rot < 4; rot++)
		{
			if (Collision_check(block_desti[0], rot, 10, 0) == 1)
			{
				temp++;
			}
			else
			{
				block_desti[1] = rot;
			}
		}

		if (temp == 4) {
			Sleep(1000);
			game_over = 1;
		}

	}
}

//위 키 눌렀을 때 y 위치 정하는 함수
void Place_block_desti_y_up(void)
{
	int i;

	for (i = block_desti[3] - 1; i > -1; i--)
	{
		if (Collision_check(block_desti[0], block_desti[1], block_desti[2], i) == 0 && Collision_check(block_desti[0], block_desti[1], block_desti[2], i+1) == 1)
		{
			block_desti[3] = i;
			break;
		}
	}
}

//아래 키 눌렀을 때 y 위치 정하는 함수
void Place_block_desti_y_down(void)
{
	int i;

	for (i = block_desti[3] + 1; i < 21 - Calcul_block_height(block_desti[0],block_desti[1]); i++)
	{
		if (Collision_check(block_desti[0], block_desti[1], block_desti[2], i) == 0 && (Collision_check(block_desti[0], block_desti[1], block_desti[2], i + 1) == 1 || i== 20 - Calcul_block_height(block_desti[0], block_desti[1])))
		{
			block_desti[3] = i;
			break;
		}
	}
}

// 점수 자릿수 체크 함수
void Check_score_cipher(void)
{
	int temp = 1;
	len = 0;
	while (score > temp) {
		temp *= 10;
		len++;
	}
}

//폭탄의 y위치를 정하는 함수
int Place_bomb_y(int x_move)
{
	int temp = 0;
	for (int i = 0; i < 20; i++)
	{
		if (board[bomb[0] + x_move][i] == 0)
		{
			bomb[1] = i;
			temp += 1;
		}
	}
	if (temp == 0) { return 0; }
	else { return 1;  }
}