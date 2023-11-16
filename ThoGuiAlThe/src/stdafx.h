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

/* ################# STANDARD ################# */
#include <functional>
#include <iostream>

#include <map>
#include <vector>
#include <stack>
#include <string>
/* ################# STANDARD ################# */

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

#include "App/Asset/AssetManager.h"
#include "App/State/StateManager.h"

#include "App/State/State.h"
//#include "App/UI/Nodes/UIElements.h"
/* ##################  SELF  ################## */


