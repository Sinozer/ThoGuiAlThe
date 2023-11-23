#pragma once

/* ##################  SFML  ################## */
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
/* ##################  SFML  ################## */

/* ################# STANDARD ################# */
#include <functional>
#include <iostream>
#include <queue>

#include <map>
#include <vector>
#include <stack>
#include <string>
#include <queue>
/* ################# STANDARD ################# */

#include "tgatLib.h"

/* ################  CONSTANTS  ############### */
#include "constants.h"
/* ################  CONSTANTS  ############### */

/* ##################  SELF  ################## */
#include "App/Singleton/SingletonManager.h"

#include "App/Asset/AssetManager.h"
#include "App/State/StateManager.h"

#include "App/State/State.h"
//#include "App/UI/Nodes/UIElements.h"
/* ##################  SELF  ################## */

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Replace new to check for memory leaks
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif