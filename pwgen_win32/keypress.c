//keypressinit.c

#include <windows.h>

/* sends a key pressed event */
static void
_key_down(char vk)
{
	char scan = 0;
	scan = MapVirtualKeyA(vk,0);

	keybd_event((char)vk,
			scan,
			0, /* down */
			0);
}

/* sends a key released event */
static void
_key_up(char vk)
{
	char scan = 0;
	scan = MapVirtualKeyA(vk,0);

	keybd_event((char)vk,
			scan,
			KEYEVENTF_KEYUP,
			0);
}


void KeyboardPaste( void )
{
    Sleep(2000);
    _key_down(VK_CONTROL);
    _key_down(0x56); //"v"
    //Sleep(30);
    _key_up(0x56);
    _key_up(VK_CONTROL);
}
