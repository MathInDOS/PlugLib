#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "pluglib.h"

WINBASEAPI WINBOOL WINAPI SetConsoleFont(HANDLE hConsole, int size);
static int org_x, org_y;

void pluglib_color(char *color)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	int input_color = (int)strtol(color, NULL, 16);
	char bg = input_color / 16;
	char fg = input_color % 16;
	SetConsoleTextAttribute(hOut, fg | bg<<4);
}


void pluglib_gotoxy(int x, int y)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(hOut, Pos);
}

void pluglib_cur_state(int state)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	
	switch(state)
	{
		case 0:
		cci.bVisible = TRUE;
		break;
		case 1:
		cci.bVisible = FALSE;
		break;
	}
	SetConsoleCursorInfo(hOut, &cci);
}

void pluglib_clear_scr(void)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dw;
	COORD Pos;
	Pos.X = 0;
	Pos.Y = 0;
	FillConsoleOutputCharacter(hOut, ' ', dw, Pos, &dw);
	pluglib_gotoxy(0,0);
}

void pluglib_cursor_size(int size)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	if(size > 100 || size < 0)exit(1);
	if(size==0)exit(1);
	if(!size)exit(1);
	cci.dwSize = size;
	SetConsoleCursorInfo(hOut, &cci);
}

void pluglib_real_time(void)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	printf("%d %d %d %d %d %d", (int)st.wDayOfWeek, (int)st.wYear, (int)st.wMonth, (int)st.wHour, (int)st.wMinute, (int)st.wSecond);
}

void pluglib_millisleep(int wait)
{
	Sleep(wait);
}

void pluglib_secsleep(int wait)
{
	Sleep(wait*1000);
}

void pluglib_quick_edit(int edit)
{
	HANDLE hOut = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwMode;
	GetConsoleMode(hOut, &dwMode);
	dwMode = dwMode | ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;
	
	if(edit == 0)
	   dwMode = dwMode & ~ENABLE_QUICK_EDIT_MODE;
	else
	   dwMode = dwMode | ENABLE_QUICK_EDIT_MODE;
	   
	SetConsoleMode(hOut, dwMode);
}

void pluglib_win_kbd(void)
{
	int winkbd = _getch();
	if((!winkbd) || (0xe0 == winkbd))
	{
		winkbd = _getch();
	    winkbd += 256;
	}
}

void pluglib_mouse(char move, int *x, int *y, int *c)
{
	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	DWORD e;
	SetConsoleMode(console, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	INPUT_RECORD ir;

	*c = NONE;

	do {
		do
			ReadConsoleInput(console, &ir, 1, &e);
		while (ir.EventType != MOUSE_EVENT);

		COORD mouse_pos = ir.Event.MouseEvent.dwMousePosition;
		*x = mouse_pos.X;
		*y = mouse_pos.Y;

		DWORD m_bs = ir.Event.MouseEvent.dwButtonState;
		DWORD m_ef = ir.Event.MouseEvent.dwEventFlags;

		if (m_bs & FROM_LEFT_1ST_BUTTON_PRESSED)
			*c = (m_ef & DOUBLE_CLICK) ? D_LEFT_BUTTON : LEFT_BUTTON;
		
		else if(m_bs & RIGHTMOST_BUTTON_PRESSED)
			*c = (m_ef & DOUBLE_CLICK) ? D_RIGHT_BUTTON : RIGHT_BUTTON;

		else {
			*c = NONE;
			if (move)
				break;
		}

	} while (*c == NONE);
}


void pluglib_setmouse_pos(int x, int y, int click)
{
	INPUT Input = {0};
	POINT pos;
	GetCursorPos(&pos);
	
	switch(click)
	{
		case 0: // Nothing Click
		click = 0;
		break;
		case 1: // Left Click
		click = 1;
		break;
		case 2: // Right Click
		click = 2;
		break;
		case 3: // D Click
		click = 3;
		break;
		case 4: // U Click
		click = 4;
		break;
	}
 
	SetCursorPos(x,y);
    if (click > 0) {
	if (click != 4) {
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = click!=2? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
		SendInput(1,&Input,sizeof(INPUT));
	}
	if (click != 3) {
		ZeroMemory(&Input,sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = click!=2? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
		SendInput(1,&Input,sizeof(INPUT));
	}
	}

}

void pluglib_console_font(int size)
{
	int realsize;
	if(size < 1 || size > 99)
	{
		realsize = 25;
	}else{realsize = size;}
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleFont(hOut, realsize);
}

void pluglib_str_len(char *str)
{
	printf("%d", strlen(str));
}

void pluglib_win_pos(char *window, int x, int y)
{
	RECT rect;
    HWND hwnd = FindWindow(NULL, window);
    if(hwnd==NULL)exit(1);
    GetWindowRect(hwnd, &rect);
	
    if(x < 0 || y < 0)exit(1);
    SetWindowPos(hwnd, HWND_TOP, x, y, rect.right-rect.left, rect.bottom-rect.top, 0);
}


void pluglib_msgbox(char *msgtitle, char *msgbody, int code)
{
	switch(code)
	{
case 0: MessageBox(NULL, msgbody, msgtitle, MB_OK);break;case 1: MessageBox(NULL, msgbody, msgtitle, MB_ICONHAND|MB_OK);break;case 2: MessageBox(NULL, msgbody, msgtitle, MB_ICONINFORMATION|MB_OK);break;case 3: MessageBox(NULL, msgbody, msgtitle, MB_ICONWARNING|MB_OK);break;case 4: MessageBox(NULL, msgbody, msgtitle, MB_ICONQUESTION|MB_OK);break;case 5: MessageBox(NULL, msgbody, msgtitle, MB_ICONEXCLAMATION|MB_OK);break;}

}


void pluglib_draw_line(int Ax, int Bx, int Ay, int By)
{

    // Based On: https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	
    int dx = abs(Bx-Ax), sx = Ax < Bx ? 1 : -1;
    int dy = abs(By-Ay), sy = Ay < By ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;
	
    while (true) {
        pluglib_gotoxy(org_x + Ax, org_y + Ay);
        putchar(' ');
        if (Ax == Bx && Ay == By)
           break;
        e2 = err;
        if (e2 >-dx) {
           err -= dy;
           Ax += sx;
        }

        if (e2 < dy) {
          err += dx;
          Ay += sy;
        }
    }

}

int pluglib_transparent(char *window,long percentage)
{
	HWND hwnd = FindWindow(NULL, window);
	if(hwnd==NULL)return 1;
	if(percentage==0)return 1;
	if(percentage > 99 || percentage < 1)return 1;
	BYTE alpha = 0;
	LONG newLong = 0;
	if(hwnd && percentage > -1 && percentage < 100)
	{
		alpha = (BYTE)(2.55 * (100 - percentage) + 0.5);
		newLong = GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
		if (!SetWindowLong(hwnd, GWL_EXSTYLE, newLong))return FALSE;
		return SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
	}
	return FALSE;
}

void pluglib_set_title(char *title)
{
	SetConsoleTitle(title);
}

void pluglib_version(void)
{
	printf("PlugLib Version-1.0");
}

void pluglib_help(void)
{
	printf("PlugLib Version-1.0\nCopyright (C) 2018-2020 MathInDOS\n\nUsage and following things are on README.md\n");
}


	

