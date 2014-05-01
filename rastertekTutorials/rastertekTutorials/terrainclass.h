#pragma once
class terrain
{
public:
	terrain(int type, int cost, bool passable);
	~terrain();
	
	int movement_cost();
	bool check_passable();
	int get_type();

private:
	int m_type;
	int m_cost;
	bool m_passable;
};

