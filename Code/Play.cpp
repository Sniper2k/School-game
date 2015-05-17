#include "StdAfx.h"
#include "Play.h"



Play::Play(void):
m_end(true),m_red(Player()),
m_blue(Player()),
m_static(2),m_button(3),m_light(false),
m_timer_light(0)
{
	m_dmg_icon=(HBITMAP)LoadImage(NULL,_T("Resourses\\dmg.bmp"),IMAGE_BITMAP,64,64,LR_LOADFROMFILE);
	m_green_rect=(HBITMAP)LoadImage(NULL,_T("Resourses\\green_rect.bmp"),IMAGE_BITMAP,64,64,LR_LOADFROMFILE);
	m_red_rect=(HBITMAP)LoadImage(NULL,_T("Resourses\\red_rect.bmp"),IMAGE_BITMAP,64,64,LR_LOADFROMFILE);
	m_unit_icon.resize(2);
	CAtlString str;
	for (int i=0;i<3;i++)
	{
		str.Format(_T("Resourses\\red_%d.bmp"),i+1);
		m_unit_icon.front().push_back((HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,64,64,LR_LOADFROMFILE));
		str.Format(_T("Resourses\\blue_%d.bmp"),i+1);
		m_unit_icon.back().push_back((HBITMAP)LoadImage(NULL,LPCTSTR(str),IMAGE_BITMAP,64,64,LR_LOADFROMFILE));
	}

	Refresh();
}

Play::~Play(void)
{
}

int Play::Game()
{
	return 0;
}

int Play::End_Turn()
{
	m_turn->End_of_Turn();
	switch (m_turn->m_type)
	{
	case Map::Cell::c_blue:
		m_turn=&m_red;
		m_non_turn=&m_blue;
		break;
	case Map::Cell::c_red:
		m_turn=&m_blue;
		m_non_turn=&m_red;
		break;
	}
	m_turn_ended=true;
	m_turns++;
	return 0;
}

int Play::Set_Static_Text()
{
	HFONT font = CreateFont(36,0,0,0,                         
		FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,_T("Old English Text MT"));

	

	CAtlString str,a,b,c,d;

	if (m_turn->m_type==Map::Cell::c_blue)
		a = _T("Blue");
	else a = _T("Red");

	Map::Cell* cell= &(m_map.m_map[m_mouse.m_rect_now.x][m_mouse.m_rect_now.y]);
	switch (cell->armor)
	{
	case 0:
		b= _T("Poll");
		break;
	case 2:
		b= _T("Stump");
		break;
	case 4:
		b= _T("Lawn");
		break;
	case 6:
		b= _T("Bush");
		break;
	case 8:
		b= _T("Stone");
		break;
	}

	if (cell->type!=Map::Cell::c_none)
	{
		Unit* unit;
		POINT cor;
		cor.x=m_mouse.m_rect_now.x;
		cor.y=m_mouse.m_rect_now.y;

		if (cell->type==Map::Cell::c_red)
		{
			size_t pos=m_red.Find_Unit(&cor);
			c= _T("Red");
			unit=&(m_red.m_team[pos]);
		}
		else 
		{
			size_t pos=m_blue.Find_Unit(&cor);
			c= _T("Blue");
			unit=&(m_blue.m_team[pos]);
		}
		
		switch(unit->m_type)
		{
		case Unit::u_archer:
			d=_T("archer");
			break;
		case Unit::u_warrior:
			d=_T("warrior");
			break;
		case Unit::u_scout:
			d=_T("scout");
			break;
		}
		str.Format(_T("%s turn\nLandtype: %s\nArmor: %d\n%s %s\nHealth: %d\nSpeed: %d\nAttack Power: %d"),
			LPCTSTR(a),LPCTSTR(b),cell->armor,LPCTSTR(c),LPCTSTR(d),unit->m_health,unit->m_speed,unit->m_at_power);
		HWND stat=GetDlgItem(hWnd,m_static);
		SendMessage(stat,WM_SETFONT,(WPARAM)font,TRUE);
		SetWindowText(stat,LPCTSTR(str));
	}
	else
	{
		str.Format(_T("%s turn\nLandtype: %s\nArmor: %d\nFree"),LPCTSTR(a),LPCTSTR(b),cell->armor);
		HWND stat=GetDlgItem(hWnd,m_static);
		SendMessage(stat,WM_SETFONT,(WPARAM)font,TRUE);
		SetWindowText(stat,LPCTSTR(str));
	}
	DeleteObject(font);
	return 0;
}

