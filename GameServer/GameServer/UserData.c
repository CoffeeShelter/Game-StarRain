#include "myGame.h"

//���� ����Ʈ�� �߰� (���� ���� �� ���� �߰�)
int AddUser(Users* head, Info userInfo) {
	Users* newUser = malloc(sizeof(Users));
	Users* temp = NULL;
	Users* prev = NULL;

	if (newUser == NULL) {
		printf("�޸� ���� ���� �߰� �Ұ�.\n");
		return -1;
	}

	newUser->data = userInfo;
	newUser->next = NULL;

	if (head->next == NULL)
		head->next = newUser;
	else {
		prev = head;
		temp = head->next;
		while (temp != NULL) {
			if (temp->data.score < newUser->data.score) {	// ������ ū ������ ����.
				newUser->next = temp;
				prev->next = newUser;
				break;
			}
			prev = temp;
			temp = temp->next;
			if (temp == NULL) {
				prev->next = newUser;
			}
		}
	}
	return 0;
}

void SortUser(Users* head, Users* target) {
	Users* temp = head->next;
	Users* prev = head;
	while (temp != NULL) {

		if (temp == target) {
			prev->next = temp->next;	// ���� ��� ����Ʈ ����
			break;
		}

		prev = temp;
		temp = temp->next;
	}

	temp = head->next;
	prev = head;
	while (temp != NULL) {
		if (temp->data.score < target->data.score) {	// �ڽź��� ������ ���� ����Ʈ �տ� ����
			target->next = temp;
			prev->next = target;
			break;
		}
		prev = temp;
		temp = temp->next;
		if (temp == NULL) {
			prev->next = target;
			break;
		}
	}

}

//���� ����Ʈ���� ����
void DelUser(Users* head, char* nickName) {
	Users* temp = head->next, * prev = head;
	while (temp != NULL) {
		if (strcmp(temp->data.name, nickName) == 0) {
			prev->next = temp->next;
			free(temp);
			break;
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

//�̸����� ���� �˻� (ã���� Users ����ü �ּ� ��ȯ)
Users* FindUser(Users* head, char* nickName) {
	Users* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data.name, nickName) == 0)
			return temp;
		temp = temp->next;
	}

	return NULL;
}

//ID�� ���� �˻� (ã���� Users ����ü �ּ� ��ȯ)
Users* FindUser_ID(Users* head, char* id) {
	Users* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data.id, id) == 0)
			return temp;
		temp = temp->next;
	}

	return NULL;
}

//�̸����� ���� �˻� (ã���� ���� ��ŷ ���� ��ȯ)
int FindUser_RANKING(Users* head, char* nickName) {
	int ranking = 0;
	Users* temp = head;	//ù ��� ������ x �׷��Ƿ� ranking 0���� ����
	while (temp != NULL) {
		if (strcmp(temp->data.name, nickName) == 0)
			return ranking;
		temp = temp->next;
		ranking++;
	}

	//���� ��
	return -1;
}

//���� ����Ʈ ��ü ���
void PrintUser(Users* head) {
	Users* temp;
	char key;	//Ű���� �Է� �ڵ�
	int start = 1, end = 10, count = 1;
	int maxCount;

	//���� ����Ʈ ���� ����
	temp = head->next;
	maxCount = 0;
	while (temp != NULL) {
		temp = temp->next;
		maxCount++;
	}

	do {
		system("cls");
		temp = head->next;
		while (temp != NULL) {
			if (count > end) break;
			if (count >= start && count <= end) {
				printf("%03d�� �̸�: %s\t����: %d\n", count, temp->data.name, temp->data.score);
			}
			temp = temp->next;
			count++;
		}
		printf("\n���� 'a' \t ���� 'd' \t ���� 'q'\n");

		key = _getch();
		switch (key) {
			//���� �׸�
		case 'a':
			if (start != 1) {
				start -= 10;
				end -= 10;
			}
			break;

			//���� �׸�
		case'd':
			if (maxCount > end) {
				start += 10;
				end += 10;
			}
			break;

			//ESC ��� ����
		case 'q':
			system("cls");
			return;
			break;

		default:
			break;
		}
		count = 1;
	} while (key != 'q');

}

//���� ����Ʈ ���Ͽ� ����
void PrintUser_FILE(FILE* file, Users* head) {
	Users* temp = head->next;
	while (temp != NULL) {
		fprintf(file, "%s %s %s %d\n", temp->data.id, temp->data.password, temp->data.name, temp->data.score);
		temp = temp->next;
	}
}

//���� ����Ʈ ����
void FreeList(Users* head) {
	Users* temp;

	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}

}