#pragma once

class effect_fade
{
public:
	effect_fade			(bool out = true, int n = 1);
	~effect_fade		();
	int		Update		();
	void	Draw		();

private:
	int		rate = 0;
	int		sign = 1;

};