#pragma once
#include "stdafx.h"
#include "Unit.h"
#include "Map.h"

class Player
{
public:
	enum T_type{t_blue,t_red};
	std::vector<Unit> m_team;
	Map::Cell::C_type m_type;

	size_t Find_Unit(POINT* cell);
	bool Unit_is_Dead(size_t pos);
	bool Is_End_of_Turn();
	int End_of_Turn();
	int Refresh(T_type type,Map* map);
	int Set_Unit(Map* map, bool num,int x);
public:
	Player(T_type type, Map* map);
	Player();
	~Player(void);
};

