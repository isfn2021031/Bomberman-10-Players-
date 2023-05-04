#pragma once

// êF
#define WHITE					255
#define RGB_WHITE				GetColor(WHITE, WHITE, WHITE)

#define BLACK					0
#define RGB_BLACK				GetColor(BLACK, BLACK, BLACK)

#define GRAY					80
#define RGB_GRAY				GetColor(GRAY,GRAY,GRAY)

#define RED_R					200
#define RED_G					60
#define RED_B					60
#define RGB_DARK_RED			GetColor(RED_R / 3, RED_G / 3, RED_B / 3)
#define RGB_RED					GetColor(RED_R, RED_G, RED_B)
#define RGB_BRIGHT_RED			GetColor(255, 200, 200)

#define RGB_BROWN				GetColor(100,50,0)

#define BLUE_R					30
#define BLUE_G					40
#define BLUE_B					240
#define RGB_DARK_BLUE			GetColor(BLUE_R / 3, BLUE_G / 3, BLUE_B / 3)
#define RGB_BLUE				GetColor(BLUE_R, BLUE_G, BLUE_B)
#define RGB_BRIGHT_BLUE			GetColor(200, 200, 255)

#define GREEN_R					0
#define GREEN_G					220
#define GREEN_B					10
#define RGB_DARK_GREEN			GetColor(GREEN_R / 3, GREEN_G / 3, GREEN_B / 3)
#define RGB_GREEN				GetColor(GREEN_R, GREEN_G, GREEN_B)
#define RGB_BRIGHT_GREEN		GetColor(200, 255, 200)


#define YELLOW_R				230
#define YELLOW_G				235
#define YELLOW_B				8
#define RGB_DARK_YELLOW			GetColor(YELLOW_R / 3, YELLOW_G / 3, YELLOW_B / 3)
#define RGB_YELLOW				GetColor(YELLOW_R, YELLOW_G, YELLOW_B)
#define RGB_BRIGHT_YELLOW		GetColor(255, 255, 200)

#define CYAN_R					0
#define CYAN_G					255
#define CYAN_B					255
#define RGB_CYAN				GetColor(CYAN_R, CYAN_G, CYAN_B)

#define ORANGE_R				255
#define ORANGE_G				110
#define ORANGE_B				10
#define RGB_DARK_ORANGE			GetColor(ORANGE_R / 3, ORANGE_G / 3, ORANGE_B / 3)
#define RGB_ORANGE				GetColor(ORANGE_R, ORANGE_G, ORANGE_B)
#define RGB_BRIGHT_ORANGE		GetColor(ORANGE_R, 150, 20)

#define MAGENTA_R				(250)
#define MAGENTA_G				(0)
#define MAGENTA_B				(240)
#define RGB_MAGENTA				GetColor(MAGENTA_R,MAGENTA_G,MAGENTA_B)

#define MUSCAT_R				144
#define MUSCAT_G				255
#define MUSCAT_B				188
#define RGB_DARK_MUSCAT			GetColor(MUSCAT_R / 3, MUSCAT_G / 3, MUSCAT_B / 3)
#define RGB_MUSCAT				GetColor(MUSCAT_R, MUSCAT_G, MUSCAT_B)
#define RGB_BRIGHT_MUSCAT		GetColor(180, 255, 222)

#define PURPLE_R				170
#define PURPLE_G				100
#define PURPLE_B				200
#define RGB_DARK_PURPLE			GetColor(PURPLE_R / 3, PURPLE_G / 3, PURPLE_B / 3)
#define RGB_PURPLE				GetColor(PURPLE_R, PURPLE_G, PURPLE_B)
#define RGB_BRIGHT_PURPLE		GetColor(220, 200, 255)

#define RGB_ICE					GetColor(170,180,255)
#define RGB_SILVER				GetColor(220,225,225)

struct RgbData
{
	int r = 255;
	int g = 255;
	int b = 255;
	unsigned int rgb = 0;
};

bool SetDrawBrightPlayer(int id = -1);
bool SetDrawBrightTeam(int id = -1);