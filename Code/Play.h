#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Map.h"
#include "Mouse.h"
#include "Global.h"

class Play
{
public:
	struct Damaged
	{
		Damaged(POINT cor1):status(true),cor(cor1){}
		POINT cor;
		bool status;
	};

	Player m_blue;
	Player m_red;
	Map m_map;
	Mouse m_mouse;

	bool m_end;
	bool m_end_turn_button;
	bool m_turn_ended;
	bool m_light;

	int m_timer_light;

	Player* m_turn;
	Player* m_non_turn;

	std::vector<RECT> m_update_zone;
	std::vector<Damaged> m_damaged;

	HBITMAP m_dmg_icon;
	HBITMAP m_green_rect;
	HBITMAP m_red_rect;
	std::vector<std::vector<HBITMAP> > m_unit_icon;

	const DWORD m_static;
	const DWORD m_button;

	int m_turns;

	CAtlString m_last_action;

	int Game();
	bool Update();
	int Draw(HDC hdc);
	int End_Turn();
	int Start();
	int Refresh();
	int Start_Print(HDC hdc);
	int Set_Static_Text();
public:
	Play(void);
	~Play(void);
};

