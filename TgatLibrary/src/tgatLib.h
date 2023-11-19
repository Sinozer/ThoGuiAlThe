#pragma once

/* ##################  WINSOCK  ############### */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>
/* ##################  WINSOCK  ############### */

/* #################  UTILITY  ################ */
#define DELPTR(ptr) if (ptr) { delete (ptr); (ptr) = nullptr; }
#define NULLPTR(ptr) if (ptr) { (ptr) = nullptr; }
/* #################  UTILITY  ################ */

/* #################  JSON	################ */
#include "External/JSON/json.hpp"
/* #################  JSON	################ */

/* #################  TYPEDEF  ################ */
typedef uint64_t TGATPLAYERID;
typedef uint64_t TGATBODYSIZE;
typedef uint64_t TGATDATASIZE;
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