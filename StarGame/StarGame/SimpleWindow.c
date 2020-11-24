#include "MyGame.h"

//�ܼ� â ũ��
void SetConsoleSize(int col, int lines)
{
	char Text[100];
	sprintf_s(Text, sizeof(Text), "mode con cols=%d lines=%d", col, lines);
	system(Text);
}

//Ŀ����ġ
void GotoXY(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//���,���� ��
void SetColor(unsigned char _BgColor, unsigned char _TextColor)
{
	if (_BgColor > 15 || _TextColor > 15) return;

	unsigned short ColorNum = (_BgColor << 4) | _TextColor;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNum);
	//SetConsoleTextAttribute(hBuffer[nScreenIndex], ColorNum);
}

//ȭ�� �����
void Clear(void)
{
	system("cls");
}

//Ŀ�� ��/����
void SetMyCursor(BOOL _bShow)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = _bShow;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//���� ���� �Լ�
void CreatBuffer(){
	COORD size = { CONSOLESIZE_COL, CONSOLESIZE_LINES };
	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT rect;

	rect.Bottom = 0;
	rect.Left = 0;
	rect.Right = CONSOLESIZE_COL - 1;;
	rect.Top = CONSOLESIZE_LINES - 1;

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);


	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hBuffer[0], &cci);
	SetConsoleCursorInfo(hBuffer[1], &cci);
}

// ���� ����
void WriteBuffer(int x, int y, char str[]){

	DWORD dw;
	COORD CursorPosition = { x, y };

	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPosition);
	WriteFile(hBuffer[nScreenIndex], str, strlen(str), &dw, NULL);

}

// ��ũ�� ��ȯ
void FlippingBuffer(){
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);

	nScreenIndex = !nScreenIndex;
}

// ���� ���� �����
void ClearBuffer(){
	COORD Coor = { 0,0 };
	DWORD dw;

	FillConsoleOutputCharacter(hBuffer[nScreenIndex], ' ', CONSOLESIZE_COL * CONSOLESIZE_LINES, Coor, &dw);
}

// ���� ����
void DeleteBuffer(){
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
}