#include "StdAfx.h"
#include "Unit.h"



Unit::Unit(U_Type type,bool num):
m_type(type),
m_health(100),m_at_power(10),m_speed(1)
{
	switch(type)
	{
	case u_archer:
		m_range=true;
		m_icon_id.y=2;
		break;
	case u_warrior:
		m_range=false;
		m_icon_id.y=0;
		m_speed+=1;
		m_at_power*=3;
		break;
	case u_scout:
		m_range=false;
		m_icon_id.y=1;
		m_speed+=3;
		break;
	}
	if (!num)
	{
		m_icon_id.x=1;
	}
	else m_icon_id.x=0;
	
}

Unit::Unit(void)
{
}


Unit::~Unit(void)
{
}

int Unit::Refresh()
{
	m_speed=1;

	switch (m_type)
	{
	case u_warrior:
		m_speed+=1;
		break;
	case u_scout:
		m_speed+=3;
		break;
	}

	return 0;
}

bool Unit::Is_Dead()
{
	if (m_health<=0)
		return true;
	else return false;
}

int Unit::Set_Damage(int damage,int armor)
{
	m_health-=(damage-armor);
	return 0;
}

bool Unit::In_Length(POINT* cell,int length)
{
	int a=cell->x-m_cor.x;
	if (a<0) a*=-1;
	int b=cell->y-m_cor.y;
	if (b<0) b*=-1;
	int c=a+b;
	if (length<c)
		return false;
	else return true;
}

bool Unit::In_Step(POINT* cell)
{
	return In_Length(cell,m_speed);
}



int Unit::Step(POINT* cell, Map* map, Map::Cell::C_type type)
{
	map->m_map[cell->x][cell->y].type=type;
	map->m_map[m_cor.x][m_cor.y].type=Map::Cell::c_none;
	int a=cell->x-m_cor.x;
	if (a<0) a*=-1;
	int b=cell->y-m_cor.y;
	if (b<0) b*=-1;
	int c=a+b;
	m_cor=*cell;
	m_speed-=c;
	return 0;
}

bool Unit::Under_Attack(POINT* cell)
{
	if (m_speed!=0)
	{
		if (m_range)
			return true;
		else
		{
			if (In_Length(cell,1))
				return true;
			else return false;
		}
	}
	else return false;
}