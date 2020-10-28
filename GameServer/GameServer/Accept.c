#include "myGame.h"

void* Accept(void* acceptData) {
	struct acceptData* data = (struct acceptData*)acceptData;
	SOCKET* server = data->server;	//서버 소켓
	SOCKET client;	//클라이언트 소켓
	SOCKADDR_IN clientAddr;	//클라이언트 주소
	int clientAddr_len = sizeof(clientAddr);	//클라이언트 주소 길이

	pthread_t clientThread;	//클라이언트 쓰레드
	struct clientData clntData;
	clntData.head = data->head;

	while (1) {
		client = accept(*server, (void*)&clientAddr, &clientAddr_len);	//연결 수락
		if (client == INVALID_SOCKET) {
			printf("accept 함수 에러(에러코드 : %d)\n", WSAGetLastError());
			return NULL;
		}
		printf("클라이언트 연결 성공(%s)\n", inet_ntoa(clientAddr.sin_addr));
		//클라이언트 쓰레드 실행
		clntData.client = &client;
		pthread_create(&clientThread, NULL, Client, (void*)&clntData);
		pthread_detach(clientThread);	//쓰레드 분리
	}

	return NULL;
}