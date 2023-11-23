#pragma once

/* ##################  WINSOCK  ############### */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>
#include <Synchapi.h>
#include <minwinbase.h>
#define MSG_SERVER (WM_USER + 1)
#define MSG_CLIENT (WM_USER + 2)
#define MSG_WEB (WM_USER + 3)
#define MSG_NUKE (WM_USER + 4)
/* ##################  WINSOCK  ############### */

/* #################  UTILITY  ################ */
#define DELPTR(ptr) if (ptr) { delete (ptr); (ptr) = nullptr; }
#define NULLPTR(ptr) if (ptr) { (ptr) = nullptr; }
#define I(x) x::GetInstance()
/* #################  UTILITY  ################ */

/* #################  JSON	################ */
#include "External/JSON/json.hpp"

static const std::string JSON_EVENT_TYPE = "eventType";

static const std::string JSON_PLAYER_MOVE = "move";

static const std::string JSON_PLAYER_ID = "player";
static const std::string JSON_PLAYER_NAME = "name";
static const std::string JSON_PLAYER_PPP = "profilePicturePath";
static const std::string JSON_PLAYER_PPTP = "profilePictureThumbPath";
static const std::string JSON_PLAYER_COLOR = "color";

static const std::string JSON_SESSION_ID = "session";
static const std::string JSON_ERROR = "error";

#define PLAYER_MOVE(x, y) {{"x", x}, {"y", y}}
#define PLAYER_MOVE_ARG_X(json) json["x"].get<int>()
#define PLAYER_MOVE_ARG_Y(json) json["y"].get<int>()
#define PLAYER_MOVE_ARGS(json) PLAYER_MOVE_ARG_X(json), PLAYER_MOVE_ARG_Y(json)

#define PLAYER_DD(name, profilePicturePath, color) {{JSON_PLAYER_NAME, name}, {JSON_PLAYER_PPP, profilePicturePath}, {JSON_PLAYER_COLOR, color}}
#define PLAYER_DD_ARG_NAME(json) json[JSON_PLAYER_NAME].get<std::string>()
#define PLAYER_DD_ARG_PPP(json) json[JSON_PLAYER_PPP].get<std::string>()
#define PLAYER_DD_ARG_PPTP(json) json[JSON_PLAYER_PPTP].get<std::string>()
#define PLAYER_DD_ARG_COLOR(json) json[JSON_PLAYER_COLOR].get<std::array<uint32_t, 4>>()
#define PLAYER_DD_ARGS(json) PLAYER_DISPLAY_DATA_ARG_NAME(json), PLAYER_DISPLAY_DATA_ARG_PPP(json), PLAYER_DISPLAY_DATA_ARG_COLOR(json)
/* #################  JSON	################ */

/* #################  TYPEDEF  ################ */
typedef uint32_t TGATPLAYERID;
typedef uint32_t TGATSESSIONID;
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
	GAME_END = 3, // {"eventType": "GAME_END", "Player" : "playerId": uuid(0)}
	SESSION_CREATED = 4, // {"eventType": "SESSION_CREATED", "Session" : "sessionId": uuid(0)}
	SESSION_JOINED = 5, // {"eventType": "SESSION_JOINED", "Session" : "sessionId": uuid(0)}

	PLAYER_INFO_CHANGED = 6, // {"eventType": "PLAYER_INFO_CHANGED", "Player" : {"name": "name", "profilePicturePath": "path", "color": [0, 0, 0, 0]}

	GAME_REPLAY = 7, // {"eventType": "GAME_REPLAY", "Player" : "playerId": uuid(0)}

	SESSION_LEFT = 8, // {"eventType": "PLAYER_LEAVE_SESSION", "Player" : "playerId": uuid(0)}

	PLAYER_INFO = 9, // {"eventType": "PLAYER_INFO", "Player" : {"name": "name", "profilePicturePath": "path", "color": [0, 0, 0, 0]}

	BAD_SESSION_ID = 101, // {"eventType": "BAD_SESSION_ID"}
	BAD = 500, // {"eventType": "BAD", error...}
};

enum class TgatClientMessage
{
	PLAYER_INPUT = 0, // {"eventType": "PLAYER_INPUT", "Move" : {"x": 0, "y": 0}
	PLAYER_CHANGE_INFO = 1, // {"eventType": "PLAYER_CHANGE_INFO", "Player" : {"name": "name", "profilePicturePath": "path", "color": [0, 0, 0, 0]}
	CREATE_SESSION = 2, // {"eventType": "CREATE_SESSION"}
	JOIN_SESSION = 3, // {"eventType": "JOIN_SESSION", "Session" : "sessionId": uuid(0)}
	REPLAY = 4, // {"eventType": "REPLAY"}
	LEAVE_SESSION = 5, // {"eventType": "LEAVE_SESSION"}
	PLAYER_DISCONNECT = 6, // {"eventType": "PLAYER_DISCONNECT", "Player" : "playerId": uuid(0)}
};

struct PlayerDisplayData
{
	std::string name;
	std::string profilePicturePath;
	std::string profilePictureThumbPath;
	std::array<uint32_t, 4> color;
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