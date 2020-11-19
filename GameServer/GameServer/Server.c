#include "myGame.h"

void loadUser(Users* head); //���� ���� �ҷ�����
void S_Process(char* msg, Users* head);	//���� ��� ó��

int Server(u_short port) {

	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;

	pthread_t acceptThread;	//Accept������ ����

	Users* head = (Users*)malloc(sizeof(Users));	//���� ���Ḯ��Ʈ
	if (head != NULL) head->next = NULL;

	int error = 0;	//���� �ڵ�

	struct acceptData data;

	//���� ������ �ҷ�����
	loadUser(head);

	//WSA������ �ʱ�ȭ
	if ((error = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup �Լ� ����(�����ڵ� : %d)\n", error);
		return -1;
	}

	//���� ���� ����
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		printf("socket �Լ� ����(�����ڵ�: %d)\n", WSAGetLastError());
		return -1;
	}

	//���Ͽ� �ּ� ���ε�
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serverSocket, (void*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		printf("bind �Լ� ����(�����ڵ� : %d)\n", WSAGetLastError());
		return -1;
	}

	//��⿭ ����
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen �Լ� ����(�����ڵ� : %d)\n", WSAGetLastError());
		return -1;
	}
	printf("������ ���������� ���Ƚ��ϴ�.\n");

	//Accept������ ����
	data.server = &serverSocket;
	data.head = head;
	pthread_create(&acceptThread, NULL, Accept, (void*)&data);	//������ ����
	pthread_detach(acceptThread);	//������ �и�

	char command[1024];	//��ɾ�
	//���� �۾�
	while (1) {
		//��ɾ� �Է�
		gets_s(command, sizeof(command));
		//��ɾ� ó��
		S_Process(command, head);

		if (strcmp(command, "exit") == 0) break;
	}

	printf("������ ���� �մϴ�.\n");

	closesocket(serverSocket); //���� ���� �ݱ�
	WSACleanup();

	//���� ����Ʈ ����
	FreeList(head);

	return 0;
}

//���� ���� �ҷ�����
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

	//���� ���� ���
	if (strcmp(command, "print") == 0) {
		command = strtok(NULL, " ");
		if (command == NULL)	PrintUser(head);
		else {
			user = FindUser(head, command);
			if (user != NULL)
				printf("[�г���] : %s\t\t[�ְ� ����] : %d\n", user->data.name, user->data.score);
			else
				printf("�ش� �г����� ������ �������� �ʽ��ϴ�.\n");
		}
	}
	//���� ����
	else if (strcmp(command, "del") == 0) {
		command = strtok(NULL, " ");
		if (command == NULL) {
			printf("������ ������ �г����� �Է����ּ���.\n");
			printf("ex >> del [�г���]\n");
		}
		else {
			user = FindUser(head, command);
			if (user == NULL) {
				printf("�ش� �г����� ������ �������� �ʽ��ϴ�.\n");
			}
			else {
				printf("���� [ %s ] �� ���� �����Ͻðڽ��ϰ�?\n",user->data.name);
				printf("[YES] : Y ,[NO] : N\n");
				
				char select = '\0';
				while (!(select == 'y' || select == 'n'))
					select = _getch();
				switch (select) {
				case 'y': {
					DelUser(head, user->data.name);
					printf("�ش� ������ �����Ͽ����ϴ�.\n");
					break;
				}
				case 'n': {
					printf("��ҵǾ����ϴ�.\n");
					break;
				}
				default: {
					printf("�߸� �����̽��ϴ�.\n Y, N ���� �ϳ��� �Է��Ͽ��ֽʽÿ�.\n");
					break;
				}
				}
			}
		}
	}
	//ȭ�� ����
	else if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0) {
		system("cls");
	}
	//���� ����
	else if (strcmp(command, "exit") == 0) {
		fopen_s(&userFile, "Users.txt", "w");

		if (userFile != NULL) {
			//���� ����Ʈ ���Ͽ� ���� ����
			PrintUser_FILE(userFile, head);
			fclose(userFile);
		}
	}
	//��ɾ� �߸� �Է�
	else
		printf("��ɾ �߸� �Է��ϼ̽��ϴ�.\n");
}