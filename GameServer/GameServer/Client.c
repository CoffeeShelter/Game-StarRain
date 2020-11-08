#include "myGame.h"

#define MSG_SIZE 1024

int Process(SOCKET* client, char* msg, Users* head); //Ŭ���̾�Ʈ �κ��� ���� �䱸���� ó�� �Լ�
void strToArray(char* pStr, int pStr_len, char aStr[]); //������ ���ڿ��� �迭 ���ڿ��� ��ȯ
void encoding(char* str, int key); //��й�ȣ ��ȣȭ

// Ŭ���̾�Ʈ ������
void* Client(void* data) {
	struct clientData* threadData = (struct clientData*)data;
	SOCKET client = threadData->client;
	Users* head = threadData->head;

	char msg[MSG_SIZE];	//���� ���� �޼��� ����
	int result = 0;	//���� ���

	while (1) {
		//�䱸���� �ޱ�
		result = recv(client, msg, sizeof(msg) - 1, 0); //���� null�� ���� ���� ����.
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			closesocket(client);
			printf("Ŭ���̾�Ʈ ���� ����\n");
			return NULL;
		}
		msg[result] = '\0'; //�� �κп� null ���� �߰�.

		//�䱸���� ó�� ���� �䱸 �Ǵ� ���� �� -1 ��ȯ, ����ó�� �� 0��ȯ
		result = Process(&client, msg, head);
		if (result != 0) break;

	}

	printf("Ŭ���̾�Ʈ ������ ����\n");
	closesocket(client);
	return NULL;
}

//Ŭ���̾�Ʈ �� �䱸 ó��
int Process(SOCKET* client, char* msg, Users* head) {
	char* command;
	int result = 0;

	if (msg == NULL) return -1;

	command = strtok(msg, "/");	//���� ���� �޼����� "/" ��ū���� �и�

	//�α���
	//	login/<���̵�>/<��й�ȣ>
	if (strcmp(command, "login") == 0) {

		Users* user = NULL;
		char* id = strtok(NULL, "/");
		char* password = strtok(NULL, "/");

		char arrPassword[32]; //��й�ȣ�� ���� ���ڿ� �迭
		strToArray(password, (int)strlen(password), arrPassword);	//�������� ���ڿ��� �迭�� ���ڿ��� ��ȯ
		encoding(arrPassword, PASSWORD_KEY);	//��ȣȭ

		user = FindUser_ID(head, id);
		if (user == NULL) {
			result = send(*client, "no_id", strlen("no_id"), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		if (strcmp(user->data.password, arrPassword) != 0) {
			result = send(*client, "no_password", strlen("no_password"), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}
		// message -> <�г���>/<�ְ�����>
		else {
			char message[256] = { '\0' };
			char score[16];
			strcpy(message, user->data.name);
			strcat(message, "/");
			sprintf(score, "%d", user->data.score);
			strcat(message, score);
			result = send(*client, message, strlen(message), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		return 0;
	}

	//ȸ������
	//	signin/<���̵�>/<��й�ȣ>/<�г���>
	else if (strcmp(command, "signin") == 0) {
		char* id = strtok(NULL, "/");
		char* password = strtok(NULL, "/");
		char* nickName = strtok(NULL, "/");
		char arrPassword[32]; //��й�ȣ�� ���� ���ڿ� �迭
		Users* user = NULL;
		int result = 0;

		strToArray(password, (int)strlen(password), arrPassword);	//�������� ���ڿ��� �迭�� ���ڿ��� ��ȯ
		encoding(arrPassword, PASSWORD_KEY);	//��ȣȭ

		user = FindUser_ID(head, id);
		//�ߺ��Ǵ� ���̵� �ִ� ���
		// send -> no_id
		if (user != NULL) {
			result = send(*client, "no_id", strlen("no_id"), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		user = FindUser(head, nickName);
		//�ߺ��Ǵ� �г����� �ִ� ���
		// send -> no_nickname
		if (user != NULL) {
			result = send(*client, "no_nickname", strlen("no_nickname"), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		// ���̵�, �г��� �� �� �ߺ��� ���� ���
		// send -> welcome
		if (user == NULL) {
			result = send(*client, "welcome", strlen("welcome"), 0);
			if (result == SOCKET_ERROR) {
				printf("�����ڵ� %d\n", WSAGetLastError());
				return -1;
			}
			Info userInfo;
			strcpy(userInfo.id, id);
			strcpy(userInfo.password, arrPassword);
			strcpy(userInfo.name, nickName);
			userInfo.score = 0;
			AddUser(head, userInfo);
			printf("%s �� ���� �߰� �Ϸ�\n", userInfo.name);
		}
	}

	//��ũ ����
	//	rank/<�г���>
	//  send -> <�г���>/<����>/ ...(10�����) .. /<�г���>/<����>/myRank/<����>/<���>
	else if (strcmp(command, "rank") == 0) {
		int i = 0, rank = 0;
		Users* temp;	//���� �ӽ� ����

		char* myName = strtok(NULL, "/");	//�ش� Ŭ���̾�Ʈ ���� �г���
		char nickName[16], score[16], rankString[16];	//�г���, ����, ���
		char message[1024] = { '\0' };	//������ �޼���


		//10��(10�����)
		temp = head->next;
		for (i = 0; i < 10; i++) {
			if (temp == NULL) break;
			strcpy(nickName, temp->data.name);
			sprintf(score, "%d", temp->data.score);

			strcat(message, nickName);
			strcat(message, "/");
			strcat(message, score);
			strcat(message, "/");
			temp = temp->next;
		}

		rank = FindUser_RANKING(head, myName);
		sprintf(rankString, "%d", rank);
		if (rank == -1) return -1;

		temp = FindUser(head, myName);

		strcat(message, "myRank/");
		sprintf(score, "%d", temp->data.score);
		strcat(message, score);
		strcat(message, "/");
		strcat(message, rankString);

		printf("%s\n", message);

		result = send(*client, message, strlen(message), 0);
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return -1;
		}
	}

	//���� ����
	// score/<�г���>/<����>
	else if (strcmp(command, "score") == 0) {
		char* nickName = strtok(NULL, "/");
		char* score = strtok(NULL, "/");
		Users* user;

		user = FindUser(head, nickName);
		user->data.score = atoi(score);
		SortUser(head, user);

		printf("%s ���� �ֽ�ȭ �Ϸ�.\n", nickName);
	}

	else if (strcmp(command, "exit") == 0) {
		return -1;
	}

	return 0;
}

//������ ���ڿ��� �迭 ���ڿ��� ��ȯ
void strToArray(char* pStr, int pStr_len, char aStr[]) {
	int i = 0;
	for (i = 0; i < pStr_len; i++) {
		aStr[i] = pStr[i];
	}
	aStr[i] = '\0';
}

//���ڿ� ��ȣȭ
void encoding(char* str, int key) {
	for (int i = 0; i < (int)strlen(str); i++) {
		str[i] = str[i] ^ key;
	}
}