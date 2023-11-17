#pragma once

/* ################  CONSTANTS  ############### */
#include "constants.h"
/* ################  CONSTANTS  ############### */

/* ##################  SFML  ################## */
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
/* ##################  SFML  ################## */

/* #################  NETWORK  ################ */
#include "JSON/json.hpp"
/* #################  NETWORK  ################ */

/* ################# STANDARD ################# */
#include <functional>
#include <iostream>

#include <map>
#include <vector>
#include <stack>
#include <string>
/* ################# STANDARD ################# */

/* ##################  WINSOCK  ############### */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Winerror.h>
#include <WS2tcpip.h>
/* ##################  WINSOCK  ############### */

/* #################  DEBUG  ################## */
#include <assert.h>
#if defined(_DEBUG) || defined(DEBUG) && !defined(LOG)
#define LOG(x) std::cout << x << '\n'
#else
#define LOG(x)
#endif
/* #################  DEBUG  ################## */

/* ##################  SELF  ################## */
#include "App/Singleton/SingletonManager.h"

#include "App/Network/NetworkManager.h"
#include "App/Asset/AssetManager.h"
#include "App/State/StateManager.h"

#include "App/State/State.h"
//#include "App/UI/Nodes/UIElements.h"
/* ##################  SELF  ################## */

#define DELPTR(ptr) if (ptr) { delete (ptr); (ptr) = nullptr; }
#define NULLPTR(ptr) if (ptr) { (ptr) = nullptr; }
