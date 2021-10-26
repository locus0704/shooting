#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>


#define LEFT 75		//좌로이동
#define RIGHT 77	//우로이동
#define UP 72		//위로이동
#define DOWN 80		//아래로 이동
#define SPACE 32	//총알발사
#define ESC 27 	//게임종료
#define ENTER 13	//

/*--------아래 매크로는 글 색깔들을 정의함. ---------*/
#define COL GetStdHandle(STD_OUTPUT_HANDLE)
#define RED SetConsoleTextAttribute(COL, 0x000c);
#define YELLOW SetConsoleTextAttribute(COL, 0x000e);
#define GREEN SetConsoleTextAttribute(COL, 0x0002);
#define SKY_BLUE SetConsoleTextAttribute(COL, 0x000b);
#define BLUE SetConsoleTextAttribute(COL, 0x0001);
#define PURPLE SetConsoleTextAttribute(COL, 0x000d);
#define WHITE SetConsoleTextAttribute(COL, 0x000f);
/*--------------------------------------------------*/
#define MAXENEMY 10
#define MAXBALL 20

#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화
#define random(n) (rand() % (n))                        //0~n까지의 난수 발생
int fx;
int bx, by;
int Score;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c) {
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
void clrscr(void) {
	COORD Cur = { 0, 0 };
	unsigned long dwLen;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Cur, &dwLen);
}

int life;

typedef struct {
	char name[10];
	int p_score;
}player;


player p_rank[11];

struct tag_Enemy {
	BOOL exist;
	int Type;
	int x, y;
	int Delta;
	int nFrame;
	int nStay;
} Enemy[MAXENEMY];

struct tag_Ball {
	BOOL exist;
	int x, y;
	int nFrame;
	int nStay;
} Ball[MAXBALL];

char* arEnemy[] = { " [-^-] "," !-X-! ", " o-@-o ", " <-&-> " };

