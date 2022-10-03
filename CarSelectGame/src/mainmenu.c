#include <stdio.h>

#include "utils.h"
#include "carlevel.h"

// Define constants
#define BUTTON_HEIGHT 75.f
#define BUTTON_WIDTH 150.f

// Declare structs
typedef struct Button
{
	const char* text;		// Text
	int			tSize;		// Font size
	CP_Color	tcolor;		// Font color
	CP_Color	color;		// Box color
	CP_Vector	pos;		// Position vector
}Button;

// Define variables
Button m_ButtonArray[2];
Button* PlayButton;
Button* ExitButton;

// Declare local functions
void Button_Init(void);
void Button_Update(void);

// CP functions
void Main_Menu_init(void) 
{
	CP_System_SetWindowSize(800, 800);
	Button_Init();
}

void Main_Menu_update(void) 
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
	Button_Update();
		
	if (CP_Input_MouseClicked() && 
		IsAreaClicked(PlayButton->pos.x, PlayButton->pos.y, BUTTON_WIDTH, BUTTON_HEIGHT,
					  CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		CP_Engine_SetNextGameState(Car_Level_init, Car_Level_update, Car_Level_exit);
	if (CP_Input_MouseClicked() &&
		IsAreaClicked(ExitButton->pos.x, ExitButton->pos.y, BUTTON_WIDTH, BUTTON_HEIGHT,
			CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		CP_Engine_Terminate();
}

void Main_Menu_exit(void) 
{
	PlayButton = NULL;
}


// Init local functions
void Button_Init() {
	CP_Settings_TextSize(20.f);
	Button btn_Play = (Button){
		.color = CP_Color_Create(255, 0, 0, 255),
		.pos.x = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * .5f,
		.pos.y = CP_System_GetWindowHeight() * 0.4f - BUTTON_HEIGHT * .5f,
		.tcolor = CP_Color_Create(0, 0, 0, 255),
		.text = "Play",
		.tSize = 24
	};
	Button btn_Quit = (Button){
		.color = CP_Color_Create(255, 0, 0, 255),
		.pos.x = CP_System_GetWindowWidth() * 0.5f - BUTTON_WIDTH * .5f,
		.pos.y = CP_System_GetWindowHeight() * 0.66f - BUTTON_HEIGHT * .5f,
		.tcolor = CP_Color_Create(0, 0, 0, 255),
		.text = "Quit",
		.tSize = 24
	};
	m_ButtonArray[0] = btn_Play;
	m_ButtonArray[1] = btn_Quit;
	PlayButton = &m_ButtonArray[0];
	ExitButton = &m_ButtonArray[1];
}

void Button_Update() {
	for (int i = 0; i < NELEMS(m_ButtonArray); i++)
	{
		Button b = m_ButtonArray[i];
		CP_Settings_Fill(b.color);
		CP_Graphics_DrawRect(b.pos.x, b.pos.y, BUTTON_WIDTH, BUTTON_HEIGHT);
		CP_Settings_Fill(b.tcolor);
		CP_Font_DrawText(b.text, b.pos.x + BUTTON_WIDTH * 0.4f, b.pos.y + BUTTON_HEIGHT * 0.6f);
	}
}