#include "cprocessing.h"
#include "mainmenu.h"

int main(void) {
	CP_Engine_SetNextGameState(Main_Menu_init, Main_Menu_update, Main_Menu_exit);
	CP_Engine_Run();
	
	return 0;
}