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
	//�÷��̾� ���� 1=�⺻ 2=���� ���� ���¿��� ���� 3=������ ������ ������ ���� ���� 4=����Ʈ 10=�� ���� ����
	int id;
	CARD hand[2][7];
	int score[2][7];
}GAMEINFO;
char* num[13] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };//���ڷ� ġȯ
char* shape[4] = { "��","��","��","��" };//���ڷ� ġȯ
CARD* allocationsrc[156];//���� �Ҵ��� ī���� ������ �����Ѵ�.
CARD empty = { 0,0,NULL };//���� ī��. ī�尡 ������ �ǹ���.
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
			//����
			for (int i = 0;i < 3;i++) {
				if (player_game[0].state[0] == 10) {
					printf("������ ���ñݾ��� ������ ������ ����Ǿ����ϴ�. \n");
					break;
				}
				bet(&player_info[i], &player_game[i], i);
				printf("%d�� �÷��̾��� ���� �ݾ�: %d$ \n", player_game[i].id, player_game[i].bet);
			}
			Sleep(3000);
			clearconsole();
			//�÷��̾� ī�� �����ֱ�
			if (player_game[0].state[0] != 10) {
				for (int i = 0;i < 3;i++) {
					dividecard(&player_game[i], &card_remain, &head);
				}
				//���� ī�� �����ֱ�
				checkdeck(&head, &card_remain);
				divecard_dealer(dealer, &card_remain, &head);
				int i = 0;
				//ī�� ����
				first_open(dealer, player_game);
				//�ൿ
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
				//���� ī�� ���� ����
				last_open(dealer, player_game);
				Sleep(5000);
				//���� �ൿ ����
				while (1) {
					clearconsole();
					if (dealer_score(dealer) <= 16) {
						printf("16 �����̹Ƿ�, ī�带 �� �� �̽��ϴ�. \n");
						dealer_draw(dealer, &card_remain, &head);
						last_open(dealer, player_game);
						Sleep(3000);
					}
					else
						break;
				}
				last_open(dealer, player_game);
				//���� ����
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
					if (player_game[i].state[0] == 4 && player_game[i].split == 0)//����Ʈ�� ���� ������ ������ �����ش�. ���� �̿Ͱ���.
						printf("%d�� �÷��̾�� ����Ʈ�Ǽ̽��ϴ�.\n\n",player_game[i].id);
					else if (player_game[i].state[0] == 4 && player_game[i].state[1] == 4)//�� �� ����Ʈ�� ���� ������ �����ش�. ���� �̿Ͱ���.
						printf("%d�� �÷��̾�� 1�� 2�� ��� ����Ʈ�Ǽ̽��ϴ�.\n\n",player_game[i].id);
				}
				Sleep(5000);
				game_save(player_info);//���� �Ϸ� �ø��� ���ӳ��� ����
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
			printf("�ٽ� �Է��Ͻʽÿ� \n\n");
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
			printf("%d�� �÷��̾� ����: %d %d %d \n", i + 1, (player + i)->bankroll, (player + i)->result, (player + i)->id);
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
	printf("1.���� �����ϱ�\n");
	printf("2.�� ���� Ȯ���ϱ�\n");
	printf("3.�ٸ� ���� ���� Ȯ���ϱ�\n");
	printf("4.���� ��/��� ���̵�\n");
	printf("5.�Ļ� ȸ��\n");
	printf("6.���� ����\n");
	printf("�޴��� �����Ͽ� �ֽʽÿ�:");
	scanf("%d", &input);
	return input;
}
void show_info(INFO * player) {
	printf("========================\n");
	if (player->id == 1) {
		printf("�� �����Դϴ� \n");
	}
	else
		printf("%d�� ��ǻ�� �÷��̾��� �����Դϴ�. \n", player->id);
	printf("������:%d$ \n����:%d$ \n\n", player->bankroll, player->result);
}
void guide() {
	while (1) {
		int input;
		printf("���ӿ� ���� �������� ���̵带 �ص帳�ϴ�. \n");
		printf("1.���� ���� ���� \n2.��� ���� \n3.���� ��Ģ ���� \n0.�׸� ����\n�޴��� �����ϼ���:");
		scanf("%d", &input);
		if (input == 1) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("1.��� �÷��̾�� ���� ���� �� ������ �մϴ�. \n");
			printf("2.������ ���۵Ǹ�, �� �÷��̾�� �ո��� ������ ī�带 2�徿 �޽��ϴ�. \n");
			printf("3.������ ��������� ī�� 2���� �ް�, �� ���� �ٷ� �� ���� �ո��� ���̵��� �����ϴ�. \n");
			printf("4.�� �÷��̾�� �����Ӱ� �ൿ�� �մϴ�. \n");
			printf("5.��� �÷��̾ �ൿ�� ���ƴٸ�, ������ ������ ī�带 �����մϴ�. \n");
			printf("6.������ ��Ģ�� ���� �ൿ�� �����մϴ�. �ൿ�� ���ƴٸ�, ������ �����ϴ�. \n");
			printf("���� �� �÷��̾�� ������ ���� ���̿� ���� ���� �ް� �˴ϴ�. \n");
			printf("\n\n ***********************************\n");
		}
		else if (input == 2) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("����Ʈ: ���� ī����� ���� ���� 21�� �Ѵ� ���¸� ���մϴ�. \n\n");
			printf("����: ���� ó�� ���� 2���� ī�� ������ ACE�� 10,J,K,Q �� �ϳ��� ���¸� ���մϴ�.\n\n");
			printf("���� �ٿ�: ī�带 ������ �� �� �̰�, ���� �ݾ��� 2��� �ϴ� ������ ���մϴ�.\n���� ��Ʈ�� �� ���¶�� �� ������ �� �� �����ϴ�.\n\n");
			printf("��Ʈ: ī�带 �� �� �� �̴� ������ ���մϴ�. �� ���̰� �����Ӱ� ���� �� �ֽ��ϴ�.\n��, ����Ʈ, ���� �ٿ�, ������, ������, �̺� �Ӵ�, ���� ���¸� �� �� �����ϴ� \n\n");
			printf("������: �׺��ϴ� ������ ���մϴ�. ���� �ݾ��� ������ �����ް� ���ӿ��� Ż���մϴ�. \n\n");
			printf("������: ī�带 ���� �ʰ� �� ���и� �����ϰڴٴ� �����Դϴ�.\n���ӿ��� ��� ������ ����������, �� �̻��� �ൿ�� �� �� �����ϴ�. \n\n");
			printf("���ø�: ���а� �� ����� ��Ȳ����, ���� �ݾ��� �� �� �� ���� �� ī��� ������ ������ �ϰڴٰ� �����ϴ� ���Դϴ�. \n���� ���, 8/8 �������� ī�带 2�� �޾Ҵٸ�, 16���� �ƴ϶� 8���� 8������ ������ ������ �� �ֽ��ϴ�.\n\n");
			printf("�̺� �Ӵ�: ���� �����̰� ������ ó�� ������ ī�尡 ACE���,\n���ºΰ� ���� ���ɼ��� �ֱ� ������ ������ ������ �����ϴ� ��� ���� �ݾ��� 2�踦 ���� �޴� �ɼ��Դϴ�.\n\n");
			printf("�ν����: ���� ������ ó�� ������ ī�尡 ACE���,\n������ ������ ���� ���ɼ��� �ֱ� ������ ���� �ݾ��� ���ݸ�ŭ�� ������ ���δ� �ɼ��Դϴ�.\n���� �����Ѵٸ� ������� 2�踦 �ֹǷ�, ������ ���� �˴ϴ�.\n\n");
			printf("\n\n **********************************\n");
		}
		else if (input == 3) {
			clearconsole();
			printf("*********************************** \n\n");
			printf("0. �⺻������ ī�� ������ ���� ū ���� �¸��մϴ�. \n");
			printf("1. �⺻ ������� 2���Դϴ�. \n");
			printf("2. �������� �¸��ϸ� ������� 2.5���Դϴ�. \n");
			printf("3. ����� ������ �ƴ� 21�� �º����� ������ ������ �¸��մϴ�. \n");
			printf("4. ������ ���� ���ų�, ���� �����̸� ���º��Դϴ�. �� ���� ���� �ݾ׸� ���� �޽��ϴ�. \n");
			printf("5. ���� ����Ʈ �Ǹ� ������ �й��̸�, ��밡 ����Ʈ�̸� ������ �¸��Դϴ�. \n");
			printf("6. ���ø��� ���� �� ���� �ٿ��� �Ͻ� �� �����ϴ�. \n");
			printf("7. ���ø��� ���Ӵ� �� ���� ����Ͻ� �� �ֽ��ϴ�. \n");
			printf("8. ���ø��� �ϼ̴ٸ� �� ������ 21�� ����ô��� ������ �ƴմϴ�. \n");
			printf("9. ACE�� 1�� �Ǵ� 11������ ���� �� �ֽ��ϴ�. ���� �������� �� ������ ���� ������ �� �ֽ��ϴ�. \n");
			printf("10. 10,J,Q,K�� ��� 10������ ó���մϴ�.\n");
			printf("11. ���ӿ� ���Ǵ� ī��� 156���Դϴ�.\n");
			printf("12. ������ ó�� ���� ī�� 2���� ���� 16 �����̸� ������ ��Ʈ, 17 �̻��̸� ������ �������ؾ��մϴ�. \n");
			printf("13. �Ļ� ȸ�� ��û �� �������� 100�޷��� �ǵ��� ���� ������, ������ �׸�ŭ ���̰� �˴ϴ�. \n");
			printf("\n\n ***********************************\n");

		}
		else if (input == 0) 
			break;
		else {
			clearconsole();
			printf("0,1,2,3�� �Է��� �� �ֽ��ϴ�. \n");
		}
	}
}
void loan(INFO * player) {
	if (player->bankroll < 10 && player->id == 1) {
		player->result -= (100 - player->bankroll);
		player->bankroll = 100;
		printf("�ܰ� 100�޷��� �ɸ�ŭ�� �־��Ƚ��ϴ�. �ٽ� �������� ������. \n");
	}
	else if (player->bankroll >= 10 && player->id == 1)
		printf("����� �ּ� ���� �ݾ׵� ���� �����׸� �����ص帳�ϴ�. \n");
	else if (player->bankroll < 10 && player->id != 1) {
		player->result -= (100 - player->bankroll);
		player->bankroll = 100;
	}
}
void bet(INFO * info, GAMEINFO * player, int i) {
	int input;
	if (info->id == 1) {
		if (info->bankroll < 10) {
			printf("�ּ� ���� �ݾ׵� �����ø� ���ӿ� ������ �� �����ϴ�. \n");
			player->state[0] = 10;
			return;
		}
		else if (info->bankroll >= 10) {
			while (1) {
				printf("���� �ݾ��� �Է��ϼ���(�ּ� 10$):");
				if (scanf("%d", &input) != 1) {
					printf("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.\n");
					clear_inputbuf();
				}
				else if (input > info->bankroll)
					printf("���� �ݾ��� �����ݺ��� �����ϴ�. \n");
				else if (input < 10)
					printf("��� 10$��ŭ�� �����ϼž� �մϴ�. \n");
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
	printf("������ ������ ī��� ������ �����ϴ�. \n");
	printf("%s %s \n\n", num[(dealer[0].num) - 1], shape[dealer[0].shape]);
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 2;j++) {
			if (n->state[j] != 0) {
				printf("%d�� �÷��̾��� %d�� ī�� \n", i + 1, j + 1);
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
		printf("\nACE�� 1���� 11�� �� ��� ������ ���ðڽ��ϱ�? \n");
		printf("0: Ace�� 1������ ���� 1: Ace�� 11������ ���� \n");
		printf("�Է�:");
		if (scanf("%d", &input) != 1) {
			printf("���ڸ� �Է��ϼ���");
			clear_inputbuf();
		}
		else if (input != 0 && input != 1)
			printf("0 �ƴϸ� 1�� �Է��Ͻ� �� �ֽ��ϴ�. \n");
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
void scorelist(GAMEINFO * player, int split) {//����Ʈ�� ����
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
    //0���� ���� ���� ����, 1���� ���� ���� ����
	for (int i = 0;i < 2;i++) {//���� ī�� �Ǻ�
		for (int j = 0;j < 7;j++) {
			if (player[0].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[0].score[i][j] >= 2 && player[0].score[i][j] <=4) 
				comp_num[1]++;
		}
	}
	for (int i = 0; i < 2;i++) {//�ٸ� cpu �Ǻ�
		for (int j = 0;j < 7;j++) {
			if (player[1].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[1].score[i][j] >= 2 && player[1].score[i][j] <=4)
				comp_num[1]++;
		}
	}
	for (int i = 0; i < 2;i++) {//�ٸ� cpu �Ǻ�
		for (int j = 0;j < 7;j++) {
			if (player[2].score[i][j] >= 7)
				comp_num[0]++;
			else if (player[2].score[i][j] >= 2 && player[2].score[i][j] <= 4)
				comp_num[1]++;
		}
	}
	//���� �Ǻ�
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
		printf("\n�÷��̾� ����:%d��\n", score);
		if (score > 21) {
			clearconsole();
			printf("������ ��� 21�� �ʰ��ϼ����Ƿ�, Ż���Դϴ�. \n");
			info->result -= player->bet;
			player->state[0] = 4;
			break;
		}
		else if (score == 21 && cnt == 0) {
			clearconsole();
			printf("�����Դϴ�! ���ϵ帳�ϴ�! \n");
			player->state[0] = 2;
			break;
		}
		else if (score == 21) {
			clearconsole();
			printf("���ڰ� �� 21�̽ùǷ�, �� �̻� �ൿ�� ���� �ʰڽ��ϴ�. \n");
			player->state[0] = 2;
			break;
		}
		printf("������ ���� �ൿ�� ���Ͻ� �� �ֽ��ϴ�.\n");
		printf("1.��Ʈ \n2.������ \n3.���� �ٿ� \n4.������ \n5.�̺� �Ӵ� \n6.�ν���� \n7.���ø� \n");
		printf("���ϴ� �ൿ�� �Է��ϼ���:");
		if (scanf("%d", &input) != 1) {
			printf("���ڸ� �Է��Ͻ� �� �ֽ��ϴ�.\n");
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
			printf("�����̸� �����ϼ̽��ϴ�.\n");
			player->state[0] = 2;
			break;
		}
		else if (input == 3 && cnt == 0) {
			if (info->bankroll >= player->bet) {
				clearconsole();
				printf("���� �ٿ��� �ϼ̽��ϴ�. \n");
				checkdeck(head, card_remain);
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d�� �÷��̾� ���� �ݾ�: %2d$ \n", player->id, player->bet);
				int idx = rand() % *card_remain;
				player->hand[0][2] = drawcard(getcard(*head, idx), card_remain, idx, head);
				scorelist(player, 0);
				score=score_sum(player, 0);
				if (score > 21) {
					printf("������ ��� 21�� �ʰ��ϼ����Ƿ�, Ż���Դϴ�. \n");
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
				printf("���� �ٿ��� �� �ݾ��� �����ϴ�. \n");
			}
		}
		else if (input == 3 && cnt != 0) {
			clearconsole();
			printf("ī�带 ���������Ƿ�, ���� �ٿ��� �Ͻ� �� �����ϴ�.\n");
		}
		else if (input == 4) {
			clearconsole();
			printf("�������� �����ϼ̽��ϴ�. �ݸ�ŭ ���� �޽��ϴ�.\n");
			info->bankroll += player->bet * 0.5;
			info->result -= player->bet * 0.5;
			player->state[0] = 3;
			break;

		}
		else if (input == 5) {
			if (score_sum(player, 0) == 21 && cnt == 0 && dealer[0].num == ace) {
				clearconsole();
				printf("�̺� �Ӵϸ� �Ͻ� �� �ֽ��ϴ�. �Ͻðڽ��ϱ�? \n");
				printf("0:���� 1:���� \n");
				printf("�Է�: ");
				if (scanf("%d", &input) != 1) {
					printf("���ڸ� �Է��Ͻñ� �ٶ��ϴ�. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 �ƴϸ� 1�� �Է��Ͻ� �� �ֽ��ϴ�. \n");
				else if (input) {
					printf("�����ϼ̽��ϴ�. \n");
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					break;
				}
				else
					printf("�����ϼ̽��ϴ�. \n");
			}
			else {
				clearconsole();
				printf("�̺� �Ӵϸ� �Ͻ� �� �ִ� ���°� �ƴմϴ�. \n");
			}

		}
		else if (input == 6) {
			if (dealer[0].num == ace && info->bankroll >= player->bet * 0.5) {
				clearconsole();
				printf("�ν������ �����Ͻ� �� �ֽ��ϴ�. �Ͻðڽ��ϱ�? \n");
				printf("0:���� 1:���� \n");
				printf("�Է�: ");
				if (scanf("%d", &input) != 1) {
					printf("���ڸ� �Է��Ͻñ� �ٶ��ϴ�. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 �ƴϸ� 1�� �Է��Ͻ� �� �ֽ��ϴ�. \n");
				else if (input) {
					printf("�����ϼ̽��ϴ�. \n");
					player->insurance = player->bet * 0.5;
					info->bankroll -= player->bet * 0.5;
				}
				else
					printf("�����ϼ̽��ϴ�. \n");
			}
			else {
				clearconsole();
				printf("�ν������ �Ͻ� �� �����ϴ�. \n");
			}

		}
		else if (input == 7) {
			if (player->hand[0][0].num == player->hand[0][1].num && info->bankroll >= player->bet && cnt == 0) {
				clearconsole();
				printf("���ø� ������ �Ͻ� �� �ֽ��ϴ�. �Ͻðڽ��ϱ�? \n");
				printf("0:���� 1:����\n");
				printf("�Է�: ");
				if (scanf("%d", &input) != 1) {
					printf("���ڸ� �Է��Ͻñ� �ٶ��ϴ�. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 �ƴϸ� 1�� �Է��Ͻ� �� �ֽ��ϴ�. \n");
				else if (input) {
					printf("�����ϼ̽��ϴ�. \n");
					player->split = 1;
					player->state[1] = 1;
					info->bankroll -= player->bet;
					player->bet += player->bet;
					break;
				}
				else
					printf("�����ϼ̽��ϴ�. \n");
			}
			else if (cnt != 0)
				printf("��Ʈ�ϼ̴ٸ� �Ͻ� �� �����ϴ�. \n");
			else if (player->hand[0][0].num != player->hand[0][1].num)
				printf("���� �ƴϹǷ� �Ͻ� �� �����ϴ�. \n");
			else if (info->bankroll < player->bet)
				printf("�ݾ��� �����մϴ�. \n");
		}
		else {
			clearconsole();
			printf("0~7������ �Է��Ͻ� �� �ֽ��ϴ�.\n");
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
			printf("%d�� �÷��̾ ����Ʈ�Ǽ̽��ϴ�. \n", player->id);
			player->state[0] = 4;
			info->result -= player->bet;
			break;
		}
		if (resist > 20) {
			if (score_sum(player, 0) > 21) {
				printf("%d�� �÷��̾ ����Ʈ�Ǽ̽��ϴ�.\n", player->id);
				player->state[0] = 4;
				info->result -= player->bet;
				break;
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (dealer[0].num == 1 && score == 21 && cnt == 0) { //�̺�Ӵ� ���� ���� ���ڰ� ������ 75�۷� �̺�Ӵ�
			if (comp_num[1] >= 4) {
				if (rand() % 4 != 0) {
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					printf("%d�� �÷��̾ �̺�Ӵϸ� ���̽��ϴ�.\n", player->id);
					break;
				}
			}
			else {//�ƴϸ� 25�۷� �̺�Ӵ�
				if (rand() % 4 == 0) {
					info->bankroll += player->bet * 2;
					info->result += player->bet;
					player->bet = 0;
					player->state[0] = 3;
					printf("%d�� �÷��̾ �̺�Ӵϸ� ���̽��ϴ�.\n", player->id);
					break;
				}
			}
		}
		else if (dealer[0].num == 1 && ins == 0) { //�ν���� ���� ���� ���ڰ� ������ 50�۷� �ν����
			if (comp_num[1] >= 4) {
				if (rand() % 2 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d�� �÷��̾ �ν������ �ż̽��ϴ�.\n", player->id);
				}
			}
			else {//�ƴϸ� 25�۷� �ν����
				if (rand()%4 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d�� �÷��̾ �ν������ �ż̽��ϴ�.\n", player->id);
				}
			}
		}
		else if (score == 21) { //�� �ɼ� ���� ���θ� ����,21�̸� Ż��
			player->state[0] = 2;
			break;
		}
		else if (player->hand[0][0].num == player->hand[0][1].num &&(player->hand[0][0].num >= 8 || player->hand[0][0].num == 1 ) && cnt == 0) { //8 8 �����̸� 87.5�۷� ���ø�
			if (rand()%8 != 0 && info->bankroll >= player->bet) {
				player->split = 1;
				player->state[1] = 1;
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d�� �÷��̾�� ���ø� ������ �����մϴ�.\n", player->id);
				printf("%d�� �÷��̾� ���� �ݾ�: %2d$\n", player->id, player->bet);
				break;
			}
		}
		else if (player->hand[0][0].num == player->hand[0][1].num && cnt == 0 ) { //12.5�� Ȯ���� ���ø� ����
			if (rand()%8 == 0 && info->bankroll >= player->bet) {
				player->split = 1;
				player->state[1] = 1;
				info->bankroll -= player->bet;
				player->bet += player->bet;
				printf("%d�� �÷��̾�� ���ø� ������ �����մϴ�.\n", player->id);
				printf("%d�� �÷��̾� ���� �ݾ�: %2d$\n", player->id, player->bet);
				break;
			}
		}
		else if (score < 9) {//9�� ���ϸ� 99�� Ȯ���� �̴´�.
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ ������ �ϼ̽��ϴ�. \n", player->id);
			}
		}
		else if (score>=10 && score<=13 ) {//13 ������ �� => 90�� Ȯ���� �̾ƺ���. 50�ۼ�Ʈ Ȯ���� ����ٿ�
			if (rand()%2 == 0 && cnt == 0 && info->bankroll >= player->bet) {//����ٿ�
				if (info->bankroll > player->bet) {
					printf("%d�� �÷��̾ ���� �ٿ� �ϼ̽��ϴ�.\n", player->id);
					checkdeck(head, card_remain);
					info->bankroll -= player->bet;
					player->bet += player->bet;
					player->state[0] = 2;
					printf("%d�� �÷��̾� ���� �ݾ�: %2d$\n", player->id,player->bet);
					int idx = rand() % *card_remain;
					player->hand[0][2] = drawcard(getcard(*head, idx), card_remain, idx, head);
					scorelist(player, 0);
					score = score_sum(player, 0);
					if (score > 21) { //����ٿ��� �ϸ� �ٸ� �ൿ�� ���ϹǷ�, �� �������� ���������� �Ѵ�.
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
			else if (rand() % 10 != 0) {//�׳� ��ο�
				for (int i = 0;i < 7;i++) {
					checkdeck(head, card_remain);
					if (player->hand[0][i].num == 0) {
						int idx = rand() % *card_remain;
						player->hand[0][i] = drawcard(getcard(*head, idx), card_remain, idx, head);
						cnt++;
						break;
					}
				}
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[0] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 66.6�� Ȯ���� �̾ƺ���. 25�� Ȯ���� ���� �ٿ� 33.3�۷� ����
			if (rand()%4 == 0 && cnt == 0 && info->bankroll >= player->bet) {
				if (info->bankroll > player->bet) {
					printf("%d�� �÷��̾ ���� �ٿ� �ϼ̽��ϴ�.\n", player->id);
					checkdeck(head, card_remain);
					info->bankroll -= player->bet;
					player->bet += player->bet;
					player->state[0] = 2;
					printf("%d�� �÷��̾� ���� �ݾ�: %2d$\n", player->id, player->bet);
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score >= 14 && score <= 16)&& comp_num[1] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���. 75�۷� ����
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else if (rand() % 4 != 0) {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score >= 14 && score <= 16) {//�˴� �ָ��� ��� => 12.5�۷� �̴´�. 50�۷� ����
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else if (rand() % 2 == 0) {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�. \n", player->id);
				info->bankroll += (player->bet * 0.5);
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score == 17 || score == 18) && comp_num[0] >= 4) {//17~18�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���. 
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score == 17|| score == 18) {//17~18�� ���� 10�� Ȯ���� �̴´�.
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if (score == 19 || score == 20) {//19~20�� ���� 5�� Ȯ���� �̴´�.
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
				printf("%d�� �÷��̾ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ �������ϼ̽��ϴ�.\n", player->id);
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
			printf("1���� ���ڰ� �� 21�̽ùǷ�, 1���� ���ؼ� �����Ͻ� �� �����ϴ�. \n");
			player->state[0] = 2;
		}
		if (score_sum(player, 1) == 21) {
			clearconsole();
			printf("2���� ���ڰ� �� 21�̽ùǷ�, 2���� ���ؼ� �����Ͻ� �� �����ϴ�. \n");
			player->state[1] = 2;
		}
		if (score_sum(player, 0) > 21 && score_sum(player, 1) > 21) {
			clearconsole();
			printf("�� ���� ��� ����Ʈ�Ǽ̽��ϴ�. \n");
			player->state[0] = 4;
			player->state[1] = 4;
			info->result -= player->bet;
			break;
		}
		else if (score_sum(player, 0) > 21) {
			printf("1���� ����Ʈ�Ǿ����ϴ�. \n");
			player->state[0] = 4;
			info->result -= player->bet*0.5;
		}
		else if (score_sum(player, 1) > 21) {
			printf("2���� ����Ʈ�Ǿ����ϴ�. \n");
			player->state[1] = 4;
			info->result -= player->bet * 0.5;
		}
		if ((player->state[0] !=1) && (player->state[1] !=1)) {
			break;
		}
		printf("\n���ø� ������ �����ϰ� �ֽ��ϴ�.\n");
		printf("������ ���� �������� �� �� �ֽ��ϴ�.\n");
		printf("0.ī�� Ȯ�� \n1.1�� ��Ʈ  \n2.2�� ��Ʈ \n3.1�� ������ \n4.2�� ������ \n5.1�� ������ \n6.2�� ������ \n7.�ν���� \n");
		printf("�޴��� ���ʽÿ�:");
		if (scanf("%d", &input) != 1) {
			printf("���ڸ� �Է��Ͻʽÿ�. \n");
		}
		else if (input == 0) {
			first_open(dealer, player);
		}
		else if (input == 1){
			clearconsole();
			if (player->state[0] !=1) {
				printf("1���� ��Ʈ�� �� ���� �����Դϴ�.\n");
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
				printf("2���� ��Ʈ�� �� ���� �����Դϴ�. \n");
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
				printf("1�� �����̸� �����ϼ̽��ϴ�. \n");
				player->state[0] = 2;
			}
			else 
				printf("1���� ���� �ൿ�� �̹� �������ϴ�. \n");
		}
		else if (input == 4) {
			clearconsole();
			if (player->state[1] ==1) {
				printf("2�� �����̸� �����ϼ̽��ϴ�. \n");
				player->state[1] = 2;
			}
			else
				printf("2���� ���� �ൿ�� �̹� �������ϴ�. \n");
		}
		else if (input == 5) {
			clearconsole();
			if (player->state[0] ==1) {
				printf("1�� �������� �����ϼ̽��ϴ�. \n");
				info->bankroll += player->bet * 0.5;
				info->result -= (player->bet * 0.5)*0.5;
			}
			else
				printf("1���� ���� �ൿ�� �̹� �������ϴ�. \n");
		}
		else if (input == 6) {
			clearconsole();
			if (player->state[1] ==1) {
				printf("2�� �������� �����ϼ̽��ϴ�. \n");
				info->bankroll += player->bet * 0.5;
				info->result -= (player->bet * 0.5) * 0.5;
			}
			else
				printf("2���� ���� �ൿ�� �̹� �������ϴ�. \n");
		}
		else if (input == 7) {
			clearconsole();
			if (dealer[0].num == 1 && info->bankroll >= player->bet * 0.5) {
				printf("�ν������ �����Ͻ� �� �ֽ��ϴ�. �Ͻðڽ��ϱ�? \n");
				printf("0:���� 1����");
				if (scanf("%d", &input) != 1) {
					printf("���ڸ� �Է��Ͻñ� �ٶ��ϴ�. \n");
					clear_inputbuf();
				}
				else if (input != 0 && input != 1)
					printf("0 �ƴϸ� 1�� �Է��Ͻ� �� �ֽ��ϴ�. \n");
				else if (input) {
					printf("�����ϼ̽��ϴ�. \n");
					player->insurance = player->bet * 0.5;
					info->bankroll -= player->bet * 0.5;
				}
				else
					printf("�����ϼ̽��ϴ�. \n");
			}
			else {
				printf("�ν������ �Ͻ� �� �����ϴ�. \n");
			}
		}
		else 
			printf("0~7������ ���ڸ� �Է��Ͻ� �� �ֽ��ϴ�. \n");

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
		//�� ���� ���� ���� �Ǵ��ϱ�
		if (score > 21 && player->state[0]==1) {
			player->state[0] = 4;
			info->result -= (player->bet * 0.5);
			printf("%d�� �÷��̾ 1���� ���� ����Ʈ�Ǽ̽��ϴ�.\n", player->id);
		}
		if (score2 > 21 && player->state[1] == 1) {
			player->state[1] = 4;
			info->result -= (player->bet * 0.5);
			printf("%d�� �÷��̾ 2���� ���� ����Ʈ�Ǽ̽��ϴ�.\n", player->id);
		}
		if (score > 21 && score2 > 21) {
			player->state[0] = 4;
			player->state[1] = 4;
			break;
		}
		if ((player->state[0] !=1)&&(player->state[1] !=1)) {
			break;
		}
		if (resist > 20) {//���� �۵��� ������ ��� ������ ����
			score = score_sum(player, 0);
			score2 = score_sum(player, 1);
			if (score > 21) {
				printf("%d�� �÷��̾ 1���� ���� ����Ʈ�Ǽ̽��ϴ�.\n", player->id);
				player->state[0] = 4;
				info->result -= (player->bet * 0.5);
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[0] = 2;
			}
			if (score2 > 21) {
				printf("%d�� �÷��̾ 2���� ���� ����Ʈ�Ǽ̽��ϴ�.\n", player->id);
				player->state[1] = 4;
				info->result -= (player->bet * 0.5);
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[1] = 2;
			}
			break;
		}
		//1�� ���� ���� �Ǵ�
		if (dealer[0].num == 1 && ins == 0) { //�ν���� ���� ���� ���ڰ� ������ 50�۷� �ν����
			if (comp_num[1] >=4) {
				if (rand()%2 == 0 && info->bankroll >= player->bet * 0.5) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d�� �÷��̾ �ν������ �ɾ����ϴ�. \n", player->id);
				}
			}
			else {//�ƴϸ� 25�۷� �ν����
				if (rand()%4 == 0) {
					info->bankroll -= player->bet * 0.5;
					player->insurance = player->bet * 0.5;
					ins = 1;
					printf("%d�� �÷��̾ �ν������ �ɾ����ϴ�. \n", player->id);
				}
			}

		}
		else if (score == 21) { //�� �ɼ� ���� ���θ� ���� �����̸� �̿� ���� �ൿ�� ���Ѵ�.
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
				printf("%d�� �÷��̾ 1������ �� �� �̾ҽ��ϴ�. \n", player->id);
			}
			else {
				printf("%d�� �÷��̾�� 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score>=10 && score <= 13) {//10~13�� �� => 90�� Ȯ���� �̾ƺ���. 
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
				printf("%d�� �÷��̾ 1������ �� �� �̾ҽ��ϴ�. \n", player->id);
			}
			else {
				printf("%d�� �÷��̾�� 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[0] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 66.6�� Ȯ���� �̾ƺ���  33.3�۷� ����
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
				printf("%d�� �÷��̾ 1������ �� �� �̾ҽ��ϴ�. \n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾�� 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if ((score >= 14 && score <= 16) && comp_num[1] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���. 75�۷� ����
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
				printf("%d�� �÷��̾ 1������ �� �� �̾ҽ��ϴ�. \n", player->id);
			}
			else if (rand() % 4 != 0) {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score >= 14 && score <= 16) {//14~16 12.5% ����Ȯ�� 50
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
				printf("%d�� �÷��̾ 1������ ī�带 �� �� �̽��ϴ�. \n", player->id);
			}
			else if (rand() % 2 == 0) {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�.\n", player->id);
				info->bankroll += player->bet * 0.5;
				player->state[0] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
				break;
			}
		}
		else if ((score == 17 || score==18)&& comp_num[0] >= 4) {//17~18�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���
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
				printf("%d�� �÷��̾ 1������ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score == 17 || score == 18) {//17~18  10�� Ȯ���� �̾ƺ���.
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
				printf("%d�� �÷��̾ 1������ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
			}
		}
		else if (score == 19 || score == 20) {//19~20�� ���� 5�� Ȯ���� �̴´�.
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
				printf("%d�� �÷��̾ 1������ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 1���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[0] = 2;
			}
		}

		//2�� ���� ���� �Ǵ�
		if (score2 == 21) { //21���ΰ�?
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
				printf("%d�� �÷��̾ 2������ �� �� �̾ҽ��ϴ�. \n", player->id);
			}
			else {
				printf("%d�� �÷��̾�� 2���� ���� �������ϼ̽��ϴ�. \n", player->id);
				player->state[1] = 2;
			}
		}
		else if (score2>=10 && score2 <=13) {//13 ������ �� => 90�� Ȯ���� �̾ƺ���. 
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 >= 14 && score2 <= 16) && comp_num[0] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 66.6�� Ȯ���� �̾ƺ���. 33.3�۷� ����
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �̽��ϴ�.\n", player->id);
			}
			else if (rand() % 3 == 0) {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 >= 14 && score2 <= 16) && comp_num[1] >= 4) {//14~16�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���. 75�۷� ����
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
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�. \n");
				player->state[1] = 2;
			}
		}
		else if (score2 >= 14 && score2 <= 16 ) {//14~16 12.5% ����Ȯ�� 25
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �����̽��ϴ�.\n", player->id);
			}
			else if (rand() % 4 == 0) {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				info->bankroll += (player->bet * 0.5)*0.5;
				player->state[1] = 3;
				break;
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[1] = 2;
			}
		}
		else if ((score2 == 17 || score2==18) && comp_num[0] >= 4) {//17~18�ε� ���� ���� ī�尡 ���� ������ �� => 25�� Ȯ���� �̾ƺ���.
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �����̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[1] = 2;

			}
		}
		else if (score2 == 17 || score2 == 18) {//10�� Ȯ���� �̾ƺ���.
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �����̽��ϴ�.\n", player->id);
			}
			else {
				player->state[1] = 2;
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
			}
		}
		else if (score2 == 19 || score2 == 20) {//19~20�� ���� 5�� Ȯ���� �̴´�.
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
				printf("%d�� �÷��̾ 2������ ī�带 �� �� �����̽��ϴ�.\n", player->id);
			}
			else {
				printf("%d�� �÷��̾ 2���� ���� �������ϼ̽��ϴ�.\n", player->id);
				player->state[1] = 2;
			}
		}
		resist++;
	}
}
void last_open(CARD dealer[], GAMEINFO* n) {
	printf("===========================\n");
	printf("������ ī��� ������ �����ϴ�. \n");
	for (int i = 0;i < 7;i++) {
		if (dealer[i].num != 0)
			printf("%s %s ", num[(dealer[i].num) - 1], shape[(dealer->shape)]);
	}
	printf("\n������ ����: %2d��\n", dealer_score(dealer));
	printf("\n\n");
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 2;j++) {
			if (n->state[j] != 0) {
				printf("%d�� �÷��̾��� %d�� ī��: ", i + 1, j + 1);
				for (int k = 0;k < 7;k++) {
					if (n->hand[j][k].num != 0) {
						printf("%s %s ", num[(n->hand[j][k].num) - 1], shape[n->hand[j][k].shape]);
					}
				}
				scorelist(n, j);
				printf("\n\n%d�� �÷��̾��� %d�� ī���� ���� ��: %2d��\n", i + 1, j + 1, score_sum(n, j));
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
	for (int i = 0;i < 7;i++) {//���� ���� ���� Ȯ��
		if (dealer[i].num != 0)
			dcnt++;
	}
	if (dealer_score(dealer) == 21 && dcnt == 2 && player->insurance != 0) {//������ �����̰� �ν������ �� ��Ȳ
		printf("%d�� �÷��̾ �ν������ �����ϼ̽��ϴ�! \n",player->id);
		printf("������ ġ�̱���! \n");
		info->bankroll += player->insurance * 2;
	}
	else {
		printf("%d�� �÷��̾ �ν������ �����ϼ̽��ϴ�. \n", player->id);
		info->result -= player->insurance;
	}
}
void money_split(GAMEINFO* gameinfo,INFO* playerinfo,CARD dealerinfo[],int result[]) {//0��:�й� 1��:���� ����Ʈ 2��:���� �¸� 3��:���� ��� 
	int dcnt = 0, ins = 0, dealerscore = 0, playerscore1 = 0, playerscore2 = 0;
	for (int i = 0;i < 7;i++) {//���� ���� ���� Ȯ��
		if (dealerinfo[i].num != 0)
			dcnt++;
	}
	dealerscore = dealer_score(dealerinfo);
	playerscore1 = score_sum(gameinfo, 0);
	playerscore2 = score_sum(gameinfo, 1);
	//1������ Ȯ�� ���ø� ���� �� ������ ����!
	//�⺻ ����: 1���� ����Ʈ�� �������� �ƴ� ��
	if (gameinfo->state[0] != 4 && gameinfo->state[0] != 3) {
		if (dealerscore > 21) { //������ ����Ʈ�� ��Ȳ
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[0] = 1;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 > dealerscore) {//�������� �̱� ���
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[0] = 2;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 == dealerscore) {//������ ������ �ƴ� ��Ȳ���� ��� ���
			playerinfo->bankroll += gameinfo->bet * 0.5;
			result[0] = 3;
		}
		else {
			playerinfo->result -= (gameinfo->bet * 0.5);
			result[0] = 0;
		}
	}
	//2������ Ȯ��
	if (gameinfo->state[1] != 4 && gameinfo->state[1] != 3) {
		if (dealerscore > 21) { //������ ����Ʈ�� ��Ȳ
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[1] = 1;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore2 > dealerscore) { //�������� �̱� ���
			playerinfo->bankroll += gameinfo->bet;
			playerinfo->result += (gameinfo->bet * 0.5);
			result[1] = 2;
		}
		else if (!(dealerscore == 21 && dcnt == 2) && playerscore2 == dealerscore) { //������ ������ �ƴ� ��Ȳ���� ��� ���
			playerinfo->bankroll += gameinfo->bet * 0.5;
			result[1] = 3;
		}
		else {
			playerinfo->result -= (gameinfo->bet * 0.5);
			result[1] = 0;
		}
	}
}
void money_normal(GAMEINFO* gameinfo, INFO* playerinfo, CARD dealerinfo[], int* result) {//0��:�й� 1��:���� ����Ʈ 2��:���� �¸� 3��:���� ��� 4��:���� �¸�
	int cnt1 = 0, dcnt = 0, dealerscore = 0, playerscore1 = 0;
	for (int i = 0;i < 7;i++) {//1�� ���� ���� Ȯ��
		if (gameinfo->hand[0][i].num != 0)
			cnt1++;
	}
	for (int i = 0;i < 7;i++) {//���� ���� ���� Ȯ��
		if (dealerinfo[i].num != 0)
			dcnt++;
	}
	dealerscore = dealer_score(dealerinfo);
	playerscore1 = score_sum(gameinfo, 0);
	if (!(dealerscore == 21 && dcnt == 2) && (playerscore1 == 21 && cnt1 == 2)){//���� ����
		playerinfo->bankroll += gameinfo->bet * 2.5;
		playerinfo->result += gameinfo->bet * 1.5;
		*result = 4;
	}
	else if ((dealerscore == 21 && dcnt == 2) && (playerscore1 == 21 && cnt1 == 2)) {//���� ���º�
		playerinfo->bankroll += gameinfo->bet;
		*result = 3;
	}
	else if (dealerscore > 21) { //������ ����Ʈ�� ��Ȳ
		playerinfo->bankroll += gameinfo->bet*2;
		playerinfo->result += gameinfo->bet;
		*result = 1;
	}
	else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 > dealerscore) {//�������� �̱� ���
		playerinfo->bankroll += gameinfo->bet*2;
		playerinfo->result += gameinfo->bet;
		*result = 2;
	}
	else if (!(dealerscore == 21 && dcnt == 2) && playerscore1 == dealerscore) {//������ ������ �ƴ� ��Ȳ���� ��� ���
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
		printf("%d�� �÷��̾��� 1���� ������ �������� �����ż� �й��ϼ̽��ϴ�. \n\n",player->id);
	}
	else if (result[0] == 1 && score_sum(player,0)<=21) {
		printf("������ ����Ʈ�����Ƿ� %d�� �÷��̾�� 1���� ���ؼ� �¸��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (result[0] == 2 && score_sum(player, 0) <= 21) {
		printf("%d�� �÷��̾��� 1���� ������ �������� ���� �¸��߽��ϴ�. \n\n", player->id);
	}
	else if(result[0]==3){
		printf("%d�� �÷��̾��� 1���� ���� ���̽��ϴ�. \n\n", player->id);
	}
	else if (player->state[0] == 4) {
		printf("%d�� �÷��̾�� 1���� ���� ����Ʈ�Ǿ� �й��ϼ̽��ϴ�. \n\n", player->id);
	}
	if (result[1] == 0) {
		printf("%d�� �÷��̾��� 2���� ������ �������� �����ż� �й��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (result[1] == 1 && score_sum(player, 1) <= 21) {
		printf("������ ����Ʈ�����Ƿ� %d�� �÷��̾�� 2���� ���ؼ� �¸��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (result[1] == 2 && score_sum(player, 1) <= 21) {
		printf("%d�� �÷��̾��� 2���� ������ �������� ���� �¸��߽��ϴ�. \n\n", player->id);
	}
	else if(result[1]==3){
		printf("%d�� �÷��̾��� 2���� ���� ���̽��ϴ�. \n\n", player->id);
	}
	else if (player->state[1] == 4) {
		printf("%d�� �÷��̾� 2���� ���� ����Ʈ�Ǿ� �й��ϼ̽��ϴ�. \n\n", player->id);
	}
}
void normal_resultoutput(int result,GAMEINFO* player) {
	if (result == 0) {
		printf("%d�� �÷��̾��� ������ �������� �����ż� �й��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (result == 1 && score_sum(player, 0) <= 21) {
		printf("������ ����Ʈ�����Ƿ� %d�� �÷��̾�� �¸��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (result == 2 && score_sum(player, 0) <= 21) {
		printf("%d�� �÷��̾��� ������ �������� ���� �¸��߽��ϴ�. \n\n", player->id);
	}
	else if(result==3){
		printf("%d�� �÷��̾�� ���̽��ϴ�. \n\n", player->id);
	}
	else if(result==4){
		printf("%d�� �÷��̾�� �������� �¸��ϼ̽��ϴ�. \n\n", player->id);
	}
	else if (player->state[0] == 4) {
		printf("%d�� �÷��̾�� ����Ʈ�Ǿ� �й��ϼ̽��ϴ�. \n\n", player->id);
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