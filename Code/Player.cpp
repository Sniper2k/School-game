#include "StdAfx.h"
#include "Player.h"


Player::Player(T_type type,Map* map)
{
	Refresh(type,map);
}

Player::Player()
{
}


Player::~Player(void)
{
}

int Player::End_of_Turn()
{
	Unit temp=m_team.front();
	for (size_t i=1;i<m_team.size();i++)
	{
		m_team[i-1]=m_team[i];
	}
	m_team.back()=temp;
	m_team.back().Refresh();
	return 0;
}

bool Player::Is_End_of_Turn()
{
	if (m_team.front().m_speed==0)
		return true;
	else return false;
}

size_t Player::Find_Unit(POINT* cell)
{
	size_t k=10;
	for (size_t i=0;i<m_team.size();i++)
	{
		if (m_team[i].m_cor.x==cell->x && m_team[i].m_cor.y==cell->y)
			k=i;
	}
	return k;
}

bool Player::Unit_is_Dead(size_t pos)
{
	if (m_team[pos].Is_Dead())
	{
		m_team.erase(m_team.begin()+pos);
		return true;
	}
	else return false;
}


struct Ref 
{

	Ref():num(0){};
	int num;
	Unit::U_Type type;
};

int Player::Set_Unit(Map* map,bool num,int x)
{
	srand( (unsigned)time( NULL ) );
	int a,b;
	Map::Cell::C_type type;
	if (!num)
	{
		a=0;
		b=1;
		type=Map::Cell::c_blue;
	}
	else
	{
		a=9;
		b=-1;
		type=Map::Cell::c_red;
	}
	std::vector<Ref> amount;
	amount.resize(3);

	amount[0].type=Unit::u_archer;
	amount[1].type=Unit::u_warrior;
	amount[2].type=Unit::u_scout;

	int k=0;
	
	for (size_t i=0;i<9;i++)
	{
		size_t pos=((size_t)rand()+ x) % (3-k) ;
		m_team[i].m_type=amount[pos].type;

		switch (amount[pos].type)
		{
		case Unit::u_archer:
			m_team[i]=Unit(amount[pos].type,num);
			m_team[i].m_cor.x=4*amount[pos].num;
			m_team[i].m_cor.y=a;
			map->m_map[m_team[i].m_cor.x][m_team[i].m_cor.y].type=type;
			break;
		case Unit::u_warrior:
			m_team[i]=Unit(amount[pos].type,num);
			m_team[i].m_cor.x=4*amount[pos].num;
			m_team[i].m_cor.y=a+b;
			map->m_map[m_team[i].m_cor.x][m_team[i].m_cor.y].type=type;
			break;
		case Unit::u_scout:
			m_team[i]=Unit(amount[pos].type,num);
			m_team[i].m_cor.x=4*amount[pos].num+1;
			m_team[i].m_cor.y=a;
			map->m_map[m_team[i].m_cor.x][m_team[i].m_cor.y].type=type;
			break;
		}
		amount[pos].num++;
		if (amount[pos].num>2)
		{
			Ref temp=amount[pos];
			amount[pos]=amount[amount.size()-1-k];
			amount[amount.size()-1-k]=temp;
			k++;
		}
	}
	return 0;
}

int Player::Refresh(T_type type, Map* map)
{
	srand( (unsigned)time( NULL ) );
	m_team.resize(9);
	int k=1;
	bool b;
	if (type==t_blue)
	{
	    int x=rand()%14;
		m_type=Map::Cell::c_blue;
		b=false;
		Set_Unit(map,b,x);
	}
	else 
	{
		int  x = rand()%138;
		m_type=Map::Cell::c_red;
		b=true;
		Set_Unit(map,b,x);
	}
	return (0);
}
