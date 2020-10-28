#include "myGame.h"

//유저 리스트에 추가 (점수 높은 순 정렬 추가)
int AddUser(Users* head, Info userInfo) {
	Users* newUser = malloc(sizeof(Users));
	Users* temp = NULL;
	Users* prev = NULL;

	if (newUser == NULL) {
		printf("메모리 부족 유저 추가 불가.\n");
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
			if (temp->data.score < newUser->data.score) {	// 점수가 큰 순으로 저장.
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
			prev->next = temp->next;	// 정렬 대상 리스트 빼기
			break;
		}

		prev = temp;
		temp = temp->next;
	}

	temp = head->next;
	prev = head;
	while (temp != NULL) {
		if (temp->data.score < target->data.score) {	// 자신보다 점수가 적은 리스트 앞에 삽입
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

//유저 리스트에서 삭제
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

//이름으로 유저 검색 (찾으면 Users 구조체 주소 반환)
Users* FindUser(Users* head, char* nickName) {
	Users* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data.name, nickName) == 0)
			return temp;
		temp = temp->next;
	}

	return NULL;
}

//ID로 유저 검색 (찾으면 Users 구조체 주소 반환)
Users* FindUser_ID(Users* head, char* id) {
	Users* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->data.id, id) == 0)
			return temp;
		temp = temp->next;
	}

	return NULL;
}

//이름으로 유저 검색 (찾으면 유저 랭킹 순위 반환)
int FindUser_RANKING(Users* head, char* nickName) {
	int ranking = 0;
	Users* temp = head;	//첫 헤드 데이터 x 그러므로 ranking 0부터 시작
	while (temp != NULL) {
		if (strcmp(temp->data.name, nickName) == 0)
			return ranking;
		temp = temp->next;
		ranking++;
	}

	//없을 시
	return -1;
}

//유저 리스트 전체 출력
void PrintUser(Users* head) {
	Users* temp;
	char key;	//키보드 입력 코드
	int start = 1, end = 10, count = 1;
	int maxCount;

	//유저 리스트 길이 측정
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
				printf("%03d번 이름: %s\t점수: %d\n", count, temp->data.name, temp->data.score);
			}
			temp = temp->next;
			count++;
		}
		printf("\n이전 'a' \t 다음 'd' \t 종료 'q'\n");

		key = _getch();
		switch (key) {
			//이전 항목
		case 'a':
			if (start != 1) {
				start -= 10;
				end -= 10;
			}
			break;

			//다음 항목
		case'd':
			if (maxCount > end) {
				start += 10;
				end += 10;
			}
			break;

			//ESC 출력 종료
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

//유저 리스트 파일에 저장
void PrintUser_FILE(FILE* file, Users* head) {
	Users* temp = head->next;
	while (temp != NULL) {
		fprintf(file, "%s %s %s %d\n", temp->data.id, temp->data.password, temp->data.name, temp->data.score);
		temp = temp->next;
	}
}

//유저 리스트 헤재
void FreeList(Users* head) {
	Users* temp;

	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}

}