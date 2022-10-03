#include "cprocessing.h"
#include <stdio.h>

// Define variables
CP_Image m_logo;
float m_alpha;
float m_mouseX, m_mouseY;

// Local functions
void updateAlpha(void) {
	// dt divide by total transition time (2s) * 255 (max alpha)
	m_alpha += (CP_System_GetDt() / 2.f) * 255.f;
	if (m_alpha >= 255.f) 
		m_alpha = 0.f;
}

void updateCursor(void) {
	int circleDiameter = 20;
	CP_Color circleColor = CP_Color_Create(255, 0, 0, 255);
	CP_Settings_Fill(circleColor);

	m_mouseX = CP_Input_GetMouseX();
	m_mouseY = CP_Input_GetMouseY();

	CP_Graphics_DrawCircle(m_mouseX, m_mouseY, circleDiameter);
}

void splash_screen_init(void)
{
	// Clamp framerate
	CP_System_SetFrameRate(30.f);

	// Init variables
	m_logo = CP_Image_Load("../assets/DigiPen_BLACK.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	CP_System_SetWindowSize(CP_Image_GetWidth(m_logo), CP_Image_GetHeight(m_logo));

	m_alpha = 0;
}

void splash_screen_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(m_logo, 0.f, 0.f, CP_Image_GetWidth(m_logo), CP_Image_GetHeight(m_logo), m_alpha);

	updateAlpha();
	updateCursor();

	if (CP_Input_KeyDown(KEY_Q))
		CP_Engine_Terminate();
}

void splash_screen_exit(void)
{
	// clear mem
	CP_Image_Free(&m_logo);
}