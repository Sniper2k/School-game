#pragma once
#include "Unit.h"

class Mouse
{
public:
	POINT m_cor;
	POINT m_rect_now;
	bool m_click;
	bool m_end;
	bool m_colour;

	int On_mouse_move(LPARAM lp);
	int On_mouse_click();

	Mouse(void);
	~Mouse(void);
};

