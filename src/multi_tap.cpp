#include "../h/multi_tap.h"

multi_tap::multi_tap()
{
	GH_multiTap = new graphicParam[multiTap_MAX];

	mBoard = new boardMenu(3, 4, multiTap_MAX, multiTap_None);
	mBoard->setGraphics(GH_multiTap, multiTap_MAX);
}
multi_tap::~multi_tap()
{
	if (mBoard != nullptr)
	{
		delete mBoard;
	}
}
void multi_tap::Update()
{
	if (mBoard != nullptr)
	{
		mBoard->Update();
	}
}
void multi_tap::Draw()
{
	if (mBoard != nullptr)
	{
		mBoard->Draw(0, 0);
	}
}
