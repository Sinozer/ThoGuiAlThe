
/* ################# STANDARD ################# */
#include <iostream>
#include <string>
#include <vector>
/* ################# STANDARD ################# */

/* ##################  WINSOCK  ################## */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>
/* ##################  WINSOCK  ################## */

/* ##################  JSON  ################## */
#include "../lib/JSON/json.hpp"
/* ##################  JSON  ################## */

/* #################  DEBUG  ################## */
#if defined(_DEBUG) || defined(DEBUG) && !defined(LOG)
#define LOG(x) std::cout << x << '\n'
#else
#define LOG(x)
#endif
/* #################  DEBUG  ################## */
