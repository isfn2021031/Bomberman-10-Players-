#pragma once
#include "../h/input_key.h"
#include "../h/input_network.h"

class input_networkClient :public input_network
{
public:
	input_networkClient						(int id = 0);
	~input_networkClient					();
	void				Update				()override;
	void				Draw				()override;
	
private:
	bool				connection = false;
	bool				checkDataExistence	();
	int					connectionIdx = 0;
	static int			GH_connection[4];
	string				IP_str = "127.0.0.1";
};