BOOL IsKeyDown(int Key) {
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

int Title(void);
int MENU(void);
void HOW(void);
void Game(void);
void Stage(void);
void Developer(void);
void Board(player p_rank[]);

gotoxy(int x, int y)
{
	COORD pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//콘솔크기설정
void main(void) {
	system("mode con cols=95 lines=40");
	Title();

}
int Title(void) {
	SKY_BLUE
		printf("\n  ┌────────────────────────────────────────────┐\n");
	printf("  │                                                                                       ││\n");
	printf("  │     ■■■   ■    ■   ■■■    ■■■   ■■■■  ■■■■  ■    ■   ■■■      ││\n");
	printf("  │    ■        ■    ■  ■    ■  ■    ■     ■        ■     ■■  ■  ■           ││\n");
	printf("  │     ■■■   ■■■■  ■    ■  ■    ■     ■        ■     ■ ■ ■  ■  ■■     ││\n");
	printf("  │          ■  ■    ■  ■    ■  ■    ■     ■        ■     ■  ■■  ■    ■     ││\n");
	printf("  │     ■■■   ■    ■   ■■■    ■■■      ■     ■■■■  ■    ■   ■■■      ││\n");
	printf("  │                                                                                       ││\n"); YELLOW
		printf("  │                   ▣▣▣   ▣▣▣▣   ▣▣▣    ▣▣▣     ▣▣▣                     │\n");
	printf("  │                  ▣           ▣     ▣    ▣   ▣    ▣  ▣                          ││\n");
	printf("  │                   ▣▣▣      ▣     ▣▣▣▣   ▣▣▣▣   ▣▣▣                     ││\n");
	printf("  │                        ▣     ▣     ▣    ▣   ▣   ▣         ▣                    ││\n");
	printf("  │                   ▣▣▣      ▣     ▣    ▣   ▣    ▣   ▣▣▣                     ││\n");
	printf("  │                                                                                       ││\n");
	printf("  │ Ver 1.0                                   ## Made By shooting stars' ##               ││\n");
	printf("  │                                             kakao talk ID :hottest0904                ││\n");
	printf("  └─────────────────────────────────────────────┘\n");
	gotoxy(0, 20);
	Sleep(500);
	WHITE
		int SELECT;
	int i;
	while (1) {
		SELECT = MENU();
		switch (SELECT) {
		case 0://how to plqy
			system("cls");
			HOW();
			break;

		case 1://start game
			for (i = 3; i > 0; i--) {
				gotoxy(65, 24);
				printf("Start in..%d", i);
				Sleep(1000);
			}
			system("cls");
			Game();
			break;

		case 2://developer information
			Developer();
			break;

		case 3://exit
			system("cls");
			gotoxy(40, 10); RED;
			printf("게임을 종료합니다...");
			gotoxy(40, 20);
			WHITE;
			exit(0);
		}
	}

}
//메뉴 옵션 선택
int MENU(void) {
	const x = 40;
	int y = 30;
	char input;

	gotoxy(x, y - 2);
	printf("    MENU");
	gotoxy(x, y);
	printf(">");
	printf(" HOW TO PLAY");
	gotoxy(x, y + 1);
	printf("  START GAME");
	gotoxy(x, y + 2);
	printf("  DEVELOPER");
	gotoxy(x, y + 3);
	printf("  EXIT");

	while (1) {
		gotoxy(x, y);
		printf("");
		input = getch();
		printf(" ");

		switch (input) {
		case UP:
			if (y > 30) y--;
			break;

		case DOWN:
			if (y < 33) y++;
			break;

		case ENTER:
			return y - 30;
		}
		gotoxy(x, y);
		printf(">");
	}

}
//게임방법 설명
void HOW(void) {
	printf("why???");
	system("cls");
	gotoxy(5, 5);
	printf("\t\tHOW TO PLAY\n");
	printf("\t\t\t\t\t\t\n");
	printf("\t1. 방향은 키보드 방향키로 조절합니다  ←→\n");
	printf("\t2. 스페이스 바를 누르면 총알이 발사됩니다\n");
	printf("\t3. 적(enemy)이 발사하는 총알을 피하고 총알을 발사해 적과 싸우는 게임입니다\n");
	printf("\t4. 점수가 높아질수록 LEVEL이 높아집니다\n");
	printf("\t5. 적이 쏘는 총알을 맞으면 HP가 줄어듭니다\n\n");
	printf("\t※게임중 ESC키를 누르면 종료합니다\n\n");
	YELLOW
		printf("\t\t 점수(SCORE) : 피한 총알 개수 + 사격\n");
	printf("\t\t 목숨(LIFE)  : ♥\n");
	printf("\t\t 체력(HP)    : ■\n");

	WHITE
		Sleep(200);
	gotoxy(40, 20);
	printf("Pleas Press ENTER to go Back...");
	getch();
	system("cls");
	Title();
}
//게임화면
void Game() {
	setcursortype(NOCURSOR); //커서 없애는 함수
	system("cls");
	gotoxy(40, 15); printf(" start game!"); Sleep(500);
	gotoxy(40, 15); printf("             ");

	Stage();
	Board(p_rank);

}
void Board(player p_rank[]) {

	for (int j = 0; j < 3; j++) {
		setcursortype(NOCURSOR);
		gotoxy(40, 20); printf("GAME OVER!"); Sleep(200);
		gotoxy(40, 20); printf("           "); Sleep(200);
	}

	FILE* file;
	char str[10];

	system("cls");
	gotoxy(30, 20); printf("이름(닉네임)을 입력하세요 : ");
	scanf("%s", str);
	strcpy(p_rank[10].name, str);
	p_rank[10].p_score = Score;



	//fileload 저장되었는 데이터 불러옴
	file = fopen("scoreboard.txt", "rt");
	if (file == NULL) {
		file = fopen("scoreboard2.txt", "a");
		fclose(file);
		return;
	}
	for (int i = 0; i < 10; i++) {
		fscanf(file, "%d %s\n", &p_rank[i].p_score, &p_rank[i].name);

	}

	//ranksort 점수에 따라 순위 정렬
	int i, j, cnt = 0;
	player temp;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 11; j++) {
			if (p_rank[j].p_score < p_rank[j + 1].p_score) {
				temp = p_rank[j];
				p_rank[j] = p_rank[j + 1];
				p_rank[j + 1] = temp;
			}
		}
	}


	//filesave 10위까지의 기록 저장
	file = fopen("scoreboard.txt", "wt");
	for (i = 0; i < 10; i++) {
		if (p_rank[i].p_score == 0) //점수가 0점이면
			fprintf(file, "0 ---\n");
		else
			fprintf(file, "%d %s\n", p_rank[i].p_score, p_rank[i].name); //fprintf(savefile, "%d %d %s\n", rank[i].time, rank[i].score, rank[i].name);
	}
	fclose(file);


	gotoxy(40, 13); printf("점수 계산 중..."); Sleep(2000); system("cls");
	gotoxy(40, 2); printf("HIGH SCORES");
	gotoxy(30, 4); printf("RANK   PLAYER NAME	SCORE");
	gotoxy(27, 5); printf("───────────────────────────────────");
	
	const x = 30;
	for (int i = 0; i < 10; i++) {
		if (i == 0) YELLOW; 
		if(i==1) WHITE;
		gotoxy(x, 6+i*2);
		printf("[%d위]", i + 1);
		gotoxy(x + 8, 6 + i*2);
		printf("닉네임: %s", p_rank[i].name);
		gotoxy(x + 26, 6 + i*2);
		printf("점수: %d", p_rank[i].p_score);
		/*gotoxy(x + 40, (i + 1) * 3 + 2);
		printf("시간: %d초", rank[i].time);
		*/
		Sleep(200);
	}

	gotoxy(30, 28);	printf("Pleas Press ANY BUTTON to Play Continue...");

	fclose(file);//close file

	getch();
	system("cls");
	Title();
}
void Stage() {

	int ch;
	int i, j;
	BOOL bFound;
	int count;

	setcursortype(NOCURSOR);

	fx = 45;
	bx = -1;
	Score = 0;
	life = 3;

	Sleep(100);
	system("cls");

	randomize();
	clrscr();
	//stage1
	for (int j = 0; j < 3; j++) {
		gotoxy(40, 15); printf("Stage 1"); Sleep(150);
		gotoxy(40, 15); printf("       "); Sleep(150);
	}


	for (count = 0;; count++) {
		// 좌우 이동 처리
		if (count % 5 == 0) {
			if (IsKeyDown(VK_LEFT)) {
				if (fx > 3) fx--;
			}
			if (IsKeyDown(VK_RIGHT)) {
				if (fx < 90) fx++;
			}
		}
		// 키 입력 처리
		if (kbhit()) {
			ch = getch();
			if (ch == 0xE0 || ch == 0) {
				getch();
			}
			else {
				switch (ch) {
				case ' ':
					if (bx == -1) {
						bx = fx;
						by = 31;
					}
					break;
				case ESC:
					goto end;
				}
			}
		}
		// 적군 생성
		if (random(100) == 0) {
			for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; }
			if (i != MAXENEMY) {
				if (random(2) == 1) {
					Enemy[i].x = 5;
					Enemy[i].Delta = 1;
				}
				else {
					Enemy[i].x = 90;
					Enemy[i].Delta = -1;
				}
				for (;;) {
					Enemy[i].y = random(10) + 1;
					for (bFound = FALSE, j = 0; j < MAXENEMY; j++) {
						if (Enemy[j].exist == TRUE && Enemy[j].y == Enemy[i].y) {
							bFound = TRUE;
							break;
						}
					}
					if (bFound == FALSE) {
						break;
					}
				}
				Enemy[i].nFrame = Enemy[i].nStay = random(6) + 1;
				Enemy[i].Type = random(sizeof(arEnemy) / sizeof(arEnemy[0]));
				Enemy[i].exist = TRUE;
			}
		}


		//stage2
		if (Score > 5) {//test 위해서 작게 잡은거
			if (Score == 6) {
				for (int j = 0; j < 3; j++) {
					gotoxy(40, 15); printf("Stage 2");
					gotoxy(35, 17); printf("SPACE바 이용하여 총쏘기 가능"); Sleep(200);
					gotoxy(40, 15); printf("       ");
					gotoxy(35, 17); printf("                             "); Sleep(200);
				}
				Score++;
			}
			// 아군 총알 이동 및 출력
			if (bx != -1) {
				gotoxy(bx, by); putch(' ');
				if (by == 0) {
					bx = -1;
				}
				else {
					by--;
					gotoxy(bx, by); putch('i');
				}
			}

			// 적군과 아군 총알의 충돌 판정
			for (i = 0; i < MAXENEMY; i++) {
				if (Enemy[i].exist == FALSE) continue;
				if (Enemy[i].y == by && abs(Enemy[i].x - bx) <= 2) {
					gotoxy(bx, by); putch(' ');
					bx = -1;
					Enemy[i].exist = FALSE;
					gotoxy(Enemy[i].x - 3, Enemy[i].y);
					puts("       ");
					Score += 7 - Enemy[i].nFrame;
					break;
				}
			}

			//stage3    
			if (Score > 20) {
				if (Score == 21) {
					for (int j = 0; j < 3; j++) {
						gotoxy(40, 15); printf("Stage 3"); Sleep(2); Sleep(200);
						gotoxy(40, 15); printf("       "); Sleep(2); Sleep(200);
					}Score++;
				}
				// 적군 생성
				if (random(100) == 0) {
					for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; }
					if (i != MAXENEMY) {
						if (random(2) == 1) {
							Enemy[i].x = 5;
							Enemy[i].Delta = 1;
						}
						else {
							Enemy[i].x = 90;
							Enemy[i].Delta = -1;
						}
						for (;;) {
							Enemy[i].y = random(10) + 1;
							for (bFound = FALSE, j = 0; j < MAXENEMY; j++) {
								if (Enemy[j].exist == TRUE && Enemy[j].y == Enemy[i].y) {
									bFound = TRUE;
									break;
								}
							}
							if (bFound == FALSE) {
								break;
							}
						}
						Enemy[i].nFrame = Enemy[i].nStay = random(6) + 1;
						Enemy[i].Type = random(sizeof(arEnemy) / sizeof(arEnemy[0]));
						Enemy[i].exist = TRUE;
					}
				}
				// 적군 총알 이동
				for (i = 0; i < MAXBALL; i++) {
					if (Ball[i].exist == FALSE) continue;
					if (--Ball[i].nStay == 0) {
						Ball[i].nStay = Ball[i].nFrame;
						gotoxy(Ball[i].x, Ball[i].y); putch(' ');
						if (Ball[i].y >= 31) {
							Ball[i].exist = FALSE;
							Score++;//적 총알 피하면 점수+
						}
						else {
							Ball[i].y++;
							gotoxy(Ball[i].x, Ball[i].y); putch('*');
						}
					}
				}
				// 적군 이동 및 출력
				for (i = 0; i < MAXENEMY; i++) {
					if (Enemy[i].exist == FALSE) continue;
					if (--Enemy[i].nStay == 0) {
						Enemy[i].nStay = Enemy[i].nFrame;
						if (Enemy[i].x >= 91 || Enemy[i].x <= 4) {
							Enemy[i].exist = FALSE;
							gotoxy(Enemy[i].x - 3, Enemy[i].y);
							puts("       ");
						}
						else {
							Enemy[i].x += Enemy[i].Delta;
							gotoxy(Enemy[i].x - 3, Enemy[i].y);
							puts(arEnemy[Enemy[i].Type]);
							// 총알 발사
							if (random(40) == 0) {
								for (j = 0; j < MAXBALL && Ball[j].exist == TRUE; j++) { ; }
								if (j != MAXBALL) {
									Ball[j].x = Enemy[i].x + 2;
									Ball[j].y = Enemy[i].y + 1;
									Ball[j].nFrame = Ball[j].nStay = Enemy[i].nFrame * 6;
									Ball[j].exist = TRUE;
								}
							}
						}
					}
				}
			}//stage3 끝
		}
		// 적군 총알 이동
		for (i = 0; i < MAXBALL; i++) {
			if (Ball[i].exist == FALSE) continue;
			if (--Ball[i].nStay == 0) {
				Ball[i].nStay = Ball[i].nFrame;
				gotoxy(Ball[i].x, Ball[i].y); putch(' ');
				if (Ball[i].y >= 31) {
					Ball[i].exist = FALSE;
					Score++;//적 총알 피하면 점수+
				}
				else {
					Ball[i].y++;
					gotoxy(Ball[i].x, Ball[i].y); putch('*');
				}
			}
		}
		// 적군 총알과 아군의 충돌 판정
		for (i = 0; i < MAXBALL; i++) {
			if (Ball[i].exist == FALSE) continue;
			if (Ball[i].y == 31 && abs(Ball[i].x - fx) <= 2) {
				Sleep(200); life--;
				gotoxy(20, 20); Ball[i].y++; Sleep(100);
				if (life == 2) {
					Ball[i].y++;
				}
				if (life == 1) {
					Ball[i].y++;
				}
				if (life == 0) {
					gotoxy(60, 36); printf("│  LIFE : ♡♡♡\t\t│");

					gotoxy(fx - 3, 29); puts("   .   ");
					gotoxy(fx - 3, 30); puts(" .  . .");
					gotoxy(fx - 3, 31); puts("..:V:..");
					Ball[i].exist = FALSE;
					Sleep(500); goto end;
				}

				continue;
			}
		}
		// 적군 이동 및 출력
		for (i = 0; i < MAXENEMY; i++) {
			if (Enemy[i].exist == FALSE) continue;
			if (--Enemy[i].nStay == 0) {
				Enemy[i].nStay = Enemy[i].nFrame;
				if (Enemy[i].x >= 91 || Enemy[i].x <= 4) {
					Enemy[i].exist = FALSE;
					gotoxy(Enemy[i].x - 3, Enemy[i].y);
					puts("       ");
				}
				else {
					Enemy[i].x += Enemy[i].Delta;
					gotoxy(Enemy[i].x - 3, Enemy[i].y);
					puts(arEnemy[Enemy[i].Type]);
					// 총알 발사
					if (random(40) == 0) {
						for (j = 0; j < MAXBALL && Ball[j].exist == TRUE; j++) { ; }
						if (j != MAXBALL) {
							Ball[j].x = Enemy[i].x + 2;
							Ball[j].y = Enemy[i].y + 1;
							Ball[j].nFrame = Ball[j].nStay = Enemy[i].nFrame * 6;
							Ball[j].exist = TRUE;
						}
					}
				}
			}
		}

		//비행기 및 점수 출력
		gotoxy(fx - 3, 31);
		puts(" <<A>> ");
		gotoxy(0, 32); printf("▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨\n");
		printf("▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨\n");
		printf("▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨\n");
		gotoxy(0, 35);	printf("==============================================================================================\n");
		printf("│ SCORE :  %05d \t\t", Score); //score
		printf("│   HP  :\t\t");
		gotoxy(60, 36); if (life == 3) printf("│  LIFE : ♥♥♥\t\t│");
		else if (life == 2) printf("│  LIFE : ♥♥♡\t\t│");
		else if (life == 1) printf("│  LIFE : ♥♡♡\t\t│");
		// 초당 100 프레임
		Sleep(1);
		}
end:
	setcursortype(NORMALCURSOR);

}
//개발자 정보
void Developer(void) {

	system("cls");
	gotoxy(25, 9); GREEN;
	printf("제작자 : shooting stars\'\n");
	printf("\t\t\t카카오톡 ID : hottest0904\n");
	printf("\t\t\tgithub : github.com/locus0704/shooting.git\n\n");
	Sleep(200);
	WHITE;
	gotoxy(30, 20);
	printf("Pleas Press ENTER to go Back...");
	getch();
	system("cls");
	Title();
}

