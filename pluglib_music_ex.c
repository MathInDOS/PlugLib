#include <windows.h>

void pluglib_wav_play(char *filename)
{
	PlaySound(filename, NULL, 0x00020000L|0x0002);
}

// commfile is open file alias
// play file [wait] (Optional)
void pluglib_mp3_play(char *commfile)
{
	mciSendString(commfile, NULL, 0, 0);
	mciSendString(commfile, NULL, 0, 0);
}