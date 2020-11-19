#include "myGame.h"

void loadUser(Users* head); //유저 정보 불러오기
void S_Process(char* msg, Users* head);	//서버 명령 처리

int Server(u_short port) {

	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;

	pthread_t acceptThread;	//Accept쓰레드 변수

	Users* head = (Users*)malloc(sizeof(Users));	//유저 연결리스트
	if (head != NULL) head->next = NULL;

	int error = 0;	//에러 코드

	struct acceptData data;

	//유저 데이터 불러오기
	loadUser(head);

	//WSA데이터 초기화
	if ((error = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup 함수 에러(에러코드 : %d)\n", error);
		return -1;
	}

	//서버 소켓 생성
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		printf("socket 함수 에러(에러코드: %d)\n", WSAGetLastError());
		return -1;
	}

	//소켓에 주소 바인딩
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, (void*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		printf("bind 함수 에러(에러코드 : %d)\n", WSAGetLastError());
		return -1;
	}

	//대기열 생성
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen 함수 에러(에러코드 : %d)\n", WSAGetLastError());
		return -1;
	}
	printf("서버가 성공적으로 열렸습니다.\n");

	//Accept쓰레드 실행
	data.server = &serverSocket;
	data.head = head;
	pthread_create(&acceptThread, NULL, Accept, (void*)&data);	//쓰레드 생성
	pthread_detach(acceptThread);	//쓰레드 분리

	char command[1024];	//명령어
	//서버 작업
	while (1) {
		//명령어 입력
		gets_s(command, sizeof(command));
		//명령어 처리
		S_Process(command, head);

		if (strcmp(command, "exit") == 0) break;
	}

	printf("서버를 종료 합니다.\n");

	closesocket(serverSocket); //서버 소켓 닫기
	WSACleanup();

	//유저 리스트 해제
	FreeList(head);

	return 0;
}

//유저 정보 불러오기
void loadUser(Users* head) {
	FILE* file = NULL;
	Info info;

	fopen_s(&file, "Users.txt", "r");

	if (file == NULL) {
		fopen_s(&file, "Users.txt", "w");
		if (file == NULL) return;
	}

	while (fscanf(file, "%s %s %s %d", info.id, info.password, info.name, &info.score) != EOF) {
		AddUser(head, info);
	}
	fclose(file);

}

void S_Process(char* msg, Users* head) {
	char* command;
	FILE* userFile;
	Users* user;

	command = strtok(msg, " ");

	//유저 정보 출력
	if (strcmp(command, "print") == 0) {
		command = strtok(NULL, " ");
		if (command == NULL)	PrintUser(head);
		else {
			user = FindUser(head, command);
			if (user != NULL)
				printf("[닉네임] : %s\t\t[최고 점수] : %d\n", user->data.name, user->data.score);
			else
				printf("해당 닉네임의 유저가 존재하지 않습니다.\n");
		}
	}
	//유저 삭제
	else if (strcmp(command, "del") == 0) {
		command = strtok(NULL, " ");
		if (command == NULL) {
			printf("삭제할 유저의 닉네임을 입력해주세요.\n");
			printf("ex >> del [닉네임]\n");
		}
		else {
			user = FindUser(head, command);
			if (user == NULL) {
				printf("해당 닉네임의 유저가 존재하지 않습니다.\n");
			}
			else {
				printf("유저 [ %s ] 를 정말 삭제하시겠습니가?\n",user->data.name);
				printf("[YES] : Y ,[NO] : N\n");
				
				char select = '\0';
				while (!(select == 'y' || select == 'n'))
					select = _getch();
				switch (select) {
				case 'y': {
					DelUser(head, user->data.name);
					printf("해당 유저를 삭제하였습니다.\n");
					break;
				}
				case 'n': {
					printf("취소되었습니다.\n");
					break;
				}
				default: {
					printf("잘못 누르셨습니다.\n Y, N 둘중 하나를 입력하여주십시오.\n");
					break;
				}
				}
			}
		}
	}
	//화면 지움
	else if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0) {
		system("cls");
	}
	//서버 종료
	else if (strcmp(command, "exit") == 0) {
		fopen_s(&userFile, "Users.txt", "w");

		if (userFile != NULL) {
			//유저 리스트 파일에 새로 저장
			PrintUser_FILE(userFile, head);
			fclose(userFile);
		}
	}
	//명령어 잘못 입력
	else
		printf("명령어를 잘못 입력하셨습니다.\n");
}