bool Play::Update()
{
	if (m_end_turn_button && !m_end)
	{
		End_Turn();
		m_end_turn_button=false;
	}
	else if (m_end_turn_button && m_end)
		return true;

	if (m_mouse.m_click)
	{
		if (m_map.m_map[m_mouse.m_rect_now.x][m_mouse.m_rect_now.y].type==Map::Cell::c_none && 
			m_turn->m_team.front().In_Step(&(m_mouse.m_rect_now)))
		{
			RECT rc;
			rc.left=m_turn->m_team.front().m_cor.x;
			rc.top=m_turn->m_team.front().m_cor.y;
			rc.right=m_turn->m_team.front().m_cor.x+1;
			rc.bottom=m_turn->m_team.front().m_cor.y+1;
			m_update_zone.push_back(rc);

			m_turn->m_team.front().Step(&(m_mouse.m_rect_now),&m_map,m_turn->m_type);
			//Here SetWindowText
			rc.left=m_turn->m_team.front().m_cor.x;
			rc.top=m_turn->m_team.front().m_cor.y;
			rc.right=m_turn->m_team.front().m_cor.x+1;
			rc.bottom=m_turn->m_team.front().m_cor.y+1;
			m_update_zone.push_back(rc);
		}

		if (m_map.m_map[m_mouse.m_rect_now.x][m_mouse.m_rect_now.y].type==m_non_turn->m_type &&
			m_turn->m_team.front().Under_Attack(&(m_mouse.m_rect_now)))
		{
			size_t unit=m_non_turn->Find_Unit(&(m_mouse.m_rect_now));
			POINT cor=m_non_turn->m_team[unit].m_cor;
			m_non_turn->m_team[unit].Set_Damage(m_turn->m_team.front().m_at_power,m_map.m_map[cor.x][cor.y].armor);
			m_turn->m_team.front().m_speed=0;
			//Here SetWindowText

			m_damaged.push_back(cor);

			
			if (m_non_turn->Unit_is_Dead(unit))
			{
				m_map.m_map[cor.x][cor.y].type=Map::Cell::c_none;
				//Here SetWindowText
				if (m_non_turn->m_team.empty())
				{
					m_end=true;
					m_mouse.m_end=true;
					return false;
				}
			}
			m_turn_ended=true;
		}


		if (m_turn->Is_End_of_Turn())
		{
			End_Turn();
		}
		m_mouse.m_click=false;

		Set_Static_Text();
	}

	if ((m_mouse.m_cor.x<640 && m_mouse.m_cor.y<640) || m_turn_ended)
	{
		POINT cor;
		cor.x=m_mouse.m_cor.x/64;
		cor.y=m_mouse.m_cor.y/64;
		if (m_mouse.m_rect_now.x!=cor.x || m_mouse.m_rect_now.y!=cor.y || m_turn_ended)
		{
			if (m_mouse.m_cor.x<640 && m_mouse.m_cor.y<640)
			{
				RECT rc;
				rc.left=m_mouse.m_rect_now.x;
				rc.top=m_mouse.m_rect_now.y;
				rc.right=m_mouse.m_rect_now.x+1;
				rc.bottom=m_mouse.m_rect_now.y+1;

				m_update_zone.push_back(rc);

				m_mouse.m_rect_now=cor;

				rc.left=m_mouse.m_rect_now.x;
				rc.top=m_mouse.m_rect_now.y;
				rc.right=m_mouse.m_rect_now.x+1;
				rc.bottom=m_mouse.m_rect_now.y+1;

				m_update_zone.push_back(rc);
			}

			

			bool a= m_turn->m_team.front().Under_Attack(&(m_mouse.m_rect_now));
			bool b= m_turn->m_team.front().In_Step(&(m_mouse.m_rect_now));
			bool c = m_map.m_map[m_mouse.m_rect_now.x][m_mouse.m_rect_now.y].type==Map::Cell::c_none;
			bool d = m_map.m_map[m_mouse.m_rect_now.x][m_mouse.m_rect_now.y].type==m_non_turn->m_type;
			if (( c && b )|| ( d && a ) )
				m_mouse.m_colour=true;
			else m_mouse.m_colour=false;

			Set_Static_Text();
		}

		if (m_turn_ended && !m_end)
		{
			RECT rc;
			rc.left=m_non_turn->m_team.back().m_cor.x;
			rc.top=m_non_turn->m_team.back().m_cor.y;
			rc.right=m_non_turn->m_team.back().m_cor.x+1;
			rc.bottom=m_non_turn->m_team.back().m_cor.y+1;
			m_update_zone.push_back(rc);
			rc.left=m_turn->m_team.front().m_cor.x;
			rc.top=m_turn->m_team.front().m_cor.y;
			rc.right=m_turn->m_team.front().m_cor.x+1;
			rc.bottom=m_turn->m_team.front().m_cor.y+1;
			m_update_zone.push_back(rc);

			m_turn_ended=false;
			Set_Static_Text();
		}
	}

	RECT rc;
	for (size_t i=0;i<m_damaged.size();i++)
	{
		rc.left=m_damaged[i].cor.x;
		rc.top=m_damaged[i].cor.y;
		rc.right=m_damaged[i].cor.x+1;
		rc.bottom=m_damaged[i].cor.y+1;
		m_update_zone.push_back(rc);

	}

	m_timer_light+=1;
	if (m_timer_light==10)
	{
		m_light=true;
		m_timer_light=0;	
	}
	if (m_timer_light%5==0)
	{
		rc.left=m_turn->m_team.front().m_cor.x;
		rc.top=m_turn->m_team.front().m_cor.y;
		rc.right=m_turn->m_team.front().m_cor.x+1;
		rc.bottom=m_turn->m_team.front().m_cor.y+1;
		m_update_zone.push_back(rc);
	}

	for (size_t i=0;i<m_update_zone.size();i++)
	{
		RECT rc;
		rc.bottom=m_update_zone[i].bottom*64;
		rc.left=m_update_zone[i].left*64;
		rc.right=m_update_zone[i].right*64;
		rc.top=m_update_zone[i].top*64;
		InvalidateRect(hWnd,&rc,NULL);
	}

	return false;
}

