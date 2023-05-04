#pragma once
#pragma comment(lib,"Ws2_32.lib")

// https://mntmanblog.com/win32_lean_and_mean/
#define WIN32_LEAN_AND_MEAN
#define BUFF_SIZE				(16)
#define RESPONSE				("Active")

#define IPV4_SEPARATE			(4)
#define IPV4_DIGITS				(3)

#define FILEPATH_IPV4			("bin/IPv4.bin")
#define FILEPATH_PORTNUMBER		("bin/PortNumber.bin")

enum
{
	PDU_Direction,

	PDU_Button_A_Edge,
	PDU_Button_B_Edge,
	PDU_Button_X_Edge,
	PDU_Button_L_Edge,
	PDU_Button_P_Edge,

	PDU_Button_A,
	PDU_Button_B,
	PDU_Button_X,
	PDU_Button_L,
	PDU_Button_P,

	PDU_MAX,
};
