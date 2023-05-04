#pragma once
#include "../h/input_key.h"
#include "../h/network_common.h"

class input_network :public input_key
{
public:
	input_network						(int code, int id = 0);
	~input_network						();
	static void			Initialize		();
	static void			Finalize		();
	
protected:
	int					PortNo = 49513;
	struct sockaddr_in	servaddr;
	SOCKET				sock = -1;
	char				pdu[PDU_MAX] = { 0 };
};