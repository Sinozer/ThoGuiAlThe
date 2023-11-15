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

/* ################# STANDART ################# */
#include <functional>
#include <iostream>

#include <map>
#include <vector>
#include <stack>
#include <string>
/* ################# STANDART ################# */

/* ##################  SELF  ################## */
#include "App/Singleton/SingletonManager.h"

#include "App/Asset/AssetManager.h"
#include "App/State/StateManager.h"

#include "App/State/State.h"
//#include "App/UI/Nodes/UIElements.h"
/* ##################  SELF  ################## */

#define DELPTR(ptr) if (ptr) { delete (ptr); (ptr) = nullptr; }
#define NULLPTR(ptr) if (ptr) { (ptr) = nullptr; }