#include "MyGame.h"

//콘솔 창 크기
void SetConsoleSize(int col, int lines)
{
	char Text[100];
	sprintf_s(Text, sizeof(Text), "mode con cols=%d lines=%d", col, lines);
	system(Text);
}

//커서위치
void GotoXY(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//배경,글자 색
void SetColor(unsigned char _BgColor, unsigned char _TextColor)
{
	if (_BgColor > 15 || _TextColor > 15) return;

	unsigned short ColorNum = (_BgColor << 4) | _TextColor;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNum);
}

//화면 지우기
void Clear(void)
{
	system("cls");
}

//커서 온/오프
void SetMyCursor(BOOL _bShow)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = _bShow;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}