int Play::Draw(HDC hdc)
{
	HDC tdc=CreateCompatibleDC(hdc);
	COLORREF rgb = RGB(255, 255, 255);
	COLORREF color_old = SetBkColor(tdc,rgb);

	while (!m_update_zone.empty())
	{
		HBITMAP old=(HBITMAP)SelectObject(tdc,*(m_map.m_map[m_update_zone.back().left][m_update_zone.back().top].icon));
		
		TransparentBlt(hdc,m_update_zone.back().left*64,m_update_zone.back().top*64,64,64,tdc,0,0,64,64,rgb);

		SelectObject(tdc,old);

		

		if (m_map.m_map[m_update_zone.back().left][m_update_zone.back().top].type!=Map::Cell::c_none)
		{
			HBITMAP old;
			size_t unit;
			POINT cor;
			cor.x=m_update_zone.back().left;
			cor.y=m_update_zone.back().top;

			if (m_map.m_map[m_update_zone.back().left][m_update_zone.back().top].type==Map::Cell::c_red)
			{
				unit=m_red.Find_Unit(&cor);
				old=(HBITMAP)SelectObject(tdc,m_unit_icon[m_red.m_team[unit].m_icon_id.x][m_red.m_team[unit].m_icon_id.y]);
			}
			else
			{
				unit=m_blue.Find_Unit(&cor);
				old=(HBITMAP)SelectObject(tdc,m_unit_icon[m_blue.m_team[unit].m_icon_id.x][m_blue.m_team[unit].m_icon_id.y]);
			}
			TransparentBlt(hdc,m_update_zone.back().left*64,m_update_zone.back().top*64,64,64,tdc,0,0,64,64,rgb);
			SelectObject(tdc,old);
		}

		if (m_update_zone.back().left==m_mouse.m_rect_now.x && m_update_zone.back().top==m_mouse.m_rect_now.y)
		{
			HBITMAP old;
			if (m_mouse.m_colour)
				old=(HBITMAP)SelectObject(tdc,m_green_rect);
			else 
				old=(HBITMAP)SelectObject(tdc,m_red_rect);

			TransparentBlt(hdc,m_update_zone.back().left*64,m_update_zone.back().top*64,64,64,tdc,0,0,64,64,rgb);

			SelectObject(tdc,old);
		}

		if (m_update_zone.back().left==m_turn->m_team.front().m_cor.x && m_update_zone.back().top==m_turn->m_team.front().m_cor.y )
		{
			if (m_light)
			{
				HBITMAP old=(HBITMAP)SelectObject(tdc,m_green_rect);
				TransparentBlt(hdc,m_turn->m_team.front().m_cor.x*64,m_turn->m_team.front().m_cor.y*64,64,64,tdc,0,0,64,64,rgb);
				SelectObject(tdc,old);
				m_light=false;
			}
			
		}
		m_update_zone.pop_back();
	}
	
	int size = (int) m_damaged.size();
	for (int i=0;i<size;i++)
	{
		if (m_damaged[i].status)
		{
			HBITMAP old=(HBITMAP)SelectObject(tdc,m_dmg_icon);

			TransparentBlt(hdc,m_damaged[i].cor.x*64,m_damaged[i].cor.y*64,64,64,tdc,0,0,64,64,rgb);

			SelectObject(tdc,old);
			m_damaged[i].status=false;
		}
		else 
		{
			m_damaged.erase(m_damaged.begin()+i);
			i--;
			size--;
		}
	}

	SetBkColor(tdc,color_old);
	DeleteDC(tdc);
	return 0;
}

