#include "myGame.h"

void* Accept(void* acceptData) {
	struct acceptData* data = (struct acceptData*)acceptData;
	SOCKET* server = data->server;	//���� ����
	SOCKET client;	//Ŭ���̾�Ʈ ����
	SOCKADDR_IN clientAddr;	//Ŭ���̾�Ʈ �ּ�
	int clientAddr_len = sizeof(clientAddr);	//Ŭ���̾�Ʈ �ּ� ����

	pthread_t clientThread;	//Ŭ���̾�Ʈ ������
	struct clientData clntData;
	clntData.head = data->head;

	while (1) {
		client = accept(*server, (void*)&clientAddr, &clientAddr_len);	//���� ����
		if (client == INVALID_SOCKET) {
			printf("accept �Լ� ����(�����ڵ� : %d)\n", WSAGetLastError());
			return NULL;
		}
		printf("Ŭ���̾�Ʈ ���� ����(%s)\n", inet_ntoa(clientAddr.sin_addr));
		//Ŭ���̾�Ʈ ������ ����
		clntData.client = &client;
		pthread_create(&clientThread, NULL, Client, (void*)&clntData);
		pthread_detach(clientThread);	//������ �и�
	}

	return NULL;
}