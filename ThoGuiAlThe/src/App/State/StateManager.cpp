#include "StateManager.h"

#pragma region Singleton
StateManager* StateManager::s_Instance = nullptr;

StateManager* StateManager::GetInstance()
{
	if (s_Instance == nullptr)
		s_Instance = new StateManager();

	return s_Instance;
}

const void StateManager::DestroyInstance()
{
	if (s_Instance == nullptr)
		return;

	delete s_Instance;
	s_Instance = nullptr;
}
#pragma endregion

StateManager::StateManager()
	: m_Adding(false)
	, m_Removing(false)
	, m_NewState(nullptr)
{
}

void StateManager::AddState(State* state)
{
	m_Adding = true;
	m_NewState = state;
}

void StateManager::RemoveState()
{
	m_Removing = true;
}

void StateManager::RemoveAllStates()
{
	m_Clearing = true;
}

bool StateManager::IsEmpty()
{
	return m_States.empty();
}

void StateManager::m_Add()
{
	m_Adding = false;
	m_States.push(m_NewState);
	m_NewState = nullptr;

	m_States.top()->Init();
}
void StateManager::m_Remove()
{
	m_Removing = false;

	m_States.top()->End();
	delete m_States.top();
	m_States.pop();
}
void StateManager::m_Clear()
{
	m_Clearing = false;

	while (!m_States.empty())
	{
		m_States.top()->End();
		delete m_States.top();
		m_States.pop();
	}
}
void StateManager::ProcessStateChanges()
{
	if (m_Removing && !m_States.empty())
		m_Remove();

	if (m_Clearing && !m_States.empty())
		m_Clear();

	if (m_Adding)
		m_Add();
}

State* StateManager::GetActiveState()
{
	return m_States.top();
}