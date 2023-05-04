#pragma once

class parameter
{
public:
	parameter(int max, int min = 0, int _se_plus = -1, int _se_minus = -1);
	void	setValue(int v);
	void	add(int n, bool se = true); 
	int		normalization(int v);
	int		getValue()		{ return value; }
	int		getValueMAX()	{ return value_max; }
	int		getValueMin()	{ return value_min; }
	

private:
	int		value_max = 1;
	int		value_min = 0;
	int		value = 0;
	int		se_plus = -1;
	int		se_minus = -1;
};