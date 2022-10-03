#include <stdio.h>

#include "utils.h"
#include "mainmenu.h"


// struct for "car" object
/* 
* With reference to CP_Graphics_DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
* Car has triangle inside the circle. Circle is centered, triangle builds off that.
* Triangle's points have to update per frame
{
	vec   Position // Circle's pos
	CPVec p1 // This is front point, where car faces
	vec   p2
	vec   p3

	float angle // A/D -+ respectively, probably in radians. when reaches 2Pie (full circle), %modulus to reset
	float speed
	char selected // on mouse click, true. mouse click on another, false. 
					// TO BE RE LOOKED AT, If mouse click off, how should i change it to false?
					 
}
*/
typedef struct Car {
	CP_Vector position;
	CP_Vector p1;
	CP_Vector p2;
	CP_Vector p3;

	int radius;

	float angle; // Degrees
	CP_Vector dir; // Direction of car at the moment
	float speed;

	CP_Color color;
}Car;

// Define vars
Car m_CarArray[3];
CP_Color m_CarColors[3];
Car* m_SelectedCar;
float m_SpinRate, m_MoveSpeed;
float m_AngleToPoints;

// Declare local
void Init_Cars(void);
void Init_Positions(void);
char isOverlap(int i, float x, float y);
void Update_Cars(void);
void Update_Points(Car* c);
void Draw_Cars(void);

// CP Functions
void Car_Level_init(void)
{
	m_SpinRate = 5.0f;
	m_MoveSpeed = 100.0f;
	m_AngleToPoints = 150.f;

	m_CarColors[0] = CP_Color_Create(255, 0, 0, 255);
	m_CarColors[1] = CP_Color_Create(0, 255, 0, 255);
	m_CarColors[2] = CP_Color_Create(0, 0, 255, 255);
	
	Init_Cars();
}

void Car_Level_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));

	Update_Cars();

	if (m_SelectedCar)
	{
		float dt = CP_System_GetDt();
		// Control car
		if (CP_Input_KeyDown(KEY_W)) {
			m_SelectedCar->position.x += m_SelectedCar->dir.x * m_MoveSpeed * dt;
			m_SelectedCar->position.y += m_SelectedCar->dir.y * m_MoveSpeed * dt;
		} else if (CP_Input_KeyDown(KEY_S)) {
			m_SelectedCar->position.x -= m_SelectedCar->dir.x * m_MoveSpeed * dt;
			m_SelectedCar->position.y -= m_SelectedCar->dir.y * m_MoveSpeed * dt;
		}

		if (CP_Input_KeyDown(KEY_A))
			m_SelectedCar->angle -= m_MoveSpeed * dt;
		else if (CP_Input_KeyDown(KEY_D))
			m_SelectedCar->angle += m_MoveSpeed * dt;
	}

	if (CP_Input_MouseClicked())
	{
		// Check which car mousepos is within
		// Set that car to selected car
		CP_Vector click = { click.x = CP_Input_GetMouseX(), click.y = CP_Input_GetMouseY() };
		for (int i = 0; i < NELEMS(m_CarArray); i++)
		{
			Car* current = &m_CarArray[i];
			if (IsCircleClicked(current->position.x, current->position.y, current->radius * 2, \
								click.x, click.y))
				m_SelectedCar = current;
		}
	}

	if (CP_Input_KeyDown(KEY_Q))
		CP_Engine_SetNextGameState(Main_Menu_init, Main_Menu_update, Main_Menu_exit);
}

void Car_Level_exit(void)
{
	m_SelectedCar = NULL;
}


// Define local functions
void Init_Cars(void)
{
	// Initialize 3 cars at random ponits.
	// With initialization of each point, 
		// check that point is not within radius of another point.
	Init_Positions();
	// Update_cars once to update p1p2p3
	for (int i = 0; i < NELEMS(m_CarArray); i++)
	{
		Car* current = &m_CarArray[i];
		// Add other vars
		current->radius = 50;
		current->angle = RNG(0, 360);
		current->color = m_CarColors[i];

		// Initialize the dir and points.
		Update_Points(current);
		// Why is third car at 0,0 always
	}
	m_SelectedCar = &m_CarArray[0];
}

void Init_Positions(void)
{
	for (int i = 0; i < NELEMS(m_CarArray); i++)
	{
		int x = 0, y = 0;
		while (isOverlap(i, x, y) || (x == 0 && y == 0))
		{
			x = RNG(0, CP_System_GetWindowWidth());
			y = RNG(0, CP_System_GetWindowHeight());
		}
		m_CarArray[i].position.x = x;
		m_CarArray[i].position.y = y;
	}
}
char isOverlap(int i, float x, float y) // function to check if current x,y overlaps with the others.
{
	for (int j = 0; j < NELEMS(m_CarArray); j++)
	{
		if (i == j)
			continue;
		Car comp = m_CarArray[j]; // Comparison
		if (CP_Math_Distance(x, y, comp.position.x, comp.position.y) <= comp.radius)
			return 1;
	}
	return 0;
}

void Update_Cars(void)
{
	Update_Points(m_SelectedCar);
	Draw_Cars();
}
void Update_Points(Car* c)
{
	// Update car direction
	c->dir = AngleToVector(CP_Math_Radians(c->angle));

	// Add scale of directional vector to position. Scale is calculated using radius / length of dir.
	c->p1 = CP_Vector_Add(c->position, CP_Vector_Scale(c->dir, c->radius / CP_Vector_Length(c->dir)));

	// OtherDir simply calculates another directional vector for the other points' reference. +-m_AngleToPoints
	CP_Vector otherP_dir = AngleToVector(CP_Math_Radians(c->angle + m_AngleToPoints));
	c->p2 = CP_Vector_Add(c->position, CP_Vector_Scale(otherP_dir, c->radius / CP_Vector_Length(otherP_dir)));
	otherP_dir = AngleToVector(CP_Math_Radians(c->angle - m_AngleToPoints));
	c->p3 = CP_Vector_Add(c->position, CP_Vector_Scale(otherP_dir, c->radius / CP_Vector_Length(otherP_dir)));
}
void Draw_Cars(void)
{
	// for each car
	// draw circle at position
	// draw triangle at p1p2p3

	for (int i = 0; i < NELEMS(m_CarArray); i++)
	{
		Car current = m_CarArray[i];
		CP_Settings_Fill(current.color);
		CP_Graphics_DrawCircle(current.position.x, current.position.y, current.radius * 2);
		CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
		CP_Graphics_DrawTriangle(current.p1.x, current.p1.y, \
								 current.p2.x, current.p2.y, \
								 current.p3.x, current.p3.y);// Change to Advanced when degrees is implemented.
	}
}