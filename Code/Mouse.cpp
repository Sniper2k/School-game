#include "StdAfx.h"
#include "Mouse.h"


Mouse::Mouse(void):
m_end(true),m_click(false),m_colour(false)
{
	m_cor.x=327;
	m_cor.y=327;
	m_rect_now.x=5;
	m_rect_now.y=5;
	
}


Mouse::~Mouse(void)
{
}

int Mouse::On_mouse_move(LPARAM lp)
{
	if (!m_end)
	{
		m_cor.x=LOWORD(lp);
		m_cor.y=HIWORD(lp);
	}
	return 0;
}

int Mouse::On_mouse_click()
{
	m_click=true;
	return 0;
}
