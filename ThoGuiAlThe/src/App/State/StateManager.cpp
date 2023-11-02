#include "stdafx.h"
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
	while (!m_States.empty())
	{
		delete m_States.top();
		m_States.pop();
	}
}

bool StateManager::IsEmpty()
{
	return m_States.empty();
}

void StateManager::ProcessStateChanges()
{
	if (m_Removing && !m_States.empty())
	{
		m_Removing = false;

		m_States.top()->End();
		m_States.pop();
	}

	if (m_Adding)
	{
		m_Adding = false;
		m_States.push(m_NewState);
		m_NewState = nullptr;

		m_States.top()->Init();
	}
}

State* StateManager::GetActiveState()
{
	return m_States.top();
}