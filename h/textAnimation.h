#pragma once
#include <string>
#include "../h/input_key.h"

using namespace std;

class textAnimation
{
public:
	textAnimation								(string str);
	virtual			~textAnimation				();
	virtual bool	Update						();
	virtual void	Draw						(int dx, int dy);
	void			setRate						(int r) { rate_update = r; }
	
private:
	input_key*		kI_ptr = nullptr;
	string			String = "";
	string			String_disp = "";
	int				frameCnt = 0;
	int				rate_update = 1;
	int				idx_str = 0;
};