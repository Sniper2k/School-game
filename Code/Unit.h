#pragma once
#include "Map.h"
#include "stdafx.h"

class Unit 
{
public:
	enum U_Type{u_archer,u_scout,u_warrior};

	int m_health;
	int m_speed;
	bool m_range;
	int m_at_power;
	POINT m_cor;
	U_Type m_type;
	POINT m_icon_id;

	int Refresh();
	int Turn(std::vector<std::vector<Map::Cell> >* map);
	bool Under_Attack(POINT* cell);
	bool In_Step(POINT* cell);
	int Step(POINT* cell, Map* map, Map::Cell::C_type type);
	bool In_Length(POINT* cell,int length);
	bool Is_Dead();
	int Set_Damage(int damage, int armor);

	Unit(U_Type type,bool num);
	Unit(void);
	~Unit(void);
};

