#include <fstream>
#include "DxLib.h"
#include "../h/input_network.h"

input_network::input_network(int code, int id) :input_key(code, id)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	// servaddr‚Ì’†g‚ðŠmŽÀ‚ÉÁ‚·B
	SecureZeroMemory((char*)&servaddr, sizeof(servaddr));

	fstream fin(FILEPATH_PORTNUMBER);
	if (fin)
	{
		fin >> PortNo;
		fin.close();
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PortNo);
}
input_network::~input_network()
{
	closesocket(sock);
}

void input_network::Initialize()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
}
void input_network::Finalize()
{
	WSACleanup();
}