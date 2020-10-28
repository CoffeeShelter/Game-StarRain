#include "myGame.h"

void Process(char* msg); //���� ���� �޼��� ó��
void* Receiver(void* data);	//�۽� ������

int Client(SOCKET* client, u_short port) {
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	//pthread_t recvThread;//�۽� ������

	struct recvThreadData threadData;
	threadData.socket = client;

	const char* sAddr = "127.0.0.1";	// ���� ������

	int result = 0;

	//WSADATA �ʱ�ȭ
	if ((result = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup �Լ� ����(�����ڵ� : %d)\n", result);
		return -1;
	}

	*client = socket(AF_INET, SOCK_STREAM, 0);	//Ŭ���̾�Ʈ ���� ����
	if (*client == INVALID_SOCKET) {
		printf("socket �Լ� ����(�����ڵ� : %d)\n", WSAGetLastError());
		return -1;
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(port);

	//���� ����
	printf("������ ���� �� �Դϴ�...\n");

	result = connect(*client, (void*)&servAddr, sizeof(servAddr));
	if (result == SOCKET_ERROR) {
		printf("���� ���� (�����ڵ� : %d)\n", WSAGetLastError());
		return -1;
	}

	printf("������ ����Ǿ����ϴ�.\n");

	//pthread_create(&recvThread, NULL, Receiver, (void*)&threadData); //�۽� ������ ����
	//pthread_detach(recvThread);	//�и�

	return 0;
}

//�۽� ������
void* Receiver(void* data) {
	struct recvThreadData* recvData = (struct recvThreadData*)data;
	SOCKET* client = recvData->socket;

	char msg[1024]; //���� ���� �޼��� ����
	int result;	//�۽� ���

	while (1) {
		result = recv(*client, msg, sizeof(msg) - 1, 0);	//���� null�� ���� ���� ����.
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return NULL;
		}
		msg[result] = '\0';

		Process(msg);
	}

	return NULL;
}

//���� ���� �޼��� ó��
void Process(char* msg) {
	char* command = NULL;

	command = strtok(msg, "/");

	if (strcmp(command, "")) {

	}

}