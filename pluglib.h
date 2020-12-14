#ifndef H_PLUGLIB_
#define H_PLUGLIB_

#define true 1
#define false !true

void pluglib_color(char *color);
void pluglib_gotoxy(int x, int y);
void pluglib_cur_state(int state);
void pluglib_clear_scr(void);
void pluglib_cursor_size(int size);
void pluglib_real_time(void);
void pluglib_millisleep(int wait);
void pluglib_secsleep(int wait);
void pluglib_quick_edit(int edit);
void pluglib_win_kbd(void);
void pluglib_mouse(char move, int *x, int *y, int *c);
void pluglib_setmouse_pos(int x, int y, int click);
void pluglib_console_font(int size);
void pluglib_str_len(char *str);
void pluglib_win_pos(char *window, int x, int y);
void pluglib_msgbox(char *msgtitle, char *msgbody, int code);
void pluglib_draw_line(int Ax, int Bx, int Ay, int By);
int pluglib_transparent(char *window,long percentage);
void pluglib_set_title(char *title);
void pluglib_version(void);
void pluglib_help(void);
enum {NONE = 0, D_LEFT_BUTTON, D_RIGHT_BUTTON, LEFT_BUTTON,RIGHT_BUTTON};

#endif

