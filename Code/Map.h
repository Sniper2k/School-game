#pragma once
class Map
{
public:
	struct Cell
	{
		enum C_type{c_none,c_blue,c_red};
		Cell():icon(NULL),armor(0),type(c_none){}
		HBITMAP* icon;
		int armor;
		C_type type;
	};

	std::vector<std::vector<Cell> > m_map;
	std::vector<HBITMAP> m_icons;

	int Refresh();
	Map(void);
	~Map(void);
};

