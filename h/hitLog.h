#pragma once
#include <deque>

using namespace std;

class hitLog
{
public:
	hitLog									();
	~hitLog									();

	void						Update		();
	void						Draw		();

	void						Addition	(int* x, int* y);
	void						setPlayerID	(int id)		{ playerID = id; }

private:
	struct message
	{
		int*	x = nullptr;
		int*	y = nullptr;
		int		frameCnt = 0;
		int		opponentID = 0;
	};
	deque<message>				messages;
	int							playerID = -1;
	static int					GH_Hit;
};