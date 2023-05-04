#include <fstream>
#include "DxLib.h"
#include "../h/input_networkClient.h"
#include "../h/configLoader.h"

using namespace std;

int input_networkClient::GH_connection[4] = { -1 };

input_networkClient::input_networkClient(int id) :input_network(DX_INPUT_KEY_PAD1, id)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	// servaddrの中身を確実に消す。
	SecureZeroMemory((char*)&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PortNo);

	char ipStr_tmp[128];
	fstream fin(FILEPATH_IPV4, ios::binary || ios::in);

	if (fin)
	{
		int idx = 0;
		for (int i = 0; i < IPV4_DIGITS * IPV4_SEPARATE; i++)
		{
			if (fin.eof() || idx >= 128)
			{
				break;
			}
			if (i > 0 && i % IPV4_DIGITS == 0)
			{
				ipStr_tmp[idx++] = '.';
			}
			int n = 0;
			fin >> n;
			ipStr_tmp[idx++] = '0' + n;
		}
		ipStr_tmp[idx] = '\0';
	}
	string tmpStr(ipStr_tmp);
	IP_str = tmpStr;
	servaddr.sin_addr.s_addr = inet_addr(ipStr_tmp);
	servaddr.sin_family = AF_INET;

	if (GH_connection[0] == -1)
	{
		LoadDivGraph("textures/connection.png", 4, 4, 1, 80, 80, GH_connection);
	}
}
input_networkClient::~input_networkClient()
{
}

void input_networkClient::Update()
{
	input_key::Update();

	pdu[PDU_Direction] = getDirection();

	pdu[PDU_Button_A] =	getButton(actButton_A);
	pdu[PDU_Button_A_Edge] = getButtonEdge(actButton_A);

	pdu[PDU_Button_B] = getButton(actButton_B);
	pdu[PDU_Button_B_Edge] = getButtonEdge(actButton_B);

	pdu[PDU_Button_X] = getButton(actButton_R);
	pdu[PDU_Button_X_Edge] = getButtonEdge(actButton_R);

	pdu[PDU_Button_L] = getButton(actButton_L);
	pdu[PDU_Button_L_Edge] = getButtonEdge(actButton_L);

	pdu[PDU_Button_P] = getButton(actButton_P);
	pdu[PDU_Button_P_Edge] = getButtonEdge(actButton_P);

	connection = false;

	if (checkDataExistence())
	{
		if (frameCnt % 4 == 0)
		{
			connectionIdx = (++connectionIdx) % 4;
		}
		connection = (sendto(sock, pdu, sizeof(pdu), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) > 0);
	}
}
void input_networkClient::Draw()
{
	if (connection)
	{
		string str = "接続先IPアドレス：" + IP_str;
		DrawFormatString(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.8, GetColor(0, 0, 0), str.c_str());
		DrawGraph(WINDOW_WIDTH * 0.9, WINDOW_HEIGHT * 0.7, GH_connection[connectionIdx], TRUE);
	}
}

bool input_networkClient::checkDataExistence()
{
	if (pdu[PDU_Direction] != Direction_None)
	{
		return true;
	}
	for (int i = 0; i < PDU_MAX; i++)
	{
		if (i != PDU_Direction)
		{
			if (pdu[i])
			{
				return true;
			}
		}
	}
	return false;
}