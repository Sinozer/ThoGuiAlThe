#pragma once

/* ##################  WINSOCK  ############### */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>

#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)
#define MSG_WEB (WM_USER + 3)
/* ##################  WINSOCK  ############### */

/* #################  UTILITY  ################ */
#define DELPTR(ptr) if (ptr) { delete (ptr); (ptr) = nullptr; }
#define NULLPTR(ptr) if (ptr) { (ptr) = nullptr; }
#define I(x) x::GetInstance()
/* #################  UTILITY  ################ */

/* #################  JSON	################ */
#include "External/JSON/json.hpp"
/* #################  JSON	################ */

/* #################  TYPEDEF  ################ */
typedef uint32_t TGATPLAYERID;
typedef uint64_t TGATBODYSIZE;
typedef uint64_t TGATDATASIZE;

enum class TgatNetworkError
{
	SEND_ERROR = 0,
	RECEIVE_ERROR = 1,
	HEADER_ERROR = 2
};

enum class TgatServerMessage
{
	PLAYER_INIT = 0, // {"eventType": "PLAYER_INIT", "Player" : "playerId": uuid(0)}
	PLAYER_DISCONNECT = 1, // {"eventType": "PLAYER_DISCONNECT", "Player" : "playerId": uuid(0)}
	PLAYER_INPUT = 2, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0, "p": 0}
	PLAYER_WIN = 3, // {"eventType": "PLAYER_WIN", "Player" : "playerId": uuid(0)}
};

enum class TgatClientMessage
{
	PLAYER_INPUT = 0, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0}
};
/* #################  TYPEDEF  ################ */

/* #################  DEBUG  ################## */
#include <iostream>
#include <assert.h>
#if defined(_DEBUG) || defined(DEBUG) && !defined(LOG)
#define LOG(x) std::cout << x << '\n'
#else
#define LOG(x)
#endif
/* #################  DEBUG  ################## */