#include "SingletonManager.h"

void SingletonManager::DestroyAllInstances()
{
	StateManager::DestroyInstance();
	AssetManager::DestroyInstance();
	NetworkManager::DestroyInstance();
}