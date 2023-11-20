
/* ################# STANDARD ################# */
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <memory>
#include <conio.h>
#include <regex>
/* ################# STANDARD ################# */

/* ##################  WINSOCK  ################## */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>
/* ##################  WINSOCK  ################## */

/* #################  DEBUG  ################## */
#if defined(_DEBUG) || defined(DEBUG) && !defined(LOG)
#define LOG(x) std::cout << x << '\n'
#else
#define LOG(x)
#endif
/* #################  DEBUG  ################## */

#include "tgatLib.h"
