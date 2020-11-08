#include "myGame.h"

#define MSG_SIZE 1024

int Process(SOCKET* client, char* msg, Users* head); //클라이언트 로부터 받은 요구사항 처리 함수
void strToArray(char* pStr, int pStr_len, char aStr[]); //포인터 문자열을 배열 문자열로 변환
void encoding(char* str, int key); //비밀번호 암호화

// 클라이언트 쓰레드
void* Client(void* data) {
	struct clientData* threadData = (struct clientData*)data;
	SOCKET client = threadData->client;
	Users* head = threadData->head;

	char msg[MSG_SIZE];	//수신 받을 메세지 변수
	int result = 0;	//수신 결과

	while (1) {
		//요구사항 받기
		result = recv(client, msg, sizeof(msg) - 1, 0); //끝에 null을 포함 하지 않음.
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			closesocket(client);
			printf("클라이언트 소켓 해제\n");
			return NULL;
		}
		msg[result] = '\0'; //끝 부분에 null 문자 추가.

		//요구사항 처리 종료 요구 또는 에러 시 -1 반환, 정상처리 시 0반환
		result = Process(&client, msg, head);
		if (result != 0) break;

	}

	printf("클라이언트 쓰레드 종료\n");
	closesocket(client);
	return NULL;
}

//클라이언트 측 요구 처리
int Process(SOCKET* client, char* msg, Users* head) {
	char* command;
	int result = 0;

	if (msg == NULL) return -1;

	command = strtok(msg, "/");	//전달 받은 메세지를 "/" 토큰으로 분리

	//로그인
	//	login/<아이디>/<비밀번호>
	if (strcmp(command, "login") == 0) {

		Users* user = NULL;
		char* id = strtok(NULL, "/");
		char* password = strtok(NULL, "/");

		char arrPassword[32]; //비밀번호를 담을 문자열 배열
		strToArray(password, (int)strlen(password), arrPassword);	//포인터형 문자열을 배열형 문자열로 변환
		encoding(arrPassword, PASSWORD_KEY);	//암호화

		user = FindUser_ID(head, id);
		if (user == NULL) {
			result = send(*client, "no_id", strlen("no_id"), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		if (strcmp(user->data.password, arrPassword) != 0) {
			result = send(*client, "no_password", strlen("no_password"), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}
		// message -> <닉네임>/<최고점수>
		else {
			char message[256] = { '\0' };
			char score[16];
			strcpy(message, user->data.name);
			strcat(message, "/");
			sprintf(score, "%d", user->data.score);
			strcat(message, score);
			result = send(*client, message, strlen(message), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		return 0;
	}

	//회원가입
	//	signin/<아이디>/<비밀번호>/<닉네임>
	else if (strcmp(command, "signin") == 0) {
		char* id = strtok(NULL, "/");
		char* password = strtok(NULL, "/");
		char* nickName = strtok(NULL, "/");
		char arrPassword[32]; //비밀번호를 담을 문자열 배열
		Users* user = NULL;
		int result = 0;

		strToArray(password, (int)strlen(password), arrPassword);	//포인터형 문자열을 배열형 문자열로 변환
		encoding(arrPassword, PASSWORD_KEY);	//암호화

		user = FindUser_ID(head, id);
		//중복되는 아이디가 있는 경우
		// send -> no_id
		if (user != NULL) {
			result = send(*client, "no_id", strlen("no_id"), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		user = FindUser(head, nickName);
		//중복되는 닉네임이 있는 경우
		// send -> no_nickname
		if (user != NULL) {
			result = send(*client, "no_nickname", strlen("no_nickname"), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			return 0;
		}

		// 아이디, 닉네임 둘 다 중복이 없을 경우
		// send -> welcome
		if (user == NULL) {
			result = send(*client, "welcome", strlen("welcome"), 0);
			if (result == SOCKET_ERROR) {
				printf("에러코드 %d\n", WSAGetLastError());
				return -1;
			}
			Info userInfo;
			strcpy(userInfo.id, id);
			strcpy(userInfo.password, arrPassword);
			strcpy(userInfo.name, nickName);
			userInfo.score = 0;
			AddUser(head, userInfo);
			printf("%s 님 정보 추가 완료\n", userInfo.name);
		}
	}

	//랭크 정보
	//	rank/<닉네임>
	//  send -> <닉네임>/<점수>/ ...(10등까지) .. /<닉네임>/<점수>/myRank/<점수>/<등수>
	else if (strcmp(command, "rank") == 0) {
		int i = 0, rank = 0;
		Users* temp;	//유저 임시 변수

		char* myName = strtok(NULL, "/");	//해당 클라이언트 유저 닉네임
		char nickName[16], score[16], rankString[16];	//닉네임, 점수, 등수
		char message[1024] = { '\0' };	//전송할 메세지


		//10번(10등까지)
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
			printf("에러코드 %d\n", WSAGetLastError());
			return -1;
		}
	}

	//점수 저장
	// score/<닉네임>/<점수>
	else if (strcmp(command, "score") == 0) {
		char* nickName = strtok(NULL, "/");
		char* score = strtok(NULL, "/");
		Users* user;

		user = FindUser(head, nickName);
		user->data.score = atoi(score);
		SortUser(head, user);

		printf("%s 정보 최신화 완료.\n", nickName);
	}

	else if (strcmp(command, "exit") == 0) {
		return -1;
	}

	return 0;
}

//포인터 문자열을 배열 문자열로 변환
void strToArray(char* pStr, int pStr_len, char aStr[]) {
	int i = 0;
	for (i = 0; i < pStr_len; i++) {
		aStr[i] = pStr[i];
	}
	aStr[i] = '\0';
}

//문자열 암호화
void encoding(char* str, int key) {
	for (int i = 0; i < (int)strlen(str); i++) {
		str[i] = str[i] ^ key;
	}
}