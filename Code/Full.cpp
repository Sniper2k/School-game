#include "StdAfx.h"
#include "Full.h"
#include "resource.h"


Full::Full(void):
m_button(1)
{
	type=u_end;
}


Full::~Full(void)
{
}

int Full::Start_Game()
{
	DestroyWindow(GetDlgItem(hWnd,m_button));
	m_play.Start();
	type=u_start;
	InvalidateRect(hWnd,NULL,NULL);
	return 0;
}

INT_PTR CALLBACK Full::WinDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		{
			int ctrl_id=LOWORD(wParam);
			int cmd=HIWORD(wParam);
			switch(ctrl_id)
			{
			case IDOK:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
				break;
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
				break;
			}
			

		}
		break;
	}
	return (INT_PTR)FALSE;
}

int Full::End_Game()
{
	m_score=m_play.m_turns;
	
	DestroyWindow(GetDlgItem(hWnd,m_play.m_button));
	DestroyWindow(GetDlgItem(hWnd,m_play.m_static));
	CreateWindow(_T("BUTTON"),_T("Start"),WS_CHILD|WS_VISIBLE,320,320,320,64,hWnd,(HMENU)m_button,NULL,NULL);

	HWND dlg=CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, WinDlg);
	RECT rc;
	GetWindowRect(dlg,&rc);
	SetWindowPos(dlg,HWND_TOP,500,300,rc.right-rc.left,rc.bottom-rc.top,NULL);
	HWND stat=GetDlgItem(dlg,IDC_STATIC2);
	CAtlString str;
	if (m_play.m_blue.m_team.empty())
		str=_T("Red Win");
	else if (m_play.m_red.m_team.empty())str=_T("Blue Win");
	else str=_T("Draw");
	HFONT font = CreateFont(24,0,0,0,                         
		FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,_T("Old English Text MT"));
	SendMessage(stat,WM_SETFONT,(WPARAM)font,TRUE);
	SetWindowText(stat,LPCTSTR(str));

	stat=GetDlgItem(dlg,IDC_STATIC1);
	SendMessage(stat,WM_SETFONT,(WPARAM)font,TRUE);
	str.Format(_T("Turns: %d"),m_score);
	SetWindowText(stat,LPCTSTR(str));
	DeleteObject(font);
	type=u_end;
	InvalidateRect(hWnd,NULL,true);
	m_play.Refresh();
	return 0;
}

int Full::End_Draw(HDC hdc)
{
	LOGBRUSH br;
	br.lbColor=RGB(0,0,0);
	br.lbStyle=BS_SOLID;
	HBRUSH brush=CreateBrushIndirect(&br);
	HBRUSH old=(HBRUSH)SelectObject(hdc,brush);
	Rectangle(hdc,0,0,960,704);
	SelectObject(hdc,old);
	DeleteObject(brush);
	return 0;
}

int Full::Draw(HDC hdc)
{
	switch (type)
	{
	case u_start:
		m_play.Start_Print(hdc);
		type=u_game;
		break;
	case u_game:
		m_play.Draw(hdc);
		break;
	case u_end:
		End_Draw(hdc);
		type=u_none;
		break;
	}
	return 0;
}

int Full::Update()
{
	if (!m_play.m_end)
	{
		m_play.Update();
		if (m_play.m_end)
		{
			End_Game();
		}
	}
	return 0;
}