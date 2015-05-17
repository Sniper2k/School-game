#pragma once
#include "Play.h"
#include "Global.h"
#include "stdafx.h"

class Full
{
public:
	enum U_type{u_start,u_game,u_end, u_none};

	Play m_play;
	int m_score;

	const DWORD m_button;
	U_type type;

	int Start_Game();
	int End_Game();
	int End_Draw(HDC hdc);
	int Update();
	int Draw(HDC hdc);
	static INT_PTR CALLBACK WinDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	Full(void);
	~Full(void);
};