int Play::Start()
{
	CreateWindow(_T("STATIC"),NULL,WS_BORDER|WS_CHILD|WS_VISIBLE|SS_CENTER,
		680,20,260,270,hWnd,(HMENU)m_static,NULL,NULL);
	CreateWindow(_T("BUTTON"),_T("Next Turn"),WS_BORDER|WS_CHILD|WS_VISIBLE,
		680,580,260,60,hWnd,(HMENU)m_button,NULL,NULL);
	m_end=false;
	m_mouse.m_end=false;
	return 0;
}

int Play::Refresh()
{
	m_end=true;
	m_map.Refresh();
	m_red.Refresh(Player::t_red,&m_map);
	m_blue.Refresh(Player::t_blue, &m_map); 
	m_turn_ended=true;
	m_turn=&m_red;
	m_non_turn=&m_blue;
	m_end_turn_button=false;
	m_turn_ended=true;
	m_last_action=_T("");
	m_turns=0;
	
	m_mouse=Mouse();
	
	m_update_zone.resize(0);
	while(m_damaged.size()!=0)
		m_damaged.pop_back();

	return 0;
}

int Play::Start_Print(HDC hdc)
{
	HDC tdc = CreateCompatibleDC(hdc);
	COLORREF rgb = RGB(255, 255, 255);
	COLORREF color_old = SetBkColor(tdc,rgb);
	HBITMAP bm=(HBITMAP)LoadImage(NULL,_T("Resourses\\bg.bmp"),IMAGE_BITMAP,960,704,LR_LOADFROMFILE);
	HBITMAP old=(HBITMAP)SelectObject(tdc,bm);

	TransparentBlt(hdc,0,0,960,704,tdc,0,0,960,704,rgb);

	SelectObject(tdc,old);

	for (size_t i=0;i<m_map.m_map.size();i++)
	{
		for (size_t j=0;j<m_map.m_map[i].size();j++)
		{
			HBITMAP old=(HBITMAP)SelectObject(tdc,*(m_map.m_map[i][j].icon));

			TransparentBlt(hdc,i*64,j*64,64,64,tdc,0,0,64,64,rgb);

			SelectObject(tdc,old);
		}
	}

	std::vector<Player*> teams;
	teams.push_back(&m_blue);
	teams.push_back(&m_red);
	for (size_t i=0;i<teams.size();i++)
	{
		for (size_t j=0;j<teams[i]->m_team.size();j++)
		{
			HBITMAP old=(HBITMAP)SelectObject(tdc,m_unit_icon[teams[i]->m_team[j].m_icon_id.x][teams[i]->m_team[j].m_icon_id.y]);

			TransparentBlt(hdc,teams[i]->m_team[j].m_cor.x*64,teams[i]->m_team[j].m_cor.y*64,64,64,tdc,0,0,64,64,rgb);

			SelectObject(tdc,old);
		}
	}
	SetBkColor(tdc,color_old);
	DeleteDC(tdc);
	return 0;
}