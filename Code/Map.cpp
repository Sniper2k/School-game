#include "StdAfx.h"
#include "Map.h"


Map::Map(void)
{
	CAtlString str;
	for (int i=0;i<5;i++)
	{
		str.Format(_T("Resourses\\%d.bmp"),i);
		m_icons.push_back((HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,64,64,LR_LOADFROMFILE));
	}

	m_map.resize(10);
	for (size_t i=0;i<m_map.size();i++)
		m_map[i].resize(10);

	Refresh();
}


Map::~Map(void)
{
}


int Map::Refresh()
{
	srand( (unsigned)time( NULL ) );
	int k=rand()%30;
	for (int i=0;i<k;i++)
	{
	}

	int o=rand()%90;

	for (size_t i=0;i<m_map.size();i++)
	{
		for (size_t j=0;j<m_map[i].size();j++)
		{
			int x=(rand()%90+o)%5;
			m_map[i][j].armor=2*x;
			m_map[i][j].icon=&(m_icons[x]);
			m_map[i][j].type=Cell::c_none;

//			лужа - защита 0
// 			яма - 1
// 			поле - 2
// 			куст - 3
// 			камень -4
		}
	}
	return 0;
}