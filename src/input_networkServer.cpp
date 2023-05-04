#include <winsock2.h>
#include <ws2tcpip.h>
#include "DxLib.h"
#include "../h/input_networkServer.h"

int input_networkServer::GH_buttonTest = -1;
int input_networkServer::GH_buttonTest_arrows[Direction_MAX] = { -1 };
int input_networkServer::GH_buttonTest_buttons[3] = { -1 };

input_networkServer::input_networkServer(int id) :input_network(-3, id)
{
	if (GH_buttonTest == -1)
	{
		GH_buttonTest = LoadGraph("textures/buttonTest/bTest.png");

		for (int d = 0; d < Direction_MAX; d++)
		{
			string path = "textures/buttonTest/bTestArrow_" + to_string(d) + ".png";
			GH_buttonTest_arrows[d] = LoadGraph(path.c_str());
		}
		for (int b = 0; b < 3; b++)
		{
			string path = "textures/buttonTest/bTest_" + to_string(b) + ".png";
			GH_buttonTest_buttons[b] = LoadGraph(path.c_str());
		}
	}
	// INADDR_ANY はすべてのクライアントからの入力
	servaddr.sin_addr.s_addr = INADDR_ANY;

	// ソケットに名前を付ける（バインド）
	if (bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		// bind失敗
		exit(-1);
	}
	// ここで、ノンブロッキング(FIONIO)に設定しています。
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

void input_networkServer::Update()
{
	// 受信
	int rv = recv(sock, pdu, sizeof(pdu), 0);
	if (rv <= 0)
	{
		// データを受信できなかったら空の値をセット
		for (int i = 0; i < PDU_MAX; i++)
		{
			pdu[i] = false;
		}
		pdu[PDU_Direction] = Direction_None;
	}
	direction = pdu[PDU_Direction];

	int button_id = getConfigActButton(actButton_A);
	if (button_id >= 0)
	{
		buttons[button_id].input = (bool)pdu[PDU_Button_A];
		buttons[button_id].edgeInput = (bool)pdu[PDU_Button_A_Edge];
	}
	button_id = getConfigActButton(actButton_B);
	if (button_id >= 0)
	{
		buttons[button_id].input = (bool)pdu[PDU_Button_B];
		buttons[button_id].edgeInput = (bool)pdu[PDU_Button_B_Edge];
	}
	button_id = getConfigActButton(actButton_R);
	if (button_id >= 0)
	{
		buttons[button_id].input = (bool)pdu[PDU_Button_X];
		buttons[button_id].edgeInput = (bool)pdu[PDU_Button_X_Edge];
	}
	button_id = getConfigActButton(actButton_L);
	if (button_id >= 0)
	{
		buttons[button_id].input = (bool)pdu[PDU_Button_L];
		buttons[button_id].edgeInput = (bool)pdu[PDU_Button_L_Edge];
	}
	button_id = getConfigActButton(actButton_P);
	if (button_id >= 0)
	{
		buttons[button_id].input = (bool)pdu[PDU_Button_P];
		buttons[button_id].edgeInput = (bool)pdu[PDU_Button_P_Edge];
	}
}
void input_networkServer::Draw()
{
	int dx = WINDOW_WIDTH * 0.4;
	int dy = WINDOW_HEIGHT * 0.4;

	DrawGraph(dx, dy, GH_buttonTest, TRUE);

	if (buttons[actButton_A].input)
	{
		DrawGraph(dx, dy, GH_buttonTest_buttons[0], TRUE);
	}
	if (buttons[actButton_B].input)
	{
		DrawGraph(dx, dy, GH_buttonTest_buttons[1], TRUE);
	}
	if (buttons[actButton_R].input)
	{
		DrawGraph(dx, dy, GH_buttonTest_buttons[2], TRUE);
	}
	if (direction != Direction_None)
	{
		DrawGraph(dx, dy, GH_buttonTest_arrows[direction], TRUE);
	}
}