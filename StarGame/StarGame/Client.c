#include "myGame.h"

void Process(char* msg); //전달 받은 메세지 처리
void* Receiver(void* data);	//송신 쓰레드

int Client(SOCKET* client, u_short port) {
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	//pthread_t recvThread;//송신 쓰레드

	struct recvThreadData threadData;
	threadData.socket = client;

	const char* sAddr = "127.0.0.1";	// 서버 아이피

	int result = 0;

	//WSADATA 초기화
	if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup 함수 에러(에러코드 : %d)\n", result);
		return -1;
	}

	*client = socket(AF_INET, SOCK_STREAM, 0);	//클라이언트 소켓 생성
	if (*client == INVALID_SOCKET) {
		printf("socket 함수 에러(에러코드 : %d)\n", WSAGetLastError());
		return -1;
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(port);

	//서버 연결
	printf("서버에 연결 중 입니다...\n");

	result = connect(*client, (void*)&servAddr, sizeof(servAddr));
	if (result == SOCKET_ERROR) {
		printf("연결 실패 (에러코드 : %d)\n", WSAGetLastError());
		return -1;
	}

	printf("서버에 연결되었습니다.\n");

	//pthread_create(&recvThread, NULL, Receiver, (void*)&threadData); //송신 쓰레드 생성
	//pthread_detach(recvThread);	//분리

	return 0;
}

//송신 쓰레드
void* Receiver(void* data) {
	struct recvThreadData* recvData = (struct recvThreadData*)data;
	SOCKET* client = recvData->socket;

	char msg[1024]; //전달 받을 메세지 변수
	int result;	//송신 결과

	while (1) {
		result = recv(*client, msg, sizeof(msg) - 1, 0);	//끝에 null을 포함 하지 않음.
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			return NULL;
		}
		msg[result] = '\0';

		Process(msg);
	}

	return NULL;
}

//전달 받은 메세지 처리
void Process(char* msg) {
	char* command = NULL;

	command = strtok(msg, "/");

	if (strcmp(command, "")) {

	}

}