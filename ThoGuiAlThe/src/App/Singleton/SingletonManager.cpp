#include "SingletonManager.h"
#include "App/Network/NetworkManager.h"

void SingletonManager::DestroyAllInstances()
{
	StateManager::DestroyInstance();
	AssetManager::DestroyInstance();
	NetworkManager::DestroyInstance();
}