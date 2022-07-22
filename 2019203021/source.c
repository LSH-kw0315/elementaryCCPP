#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
typedef enum cardnum {
	ace=1,two,three,four,five,six,seven,eight,nine,ten,jack,queen,king
}CARDNUM;
typedef enum cardshape {
	heart,diamond,clover,spade
}CARDSHAPE;
typedef struct card {
	CARDNUM num;
	CARDSHAPE shape;
	struct card* ptr;
}CARD;
typedef struct info {
	int bankroll;
	int result;
	int id;
}INFO;
typedef struct gameinfo {
	int split;
	int insurance;
	int bet;
	int state[2];
	//플레이어 상태 1=기본 2=게임 참여 상태에서 종료 3=게임을 모종의 이유로 먼저 끝냄 4=버스트 10=돈 없는 상태
	int id;
	CARD hand[2][7];
	int score[2][7];
}GAMEINFO;
char* num[13] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };//문자로 치환
char* shape[4] = { "♥","◆","♣","♠" };//문자로 치환
CARD* allocationsrc[156];//동적 할당한 카드의 원본을 저장한다.
CARD empty = { 0,0,NULL };//공백 카드. 카드가 없음을 의미함.
void clear_inputbuf();
void file_read(INFO* player);
void game_save(INFO* player);
void initialize(GAMEINFO* player);
int menu();
void show_info(INFO* player);
void guide();
void loan(INFO* player);
void bet(INFO* info, GAMEINFO* player, int i);
CARD* makecard(int i);
void makedeck(CARD* ptr, CARD** head, int i);
CARD* getcard(CARD* head, int i);
CARD drawcard(CARD* finder, int* remain, int jud, CARD** head);
void checkdeck(CARD** head, int* card_remain);
void dividecard(GAMEINFO* player, int* remain, CARD** head);
void divecard_dealer(CARD* dealer, int* remain, CARD** head);
void first_open(CARD dealer[], GAMEINFO* n);
int ace_decide_user();
int ace_decide_cpu(int score[2][7], int split, int n);
void scorelist(GAMEINFO* player, int split);
int score_sum(GAMEINFO* player, int split);
int dealer_score(CARD dealer[]);
void cpu_judge(CARD dealer[], GAMEINFO player[],int* comp_num);
void action_user_normal(GAMEINFO* player, INFO* info, CARD dealer[], int* card_remain, CARD** head);
void action_cpu_normal(GAMEINFO* player, INFO* info, CARD dealer[], int* card_remain, CARD** head);
void split_init(GAMEINFO* player);
void split_draw(GAMEINFO* player, CARD** head, int* remain,CARD dealer[]);
void splitgame_user(GAMEINFO* player, INFO* info, CARD dealer[], int* card_remain, CARD** head);
void splitgame_cpu(GAMEINFO* player, INFO* info, CARD dealer[], int* card_remain, CARD** head);
void dealer_draw(CARD dealer[], int* card_remain, CARD** head);
void insurance_money(GAMEINFO* player, INFO* info, CARD dealer[]);
void money_split(GAMEINFO* gameinfo, INFO* playerinfo, CARD dealerinfo[],int result[]);
void money_normal(GAMEINFO* gameinfo, INFO* playerinfo, CARD dealerinfo[],int* result);
void last_open(CARD dealer[], GAMEINFO* n);
void split_resultoutput(int result[], GAMEINFO* player);
void normal_resultoutput(int result, GAMEINFO* player);
void clearconsole();
void dealerinitialize(CARD dealer[]);
int main(void) {
	CARD* head = NULL;
	INFO player_info[3];
	GAMEINFO player_game[3];
	CARD dealer[7];
	int card_remain = 156;
	file_read(player_info);
	while (1) {
		initialize(player_game);
		dealerinitialize(dealer);
		srand((unsigned int)time(NULL));
		int input = menu();
		if (input == 1) {
			clearconsole();
			//배팅
			for (int i = 0;i < 3;i++) {
				if (player_game[0].state[0] == 10) {
					printf("유저의 배팅금액이 부족해 게임이 무산되었습니다. \n");
					break;
				}
				bet(&player_info[i], &player_game[i], i);
				printf("%d번 플레이어의 베팅 금액: %d$ \n", player_game[i].id, player_game[i].bet);
			}
			Sleep(3000);
			clearconsole();
			//플레이어 카드 나눠주기
			if (player_game[0].state[0] != 10) {
				for (int i = 0;i < 3;i++) {
					dividecard(&player_game[i], &card_remain, &head);
				}
				//딜러 카드 나눠주기
				checkdeck(&head, &card_remain);
				divecard_dealer(dealer, &card_remain, &head);
				int i = 0;
				//카드 오픈
				first_open(dealer, player_game);
				//행동
				action_user_normal(&player_game[0], &player_info[0], dealer, &card_remain, &head);
				first_open(dealer, player_game);
				Sleep(5000);
				clearconsole();
				action_cpu_normal(&player_game[1], &player_info[1], dealer, &card_remain, &head);
				first_open(dealer, player_game);
				Sleep(5000);
				clearconsole();
				action_cpu_normal(&player_game[2], &player_info[2], dealer, &card_remain, &head);
				first_open(dealer, player_game);
				Sleep(5000);
				clearconsole();
				if (player_game[0].split == 1) {
					split_init(&player_game[0]);
					split_draw(&player_game[0], &head, &card_remain,dealer);
					splitgame_user(&player_game[0], &player_info[0], dealer, &card_remain, &head);
					last_open(dealer, player_game);
					Sleep(5000);
					clearconsole();
				}
				if (player_game[1].split == 1) {
					split_init(&player_game[1]);
					split_draw(&player_game[1], &head, &card_remain, dealer);
					splitgame_cpu(&player_game[1], &player_info[1], dealer, &card_remain, &head);
					last_open(dealer, player_game);
					Sleep(5000);
					clearconsole();
				}
				if (player_game[2].split == 1) {
					split_init(&player_game[2]);
					split_draw(&player_game[2], &head, &card_remain, dealer);
					splitgame_cpu(&player_game[2], &player_info[2], dealer, &card_remain, &head);
					last_open(dealer, player_game);
					Sleep(5000);
					clearconsole();
				}
				//딜러 카드 전부 공개
				last_open(dealer, player_game);
				Sleep(5000);
				//딜러 행동 결정
				while (1) {
					clearconsole();
					if (dealer_score(dealer) <= 16) {
						printf("16 이하이므로, 카드를 한 장 뽑습니다. \n");
						dealer_draw(dealer, &card_remain, &head);
						last_open(dealer, player_game);
						Sleep(3000);
					}
					else
						break;
				}
				last_open(dealer, player_game);
				//배팅 판정
				for (int i = 0;i < 3;i++) {
					int split_res[2] = { 7,7 };
					int normal_res = 7;
					if (player_game[i].insurance != 0)
						insurance_money(&player_game[i], &player_info[i], dealer);
					if (player_game[i].split == 1 && (player_game[i].state[0]==2 || player_game[i].state[1]==2)) {
						money_split(&player_game[i], &player_info[i], dealer,split_res);
						split_resultoutput(split_res, &player_game[i]);
					}
					else if (player_game[i].state[0] == 2) {
						money_normal(&player_game[i], &player_info[i], dealer,&normal_res);
						normal_resultoutput(normal_res, &player_game[i]);
					}
					if (player_game[i].state[0] == 4 && player_game[i].split == 0)//버스트된 경우는 위에서 판정을 안해준다. 따라서 이와같이.
						printf("%d번 플레이어는 버스트되셨습니다.\n\n",player_game[i].id);
					else if (player_game[i].state[0] == 4 && player_game[i].state[1] == 4)//둘 다 버스트된 경우는 판정을 안해준다. 따라서 이와같이.
						printf("%d번 플레이어는 1열 2열 모두 버스트되셨습니다.\n\n",player_game[i].id);
				}
				Sleep(5000);
				game_save(player_info);//게임 완료 시마다 게임내역 저장
				clearconsole();
			}
		}
		else if (input == 2) {
			clearconsole();
			show_info(&player_info[0]);
		}
		else if (input == 3) {
			clearconsole();
			for (int i = 1;i <= 2;i++) 
				show_info(&player_info[i]);
		}
		else if (input == 4) {
			clearconsole();
			guide();
		}
		else if (input == 5) {
			clearconsole();
			loan(&player_info[0]);
		}
		else if (input == 6) {
			game_save(player_info);
			break;
		}
		else {
			printf("다시 입력하십시오 \n\n");
			clear_inputbuf();
			clearconsole();
		}
	}
	return 0;
}
void clear_inputbuf() {
	while (getchar() != '\n');
}
void file_read(INFO * player) {
	FILE* reader = fopen("playerinfo.bin", "rb");
	if (reader == NULL) {
		for (int i = 0;i < 3;i++) {
			(player + i)->bankroll = 100;
			(player + i)->result = 0;
			(player + i)->id = i + 1;
		}
	}
	else {
		for (int i = 0;i < 3;i++) {
			fread((void*)(player + i), sizeof * (player + i), 1, reader);
			printf("%d번 플레이어 정보: %d %d %d \n", i + 1, (player + i)->bankroll, (player + i)->result, (player + i)->id);
		}
		fclose(reader);
	}
}
void game_save(INFO * player) {
	FILE* writer = fopen("playerinfo.bin", "wb");
	for (int i = 0;i < 3;i++)
		fwrite((void*)(player + i), sizeof * (player + i), 1, writer);
	fclose(writer);
}
void initialize(GAMEINFO * player) {
	for (int i = 0;i < 3;i++) {
		(player + i)->bet = 0;
		(player + i)->insurance = 0;
		(player + i)->state[0] = 0;
		(player + i)->state[1] = 0;
		(player + i)->split = 0;
		for (int n = 0;n < 2;n++) {
			for (int k = 0;k < 7;k++) {
				(player + i)->hand[n][k] = empty;
				(player + i)->score[n][k] = 0;
			}
		}

	}
}
int menu() {
	int input;
	printf("========================\n");
	printf("1.게임 시작하기\n");
	printf("2.내 정보 확인하기\n");
	printf("3.다른 유저 정보 확인하기\n");
	printf("4.게임 룰/용어 가이드\n");
	printf("5.파산 회생\n");
	printf("6.게임 종료\n");
	printf("메뉴를 선택하여 주십시오:");
	scanf("%d", &input);
	return input;
}
void show_info(INFO * player) {
	printf("========================\n");
	if (player->id == 1) {
		printf("내 정보입니다 \n");
	}
	else
		printf("%d번 컴퓨터 플레이어의 정보입니다. \n", player->id);
	printf("소지금:%d$ \n전적:%d$ \n\n", player->bankroll, player->result);
}
void guide() {
	while (1) {
		int input;
		printf("게임에 대한 전반적인 가이드를 해드립니다. \n");
		printf("1.게임 과정 보기 \n2.용어 보기 \n3.게임 규칙 보기 \n0.그만 보기\n메뉴를 선택하세요:");
		scanf("%d", &input);
		if (input == 1) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("1.모든 플레이어는 게임 시작 전 배팅을 합니다. \n");
			printf("2.게임이 시작되면, 각 플레이어는 앞면이 공개된 카드를 2장씩 받습니다. \n");
			printf("3.딜러는 뒤집어놓은 카드 2장을 받고, 그 직후 바로 한 장을 앞면이 보이도록 놓습니다. \n");
			printf("4.각 플레이어는 자유롭게 행동을 합니다. \n");
			printf("5.모든 플레이어가 행동을 마쳤다면, 딜러는 나머지 카드를 공개합니다. \n");
			printf("6.딜러는 규칙에 따라 행동을 선택합니다. 행동을 마쳤다면, 게임은 끝납니다. \n");
			printf("종료 후 플레이어와 딜러의 점수 차이에 따라 돈을 받게 됩니다. \n");
			printf("\n\n ***********************************\n");
		}
		else if (input == 2) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("버스트: 가진 카드들의 숫자 합이 21을 넘는 상태를 말합니다. \n\n");
			printf("블랙잭: 내가 처음 받은 2장의 카드 조합이 ACE와 10,J,K,Q 중 하나인 상태를 말합니다.\n\n");
			printf("더블 다운: 카드를 무조건 한 장 뽑고, 배팅 금액을 2배로 하는 행위를 말합니다.\n만약 히트를 한 상태라면 이 행위를 할 수 없습니다.\n\n");
			printf("히트: 카드를 한 장 더 뽑는 행위를 말합니다. 몇 번이고 자유롭게 뽑을 수 있습니다.\n단, 버스트, 더블 다운, 서렌더, 스테이, 이븐 머니, 블랙잭 상태면 할 수 없습니다 \n\n");
			printf("서렌더: 항복하는 행위를 말합니다. 배팅 금액의 절반을 돌려받고 게임에서 탈락합니다. \n\n");
			printf("스테이: 카드를 뽑지 않고 이 손패를 유지하겠다는 선언입니다.\n게임에는 계속 참가한 상태이지만, 더 이상의 행동은 할 수 없습니다. \n\n");
			printf("스플릿: 손패가 원 페어인 상황에서, 배팅 금액을 한 번 더 내고 각 카드로 독립된 게임을 하겠다고 선언하는 것입니다. \n예를 들면, 8/8 조합으로 카드를 2장 받았다면, 16점이 아니라 8점과 8점으로 게임을 진행할 수 있습니다.\n\n");
			printf("이븐 머니: 내가 블랙잭이고 딜러가 처음 보여준 카드가 ACE라면,\n무승부가 나올 가능성이 있기 때문에 블랙잭의 이점을 포기하는 대신 배팅 금액의 2배를 돌려 받는 옵션입니다.\n\n");
			printf("인슈어런스: 만약 딜러가 처음 보여준 카드가 ACE라면,\n딜러가 블랙잭이 나올 가능성이 있기 때문에 배팅 금액의 절반만큼의 보험을 들어두는 옵션입니다.\n만약 성공한다면 보험금의 2배를 주므로, 본전을 보게 됩니다.\n\n");
			printf("\n\n **********************************\n");
		}
		else if (input == 3) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("0. 기본적으로 카드 숫자의 합이 큰 쪽이 승리합니다. \n");
			printf("1. 기본 배당율은 2배입니다. \n");
			printf("2. 블랙잭으로 승리하면 배당율이 2.5배입니다. \n");
			printf("3. 블랙잭과 블랙잭이 아닌 21과 맞붙으면 블랙잭이 무조건 승리합니다. \n");
			printf("4. 숫자의 합이 같거나, 서로 블랙잭이면 무승부입니다. 이 때는 배팅 금액만 돌려 받습니다. \n");
			printf("5. 내가 버스트 되면 무조건 패배이며, 상대가 버스트이면 무조건 승리입니다. \n");
			printf("6. 스플릿을 했을 땐 더블 다운을 하실 수 없습니다. \n");
			printf("7. 스플릿은 게임당 한 번만 사용하실 수 있습니다. \n");
			printf("8. 스플릿을 하셨다면 두 장으로 21을 만드시더라도 블랙잭이 아닙니다. \n");
			printf("9. ACE는 1점 또는 11점으로 사용될 수 있습니다. 뽑은 시점에서 몇 점으로 쓸지 결정할 수 있습니다. \n");
			printf("10. 10,J,Q,K는 모두 10점으로 처리합니다.\n");
			printf("11. 게임에 사용되는 카드는 156장입니다.\n");
			printf("12. 딜러는 처음 받은 카드 2장이 합이 16 이하이면 무조건 히트, 17 이상이면 무조건 스테이해야합니다. \n");
			printf("13. 파산 회생 신청 시 소지금이 100달러가 되도록 돈을 받지만, 전적도 그만큼 까이게 됩니다. \n");
			printf("\n\n ***********************************\n");

		}
		else if (input == 0) 
			break;
		else {
			clearconsole();
			printf("0,1,2,3만 입력할 수 있습니다. \n");
		}
	}
}
void loan(INFO * player) {
	if (player->bankroll < 10 && player->id == 1) {
		player->result -= (100 - player->bankroll);
		player->bankroll = 100;
		printf("잔고가 100달러가 될만큼만 넣어드렸습니다. 다신 도박하지 마세요. \n");
	}
	else if (player->bankroll >= 10 && player->id == 1)
		printf("저희는 최소 배팅 금액도 없는 분한테만 지원해드립니다. \n");
	else if (player->bankroll < 10 && player->id != 1) {
		player->result -= (100 - player->bankroll);
		player->bankroll = 100;
	}
}
void bet(INFO * info, GAMEINFO * player, int i) {
	int input;
	if (info->id == 1) {
		if (info->bankroll < 10) {
			printf("최소 배팅 금액도 없으시면 게임에 참여할 수 없습니다. \n");
			player->state[0] = 10;
			return;
		}
		else if (info->bankroll >= 10) {
			while (1) {
				printf("배팅 금액을 입력하세요(최소 10$):");
				if (scanf("%d", &input) != 1) {
					printf("숫자만 입력하실 수 있습니다.\n");
					clear_inputbuf();
				}
				else if (input > info->bankroll)
					printf("배팅 금액이 소지금보다 많습니다. \n");
				else if (input < 10)
					printf("적어도 10$만큼은 배팅하셔야 합니다. \n");
				else {
					info->bankroll -= input;
					player->bet = input;
					player->state[0] = 1;
					player->id = i + 1;
					break;
				}
			}
		}
	}
	else if (player->id != 1) {
		if (info->bankroll < 10)
			loan(info);
		while (1) {
			input = rand() % 31 + 10;
			if (info->bankroll >= input) {
				info->bankroll -= input;
				player->bet = input;
				player->state[0] = 1;
				player->id = i + 1;
				break;
			}
		}
	}

}
CARD* makecard(int i) {
	CARD* p;
	p = (CARD*)malloc(sizeof(CARD));
	p->num = i % 13 + 1;
	p->shape = i / 3 / 13;
	p->ptr = NULL;
	allocationsrc[i]=p;
	return p;
}
void makedeck(CARD * ptr, CARD * *head, int i) {
	CARD* p = *head;
	if (p == NULL)
		* head = ptr;
	else {
		while (p->ptr != NULL)
			p = p->ptr;
		p->ptr = ptr;
	}
}
CARD* getcard(CARD * head, int i) {
	CARD* p = head;
	while (p->ptr != NULL && i > 1) {
		i--;
		p = p->ptr;
	}
	return p;
}
CARD drawcard(CARD * finder, int* remain, int jud, CARD * *head) {
	CARD card;
	if (jud == 0) {
		CARD* p = *head;
		card.num = p->num;
		card.shape = p->shape;
		*head = p->ptr;
	}
	else {
		card.num = finder->ptr->num;
		card.shape = finder->ptr->shape;
		finder->ptr = (finder->ptr)->ptr;
	}
	*remain -= 1;
	return card;
}
void checkdeck(CARD * *head, int* card_remain) {
	CARD* p = *head;
	if (*card_remain<=0 || p==NULL) {
		*card_remain = 156;
		for (int i = 0;i < 156;i++) {
			if (allocationsrc[i] != NULL)
				free(allocationsrc[i]);
		}
		for (int i = 0;i < 156;i++)
			makedeck(makecard(i), head, i);
	}
}
void dividecard(GAMEINFO * player, int* remain, CARD * *head) {
	for (int n = 0;n < 2;n++) {
		checkdeck(head, remain);
		int card_order = rand() % (*remain);
		player->hand[0][n] = drawcard(getcard(*head, card_order), remain, card_order, head);
	}
}
void divecard_dealer(CARD * dealer, int* remain, CARD * *head) {
	for (int i = 0;i < 2;i++) {
		checkdeck(head, remain);
		int card_order = rand() % (*remain);
		dealer[i] = drawcard(getcard(*head, card_order), remain, card_order, head);
	}
}
void first_open(CARD dealer[], GAMEINFO * n) {
	printf("===========================\n");
	printf("딜러가 오픈한 카드는 다음과 같습니다. \n");
	printf("%s %s \n\n", num[(dealer[0].num) - 1], shape[dealer[0].shape]);
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 2;j++) {
			if (n->state[j] != 0) {
				printf("%d번 플레이어의 %d열 카드 \n", i + 1, j + 1);
				for (int k = 0;k < 7;k++) {
					if (n->hand[j][k].num != 0) {
						printf("%s %s ", num[(n->hand[j][k].num) - 1], shape[n->hand[j][k].shape]);
					}
				}
				scorelist(n, j);
				printf("\n\n");
			}
		}
		n++;
	}
}
int ace_decide_user() {
	int input;
	while (1) {
		printf("\nACE를 1점과 11점 중 어느 점수로 쓰시겠습니까? \n");
		printf("0: Ace를 1점으로 쓴다 1: Ace를 11점으로 쓴다 \n");
		printf("입력:");
		if (scanf("%d", &input) != 1) {
			printf("숫자만 입력하세요");
			clear_inputbuf();
		}
		else if (input != 0 && input != 1)
			printf("0 아니면 1만 입력하실 수 있습니다. \n");
		else if (input == 0) {
			return 1;
		}
		else if(input==1){
			return 11;
		}
	}
}
int ace_decide_cpu(int score[2][7], int split, int n) {
	int sum = 0;
	for (int i = 0;i < n;i++) {
		sum += score[split][i];
	}
	if (sum > 10) {
		return 1;
	}
	else {
		return 11;
	}
}
void scorelist(GAMEINFO * player, int split) {//리스트로 관리
	for (int n = 0;n < 7;n++) {
		if (player->hand[split][n].num >= 10)
			player->score[split][n] = 10;
		else if (player->hand[split][n].num == 1 && player->score[split][n] == 0 && player->id == 1)
			player->score[split][n] = ace_decide_user();
		else if (player->hand[split][n].num == 1 && player->score[split][n] == 0 && player->id != 1)
			player->score[split][n] = ace_decide_cpu(player->score, split, n);
		else if(player->hand[split][n].num!=1 && player->hand[split][n].num<10)
			player->score[split][n] = player->hand[split][n].num;
	}
}
int score_sum(GAMEINFO * player, int split) {
	int sum = 0;
	for (int i = 0;i < 7;i++) {
		sum += player->score[split][i];
	}
	return sum;
}
void cpu_judge(CARD dealer[], GAMEINFO player[],int* comp_num) {
    //0번이 높은 수의 갯수, 1번이 낮은 수의 갯수
	for (int i = 0;i < 2;i++) {//유저 카드 판별
		for (int j = 0;j < 7;j++) {
			if (player[0].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[0].score[i][j] >= 2 && player[0].score[i][j] <=4) 
				comp_num[1]++;
		}
	}
	for (int i = 0; i < 2;i++) {//다른 cpu 판별
		for (int j = 0;j < 7;j++) {
			if (player[1].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[1].score[i][j] >= 2 && player[1].score[i][j] <=4)
				comp_num[1]++;
		}
	}
	for (int i = 0; i < 2;i++) {//다른 cpu 판별
		for (int j = 0;j < 7;j++) {
			if (player[2].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[2].score[i][j] >= 2 && player[2].score[i][j] <= 4)
				comp_num[1]++;
		}
	}
	//딜러 판별
	if (dealer[0].num >= 7 || dealer[0].num == 1)
		comp_num[0]++;
	else if(dealer[0].num>=2 && dealer[0].num<=4)
		comp_num[1]++;
}
void action_user_normal(GAMEINFO * player, INFO * info, CARD dealer[], int* card_remain, CARD * *head) {
	int input;
	int cnt = 0;
	int score = 0;
	while (1) {
		scorelist(player, 0);
		score = score_sum(player, 0);
		clearconsole();
		first_open(dealer, player);
		printf("\n플레이어 점수:%d점\n", score);
		if (score > 21) {
			clearconsole();
			printf("뽑으신 결과 21을 초과하셨으므로, 탈락입니다. \n");
			info->result -= player->bet;
			player->state[0] = 4;
			break;
		}
		else if (score == 21 && cnt == 0) {
			clearconsole();
			printf("블랙잭입니다! 축하드립니다! \n");
			player->state[0] = 2;
			break;
		}
		else if (score == 21) {
			clearconsole();
			printf("숫자가 딱 21이시므로, 더 이상 행동은 받지 않겠습니다. \n");
			player->state[0] = 2;
			break;
		}
		printf("다음과 같은 행동을 취하실 수 있습니다.\n");
		printf("1.히트 \n2.스테이 \n3.더블 다운 \n4.서렌더 \n5.이븐 머니 \n6.인슈어런스 \n7.스플릿 \n");
		printf("원하는 행동을 입력하세요:");
		if (scanf("%d", &input) != 1) {
			printf("숫자만 입력하실 수 있습니다.\n");
			clear_inputbuf();
		}
		else if (input == 1) {
			clearconsole();
			for (int i = 0;i < 7;i++) {
				checkdeck(head, card_remain);
				if (player->hand[0][i].num == 0) {
					int idx = rand() % *card_remain;
					player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
					cnt++;
					break;
				}
			}
		}
		else if (input == 2) {
			clearconsole();
			printf("스테이를 선택하셨습니다.\n");
			player->state[0] = 2;
			break;
		}
		else if (input == 3 && cnt == 0) {
			if (info->bankroll >= player->bet) {
				clearconsole();
				printf("더블 다운을 하셨습니다. \n");
				checkdeck(head, card_remain);
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d번 플레이어 배팅 금액: %2d$ \n", player->id, player->bet);
				int idx = rand() % *card_remain;
				player->hand[0][2] = drawcard(getcard(*head, idx), card_remain, idx, head);
				scorelist(player, 0);
				score=score_sum(player, 0);
				if (score > 21) {
					printf("뽑으신 결과 21을 초과하셨으므로, 탈락입니다. \n");
					info->result -= player->bet;
					player->state[0] = 4;
					break;
				}
				else {
					player->state[0] = 2;
					break;
				}
			}
			else {
				clearconsole();
				printf("더블 다운을 할 금액이 없습니다. \n");
			}
		}
		else if (input == 3 && cnt != 0) {
			clearconsole();
			printf("카드를 뽑으셨으므로, 더블 다운은 하실 수 없습니다.\n");
		}
		else if (input == 4) {
			clearconsole();
			printf("서렌더를 선택하셨습니다. 반만큼 돌려 받습니다.\n");
			info->bankroll += player->bet * 0.5;
			info->result -= player->bet * 0.5;
			player->state[0] = 3;
			break;

		}
		else if (input == 5) {
			if (score_sum(player, 0) == 21 && cnt == 0 && dealer[0].num == ace) {
				clearconsole();
				printf("이븐 머니를 하실 수 있습니다. 하시겠습니까? \n");
				printf("0:거절 1:수락 \n");
				printf("입력: ");
				if (scanf("%d", &input) != 1) {
					printf("숫자만 입력하시기 바랍니다. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 아니면 1만 입력하실 수 있습니다. \n");
				else if (input) {
					printf("수락하셨습니다. \n");
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					break;
				}
				else
					printf("거절하셨습니다. \n");
			}
			else {
				clearconsole();
				printf("이븐 머니를 하실 수 있는 상태가 아닙니다. \n");
			}

		}
		else if (input == 6) {
			if (dealer[0].num == ace && info->bankroll >= player->bet * 0.5) {
				clearconsole();
				printf("인슈어런스를 선택하실 수 있습니다. 하시겠습니까? \n");
				printf("0:거절 1:수락 \n");
				printf("입력: ");
				if (scanf("%d", &input) != 1) {
					printf("숫자만 입력하시기 바랍니다. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 아니면 1만 입력하실 수 있습니다. \n");
				else if (input) {
					printf("수락하셨습니다. \n");
					player->insurance = player->bet * 0.5;
					info->bankroll -= player->bet * 0.5;
				}
				else
					printf("거절하셨습니다. \n");
			}
			else {
				clearconsole();
				printf("인슈어런스를 하실 수 없습니다. \n");
			}

		}
		else if (input == 7) {
			if (player->hand[0][0].num == player->hand[0][1].num && info->bankroll >= player->bet && cnt == 0) {
				clearconsole();
				printf("스플릿 게임을 하실 수 있습니다. 하시겠습니까? \n");
				printf("0:거절 1:수락\n");
				printf("입력: ");
				if (scanf("%d", &input) != 1) {
					printf("숫자만 입력하시기 바랍니다. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 아니면 1만 입력하실 수 있습니다. \n");
				else if (input) {
					printf("수락하셨습니다. \n");
					player->split = 1;
					player->state[1] = 1;
					info->bankroll -= player->bet;
					player->bet += player->bet;
					break;
				}
				else
					printf("거절하셨습니다. \n");
			}
			else if (cnt != 0)
				printf("히트하셨다면 하실 수 없습니다. \n");
			else if (player->hand[0][0].num != player->hand[0][1].num)
				printf("원페어가 아니므로 하실 수 없습니다. \n");
			else if (info->bankroll < player->bet)
				printf("금액이 부족합니다. \n");
		}
		else {
			clearconsole();
			printf("0~7까지만 입력하실 수 있습니다.\n");
		}
	}
}
void action_cpu_normal(GAMEINFO * player, INFO * info, CARD dealer[], int* card_remain, CARD * *head) {
	int ins = 0;
	int cnt = 0;
	int resist = 0;
	int score = 0;
	int comp_num[2] = { 0,0 };
	while (1) {
		scorelist(player, 0);
		score = score_sum(player, 0);
		cpu_judge(dealer, player, comp_num);
		if (score > 21) {
			printf("%d번 플레이어가 버스트되셨습니다. \n", player->id);
			player->state[0] = 4;
			info->result -= player->bet;
			break;
		}
		if (resist > 20) {
			if (score_sum(player, 0) > 21) {
				printf("%d번 플레이어가 버스트되셨습니다.\n", player->id);
				player->state[0] = 4;
				info->result -= player->bet;
				break;
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (dealer[0].num == 1 && score == 21 && cnt == 0) { //이븐머니 여부 낮은 숫자가 많으면 75퍼로 이븐머니
			if (comp_num[1] >= 4) {
				if (rand() % 4 != 0) {
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					printf("%d번 플레이어가 이븐머니를 고르셨습니다.\n", player->id);
					break;
				}
			}
			else {//아니면 25퍼로 이븐머니
				if (rand() % 4 == 0) {
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					printf("%d번 플레이어가 이븐머니를 고르셨습니다.\n", player->id);
					break;
				}
			}
		}
		else if (dealer[0].num == 1 && ins == 0) { //인슈어런스 여부 낮은 숫자가 많으면 50퍼로 인슈어런스
			if (comp_num[1] >= 4) {
				if (rand() % 2 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d번 플레이어가 인슈어런스를 거셨습니다.\n", player->id);
				}
			}
			else {//아니면 25퍼로 인슈어런스
				if (rand()%4 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d번 플레이어가 인슈어런스를 거셨습니다.\n", player->id);
				}
			}
		}
		else if (score == 21) { //위 옵션 선택 여부를 묻고,21이면 탈출
			player->state[0] = 2;
			break;
		}
		else if (player->hand[0][0].num == player->hand[0][1].num &&(player->hand[0][0].num >= 8 || player->hand[0][0].num == 1 ) && cnt == 0) { //8 8 조합이면 87.5퍼로 스플릿
			if (rand()%8 != 0 && info->bankroll >= player->bet) {
				player->split = 1;
				player->state[1] = 1;
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d번 플레이어는 스플릿 게임을 진행합니다.\n", player->id);
				printf("%d번 플레이어 배팅 금액: %2d$\n", player->id, player->bet);
				break;
			}
		}
		else if (player->hand[0][0].num == player->hand[0][1].num && cnt == 0 ) { //12.5퍼 확률로 스플릿 게임
			if (rand()%8 == 0 && info->bankroll >= player->bet) {
				player->split = 1;
				player->state[1] = 1;
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d번 플레이어는 스플릿 게임을 진행합니다.\n", player->id);
				printf("%d번 플레이어 배팅 금액: %2d$\n", player->id, player->bet);
				break;
			}
		}
		else if (score < 9) {//9점 이하면 99퍼 확률로 뽑는다.
			if (rand() % 100 != 1) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 스테이 하셨습니다. \n", player->id);
			}
		}
		else if (score>=10 && score<=13 ) {//13 이하일 때 => 90퍼 확률로 뽑아본다. 50퍼센트 확률로 더블다운
			if (rand()%2 == 0 && cnt == 0 && info->bankroll >= player->bet) {//더블다운
				if (info->bankroll > player->bet) {
					printf("%d번 플레이어가 더블 다운 하셨습니다.\n", player->id);
					checkdeck(head, card_remain);
					info->bankroll -= player->bet;
					player->bet += player->bet;
					player->state[0] = 2;
					printf("%d번 플레이어 배팅 금액: %2d$\n", player->id,player->bet);
					int idx = rand() % *card_remain;
					player->hand[0][2] = drawcard(getcard(*head, idx), card_remain, idx, head);
					scorelist(player, 0);
					score = score_sum(player, 0);
					if (score > 21) { //더블다운을 하면 다른 행동은 못하므로, 이 시점에서 빠져나가야 한다.
						info->result -= player->bet;
						player->state[0] = 4;
						break;
					}
					else {
						player->state[0] = 2;
						break;
					}
				}
			}
			else if (rand() % 10 != 0) {//그냥 드로우
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[0] >= 4) {//14~16인데 높은 숫자 카드가 많이 뽑혔을 때 => 66.6퍼 확률로 뽑아본다. 25퍼 확률로 더블 다운 33.3퍼로 서렌
			if (rand()%4 == 0 && cnt == 0 && info->bankroll >= player->bet) {
				if (info->bankroll > player->bet) {
					printf("%d번 플레이어가 더블 다운 하셨습니다.\n", player->id);
					checkdeck(head, card_remain);
					info->bankroll -= player->bet;
					player->bet += player->bet;
					player->state[0] = 2;
					printf("%d번 플레이어 배팅 금액: %2d$\n", player->id, player->bet);
					int idx = rand() % *card_remain;
					player->hand[0][2] = drawcard(getcard(*head, idx), card_remain, idx, head);
					scorelist(player, 0);
					score = score_sum(player, 0);
					if (score > 21) {
						info->result -= player->bet;
						player->state[0] = 4;
						break;
					}
					else {
						player->state[0] = 2;
						break;
					}
				}
			}
			else if(rand()%3!=0){
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d번 플레이어가 서렌더하셨습니다. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score >= 14 && score <= 16)&& comp_num[1] >= 4) {//14~16인데 낮은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다. 75퍼로 서렌
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else if (rand() % 4 != 0) {
				printf("%d번 플레이어가 서렌더하셨습니다. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score >= 14 && score <= 16) {//죄다 애매한 경우 => 12.5퍼로 뽑는다. 50퍼로 서렌
			if (rand()%8 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else if (rand() % 2 == 0) {
				printf("%d번 플레이어가 서렌더하셨습니다. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score == 17 || score == 18) && comp_num[0] >= 4) {//17~18인데 높은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다. 
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score == 17|| score == 18) {//17~18일 때는 10퍼 확률로 뽑는다.
			if (rand()%10 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score == 19 || score == 20) {//19~20일 때는 5퍼 확률로 뽑는다.
			if (rand()%20 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d번 플레이어가 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		resist++;
	}
}
void split_init(GAMEINFO* player) {
	player->hand[1][0] = player->hand[0][1];
	player->hand[0][1] = empty;
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 7;j++) {
			player->score[i][j]=0;
		}
	}
}
void split_draw(GAMEINFO* player,CARD** head,int* remain,CARD dealer[]) {
	for (int i = 0;i < 2;i++) {
		checkdeck(head, remain);
		int idx = rand() % *remain;
		player->hand[i][1] = drawcard(getcard(*head, idx), remain, idx, head);
	}
	if (player->id == 1) {
		first_open(dealer, player);
		scorelist(player, 0);
		scorelist(player, 1);
	}
	else {
		scorelist(player, 0);
		scorelist(player, 1);
	}

}
void splitgame_user(GAMEINFO* player,INFO* info,CARD dealer[], int* card_remain, CARD** head) {
	int input;
	int cnt1=0;
	int cnt2=0;
	while (1) {
		scorelist(player, 0);
		scorelist(player, 1);
		if (score_sum(player,0) == 21) {
			clearconsole();
			printf("1열의 숫자가 딱 21이시므로, 1열에 대해선 선언하실 수 없습니다. \n");
			player->state[0] = 2;
		}
		if (score_sum(player, 1) == 21) {
			clearconsole();
			printf("2열의 숫자가 딱 21이시므로, 2열에 대해선 선언하실 수 없습니다. \n");
			player->state[1] = 2;
		}
		if (score_sum(player, 0) > 21 && score_sum(player, 1) > 21) {
			clearconsole();
			printf("두 게임 모두 버스트되셨습니다. \n");
			player->state[0] = 4;
			player->state[1] = 4;
			info->result -= player->bet;
			break;
		}
		else if (score_sum(player, 0) > 21) {
			printf("1열이 버스트되었습니다. \n");
			player->state[0] = 4;
			info->result -= player->bet*0.5;
		}
		else if (score_sum(player, 1) > 21) {
			printf("2열이 버스트되었습니다. \n");
			player->state[1] = 4;
			info->result -= player->bet * 0.5;
		}
		if ((player->state[0] !=1) && (player->state[1] !=1)) {
			break;
		}
		printf("\n스플릿 게임을 진행하고 있습니다.\n");
		printf("다음과 같은 선택지를 고를 수 있습니다.\n");
		printf("0.카드 확인 \n1.1열 히트  \n2.2열 히트 \n3.1열 스테이 \n4.2열 스테이 \n5.1열 서렌더 \n6.2열 서렌더 \n7.인슈어런스 \n");
		printf("메뉴를 고르십시오:");
		if (scanf("%d", &input) != 1) {
			printf("숫자만 입력하십시오. \n");
		}
		else if (input == 0) {
			first_open(dealer, player);
		}
		else if (input == 1){
			clearconsole();
			if (player->state[0] !=1) {
				printf("1열은 히트할 수 없는 상태입니다.\n");
			}else if (score_sum(player, 0) < 21) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
			}
		}
		else if (input == 2) {
			clearconsole();
			if (player->state[1] !=1) {
				printf("2열은 히트할 수 없는 상태입니다. \n");
			}
			else if (score_sum(player, 1) < 21) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
			}
		}
		else if (input == 3) {
			clearconsole();
			if (player->state[0] ==1) {
				printf("1열 스테이를 선택하셨습니다. \n");
				player->state[0] = 2;
			}
			else 
				printf("1열에 대한 행동은 이미 끝났습니다. \n");
		}
		else if (input == 4) {
			clearconsole();
			if (player->state[1] ==1) {
				printf("2열 스테이를 선택하셨습니다. \n");
				player->state[1] = 2;
			}
			else
				printf("2열에 대한 행동은 이미 끝났습니다. \n");
		}
		else if (input == 5) {
			clearconsole();
			if (player->state[0] ==1) {
				printf("1열 서렌더를 선택하셨습니다. \n");
				info->bankroll += player->bet * 0.5;
				info->result -= (player->bet * 0.5)*0.5;
			}
			else
				printf("1열에 대한 행동은 이미 끝났습니다. \n");
		}
		else if (input == 6) {
			clearconsole();
			if (player->state[1] ==1) {
				printf("2열 서렌더를 선택하셨습니다. \n");
				info->bankroll += player->bet * 0.5;
				info->result -= (player->bet * 0.5) * 0.5;
			}
			else
				printf("2열에 대한 행동은 이미 끝났습니다. \n");
		}
		else if (input == 7) {
			clearconsole();
			if (dealer[0].num == 1 && info->bankroll >= player->bet * 0.5) {
				printf("인슈어런스를 선택하실 수 있습니다. 하시겠습니까? \n");
				printf("0:거절 1수락");
				if (scanf("%d", &input) != 1) {
					printf("숫자만 입력하시기 바랍니다. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 아니면 1만 입력하실 수 있습니다. \n");
				else if (input) {
					printf("수락하셨습니다. \n");
					player->insurance = player->bet * 0.5;
					info->bankroll -= player->bet * 0.5;
				}
				else
					printf("거절하셨습니다. \n");
			}
			else {
				printf("인슈어런스를 하실 수 없습니다. \n");
			}
		}
		else 
			printf("0~7까지의 숫자만 입력하실 수 있습니다. \n");

	}
}
void splitgame_cpu(GAMEINFO* player, INFO* info, CARD dealer[], int* card_remain, CARD** head) {
	int ins = 0;
	int resist = 0;
	int cnt1 = 0;
	int cnt2 = 0;
	int score = 0;
	int score2 = 0;
	int comp_num[2] = { 0,0 };
	while (1) {
		scorelist(player, 0);
		scorelist(player, 1);
		score = score_sum(player, 0);
		score2 = score_sum(player, 1);
		cpu_judge(dealer, player, comp_num);
		//한 바퀴 돌고 나서 판단하기
		if (score > 21 && player->state[0]==1) {
			player->state[0] = 4;
			info->result -= (player->bet * 0.5);
			printf("%d번 플레이어가 1열에 대해 버스트되셨습니다.\n", player->id);
		}
		if (score2 > 21 && player->state[1] == 1) {
			player->state[1] = 4;
			info->result -= (player->bet * 0.5);
			printf("%d번 플레이어가 2열에 대해 버스트되셨습니다.\n", player->id);
		}
		if (score > 21 && score2 > 21) {
			player->state[0] = 4;
			player->state[1] = 4;
			break;
		}
		if ((player->state[0] !=1)&&(player->state[1] !=1)) {
			break;
		}
		if (resist > 20) {//정상 작동을 안했을 경우 억지로 종료
			score = score_sum(player, 0);
			score2 = score_sum(player, 1);
			if (score > 21) {
				printf("%d번 플레이어가 1열에 대해 버스트되셨습니다.\n", player->id);
				player->state[0] = 4;
				info->result -= (player->bet * 0.5);
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[0] = 2;
			}
			if (score2 > 21) {
				printf("%d번 플레이어가 2열에 대해 버스트되셨습니다.\n", player->id);
				player->state[1] = 4;
				info->result -= (player->bet * 0.5);
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[1] = 2;
			}
			break;
		}
		//1번 열에 대한 판단
		if (dealer[0].num == 1 && ins == 0) { //인슈어런스 여부 낮은 숫자가 많으면 50퍼로 인슈어런스
			if (comp_num[1] >=4) {
				if (rand()%2 == 0 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d번 플레이어가 인슈어런스를 걸었습니다. \n", player->id);
				}
			}
			else {//아니면 25퍼로 인슈어런스
				if (rand()%4 == 0) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d번 플레이어가 인슈어런스를 걸었습니다. \n", player->id);
				}
			}

		}
		else if (score == 21) { //위 옵션 선택 여부를 묻고 블랙잭이면 이에 대한 행동은 안한다.
			player->state[0] = 2;
		}
		else if (score < 10) {
			if (rand() % 100 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 한 장 뽑았습니다. \n", player->id);
			}
			else {
				printf("%d번 플레이어는 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score>=10 && score <= 13) {//10~13일 때 => 90퍼 확률로 뽑아본다. 
			if (rand()%10 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 한 장 뽑았습니다. \n", player->id);
			}
			else {
				printf("%d번 플레이어는 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[0] >= 4) {//14~16인데 높은 숫자 카드가 많이 뽑혔을 때 => 66.6퍼 확률로 뽑아본다  33.3퍼로 서렌
			if (rand()%3 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 한 장 뽑았습니다. \n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d번 플레이어가 1열에 대해 서렌더하셨습니다. \n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어는 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[1] >= 4) {//14~16인데 낮은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다. 75퍼로 서렌
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 한 장 뽑았습니다. \n", player->id);
			}
			else if (rand() % 4 != 0) {
				printf("%d번 플레이어가 1열에 대해 서렌더하셨습니다. \n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score >= 14 && score <= 16) {//14~16 12.5% 서렌확률 50
			if (rand()%8 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 카드를 한 장 뽑습니다. \n", player->id);
			}
			else if (rand() % 2 == 0) {
				printf("%d번 플레이어가 1열에 대해 서렌더하셨습니다.\n", player->id);
				info->bankroll += player->bet * 0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score == 17 || score==18)&& comp_num[0] >= 4) {//17~18인데 높은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score == 17 || score == 18) {//17~18  10퍼 확률로 뽑아본다.
			if (rand()%10 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score == 19 || score == 20) {//19~20일 때는 5퍼 확률로 뽑는다.
			if (rand()%20 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt1++;
						break;
					}
				}
				printf("%d번 플레이어가 1열에서 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 1열에 대해 스테이하셨습니다.\n", player->id);
				player->state[0] = 2;
			}
		}

		//2번 열에 대한 판단
		if (score2 == 21) { //21점인가?
			player->state[1] = 2;
		}
		else if (score2 < 10) {
			if (rand() % 100 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 한 장 뽑았습니다. \n", player->id);
			}
			else {
				printf("%d번 플레이어는 2열에 대해 스테이하셨습니다. \n", player->id);
				player->state[1] = 2;
			}
		}
		else if (score2>=10 && score2 <=13) {//13 이하일 때 => 90퍼 확률로 뽑아본다. 
			if (rand()%10 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 >= 14 && score2 <= 16) && comp_num[0] >= 4) {//14~16인데 높은 숫자 카드가 많이 뽑혔을 때 => 66.6퍼 확률로 뽑아본다. 33.3퍼로 서렌
			if (rand()%3 != 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑습니다.\n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d번 플레이어가 2열에 대해 서렌더하셨습니다.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 >= 14 && score2 <= 16) && comp_num[1] >= 4) {//14~16인데 낮은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다. 75퍼로 서렌
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
			}
			else if (rand() % 4 !=0) {
				printf("%d번 플레이어가 2열에 대해 서렌더하셨습니다.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다. \n");
				player->state[1] = 2;
			}
		}
		else if (score2 >= 14 && score2 <= 16 ) {//14~16 12.5% 서렌확률 25
			if (rand()%8 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑으셨습니다.\n", player->id);
			}
			else if (rand() % 4 == 0) {
				printf("%d번 플레이어가 2열에 대해 서렌더하셨습니다.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 == 17 || score2==18) && comp_num[0] >= 4) {//17~18인데 높은 숫자 카드가 많이 뽑혔을 때 => 25퍼 확률로 뽑아본다.
			if (rand()%4 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑으셨습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
				player->state[1] = 2;

			}
		}
		else if (score2 == 17 || score2 == 18) {//10퍼 확률로 뽑아본다.
			if (rand() % 10 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑으셨습니다.\n", player->id);
			}
			else {
				player->state[1] = 2;
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
			}
		}
		else if (score2 == 19 || score2 == 20) {//19~20일 때는 5퍼 확률로 뽑는다.
			if (rand()%20 == 0) {
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[1][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[1][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt2++;
						break;
					}
				}
				printf("%d번 플레이어가 2열에서 카드를 한 장 뽑으셨습니다.\n", player->id);
			}
			else {
				printf("%d번 플레이어가 2열에 대해 스테이하셨습니다.\n", player->id);
				player->state[1] = 2;
			}
		}
		resist++;
	}
}
void last_open(CARD dealer[], GAMEINFO* n) {
	printf("===========================\n");
	printf("딜러의 카드는 다음과 같습니다. \n");
	for (int i = 0;i < 7;i++) {
		if (dealer[i].num != 0)
			printf("%s %s ", num[(dealer[i].num) - 1], shape[(dealer->shape)]);
	}
	printf("\n딜러의 점수: %2d점\n", dealer_score(dealer));
	printf("\n\n");
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 2;j++) {
			if (n->state[j] != 0) {
				printf("%d번 플레이어의 %d열 카드: ", i + 1, j + 1);
				for (int k = 0;k < 7;k++) {
					if (n->hand[j][k].num != 0) {
						printf("%s %s ", num[(n->hand[j][k].num) - 1], shape[n->hand[j][k].shape]);
					}
				}
				scorelist(n, j);
				printf("\n\n%d번 플레이어의 %d열 카드의 점수 합: %2d점\n", i + 1, j + 1, score_sum(n, j));
				printf("\n\n");
			}
		}
		n++;
	}
}
void dealer_draw(CARD dealer[],int* card_remain,CARD** head) {
	for (int i = 0;i < 7;i++) {
		checkdeck(head, card_remain);
		if (dealer[i].num == 0) {
			int idx = rand() % *card_remain;
			dealer[i] = drawcard(getcard(*head, idx), card_remain, idx, head);
			break;
		}
	}
}
int dealer_score(CARD dealer[]) {
	int sum = 0;
	for (int i = 0;i < 7;i++) {
		if (dealer[i].num == ace && sum > 10) 
			sum += 1;
		else if (dealer[i].num == ace && sum <= 10) 
			sum += 11;
		else if (dealer[i].num >= 10) 
			sum += 10;
		else if (dealer[i].num < 10 && dealer[i].num != ace) 
			sum += dealer[i].num;
	}
	return sum;
}
void insurance_money(GAMEINFO* player, INFO* info, CARD dealer[]) {
	int dcnt = 0;
	for (int i = 0;i < 7;i++) {//딜러 블랙잭 여부 확인
		if (dealer[i].num != 0)
			dcnt++;
	}
	if (dealer_score(dealer) == 21 && dcnt == 2 && player->insurance != 0) {//딜러만 블랙잭이고 인슈어런스를 건 상황
		printf("%d번 플레이어가 인슈어런스에 성공하셨습니다! \n",player->id);
		printf("본전을 치셨군요! \n");
		info->bankroll += player->insurance * 2;
	}
	else {
		printf("%d번 플레이어가 인슈어런스에 실패하셨습니다. \n", player->id);
		info->result -= player->insurance;
	}
}
void money_split(GAMEINFO* gameinfo,INFO* playerinfo,CARD dealerinfo[],int result[]) {//0번:패배 1번:딜러 버스트 2번:게임 승리 3번:게임 비김 
	int dcnt = 0, ins = 0, dealerscore = 0, playerscore1 = 0, playerscore2 = 0;
	for (int i = 0;i < 7;i++) {//딜러 블랙잭 여부 확인
		if (dealerinfo[i].num != 0)
			dcnt++;
	}
	dealerscore = dealer_score(dealerinfo);
	playerscore1 = score_sum(gameinfo, 0);
	playerscore2 = score_sum(gameinfo, 1);
	//1열끼리 확인 스플릿 게임 시 블랙잭이 없다!
	//기본 전제: 1열이 버스트나 서렌더가 아닐 것
	if (gameinfo->state[0] != 4 && gameinfo->state[0] != 3) {
		if (dealerscore > 21) { //딜러가 버스트된 상황
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[0] = 1;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 > dealerscore) {//점수에서 이긴 경우
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[0] = 2;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 == dealerscore) {//딜러가 블랙잭이 아닌 상황에서 비긴 경우
			playerinfo->bankroll += gameinfo->bet * 0.5;
			result[0] = 3;
		}
		else {
			playerinfo->result -= (gameinfo->bet * 0.5);
			result[0] = 0;
		}
	}
	//2열끼리 확인
	if (gameinfo->state[1] != 4 && gameinfo->state[1] != 3) {
		if (dealerscore > 21) { //딜러가 버스트된 상황
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[1] = 1;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore2 > dealerscore) { //점수에서 이긴 경우
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[1] = 2;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore2 == dealerscore) { //딜러가 블랙잭이 아닌 상황에서 비긴 경우
			playerinfo->bankroll += gameinfo->bet * 0.5;
			result[1] = 3;
		}
		else {
			playerinfo->result -= (gameinfo->bet * 0.5);
			result[1] = 0;
		}
	}
}
void money_normal(GAMEINFO* gameinfo, INFO* playerinfo, CARD dealerinfo[], int* result) {//0번:패배 1번:딜러 버스트 2번:게임 승리 3번:게임 비김 4번:블랙잭 승리
	int cnt1 = 0, dcnt = 0, dealerscore = 0, playerscore1 = 0;
	for (int i = 0;i < 7;i++) {//1열 블랙잭 여부 확인
		if (gameinfo->hand[0][i].num != 0)
			cnt1++;
	}
	for (int i = 0;i < 7;i++) {//딜러 블랙잭 여부 확인
		if (dealerinfo[i].num != 0)
			dcnt++;
	}
	dealerscore = dealer_score(dealerinfo);
	playerscore1 = score_sum(gameinfo, 0);
	if (!(dealerscore == 21 && dcnt == 2) && (playerscore1 == 21 && cnt1 == 2)){//나만 블랙잭
		playerinfo->bankroll += gameinfo->bet * 2.5;
		playerinfo->result += gameinfo->bet * 1.5;
		*result = 4;
	}
	else if ((dealerscore == 21 && dcnt == 2) && (playerscore1 == 21 && cnt1 == 2)) {//블랙잭 무승부
		playerinfo->bankroll += gameinfo->bet;
		*result = 3;
	}
	else if (dealerscore > 21) { //딜러가 버스트된 상황
		playerinfo->bankroll += gameinfo->bet*2;
		playerinfo->result += gameinfo->bet;
		*result = 1;
	}
	else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 > dealerscore) {//점수에서 이긴 경우
		playerinfo->bankroll += gameinfo->bet*2;
		playerinfo->result += gameinfo->bet;
		*result = 2;
	}
	else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 == dealerscore) {//딜러가 블랙잭이 아닌 상황에서 비긴 경우
		playerinfo->bankroll += gameinfo->bet;
		*result = 3;
	}
	else {
		playerinfo->result -= gameinfo->bet;
		*result = 0;
	}

}
void split_resultoutput(int result[],GAMEINFO* player) {
	if (result[0] == 0) {
		printf("%d번 플레이어의 1열의 점수가 딜러보다 낮으셔서 패배하셨습니다. \n\n",player->id);
	}
	else if (result[0] == 1 && score_sum(player,0)<=21) {
		printf("딜러가 버스트됐으므로 %d번 플레이어는 1열에 대해서 승리하셨습니다. \n\n", player->id);
	}
	else if (result[0] == 2 && score_sum(player, 0) <= 21) {
		printf("%d번 플레이어의 1열의 점수가 딜러보다 높아 승리했습니다. \n\n", player->id);
	}
	else if(result[0]==3){
		printf("%d번 플레이어의 1열에 대해 비기셨습니다. \n\n", player->id);
	}
	else if (player->state[0] == 4) {
		printf("%d번 플레이어는 1열에 대해 버스트되어 패배하셨습니다. \n\n", player->id);
	}
	if (result[1] == 0) {
		printf("%d번 플레이어의 2열의 점수가 딜러보다 낮으셔서 패배하셨습니다. \n\n", player->id);
	}
	else if (result[1] == 1 && score_sum(player, 1) <= 21) {
		printf("딜러가 버스트됐으므로 %d번 플레이어는 2열에 대해서 승리하셨습니다. \n\n", player->id);
	}
	else if (result[1] == 2 && score_sum(player, 1) <= 21) {
		printf("%d번 플레이어의 2열의 점수가 딜러보다 높아 승리했습니다. \n\n", player->id);
	}
	else if(result[1]==3){
		printf("%d번 플레이어의 2열에 대해 비기셨습니다. \n\n", player->id);
	}
	else if (player->state[1] == 4) {
		printf("%d번 플레이어 2열에 대해 버스트되어 패배하셨습니다. \n\n", player->id);
	}
}
void normal_resultoutput(int result,GAMEINFO* player) {
	if (result == 0) {
		printf("%d번 플레이어의 점수가 딜러보다 낮으셔서 패배하셨습니다. \n\n", player->id);
	}
	else if (result == 1 && score_sum(player, 0) <= 21) {
		printf("딜러가 버스트됐으므로 %d번 플레이어는 승리하셨습니다. \n\n", player->id);
	}
	else if (result == 2 && score_sum(player, 0) <= 21) {
		printf("%d번 플레이어의 점수가 딜러보다 높아 승리했습니다. \n\n", player->id);
	}
	else if(result==3){
		printf("%d번 플레이어는 비기셨습니다. \n\n", player->id);
	}
	else if(result==4){
		printf("%d번 플레이어는 블랙잭으로 승리하셨습니다. \n\n", player->id);
	}
	else if (player->state[0] == 4) {
		printf("%d번 플레이어는 버스트되어 패배하셨습니다. \n\n", player->id);
	}
}
void clearconsole() {
	system("cls");
}
void dealerinitialize(CARD dealer[]) {
	for (int i = 0;i < 7;i++) {
		dealer[i] = empty;
	}
}