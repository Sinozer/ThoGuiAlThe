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

	DELPTR(s_Instance);
}
#pragma endregion

StateManager::StateManager() : m_NewState(nullptr), m_Adding(false), m_Removing(0), m_Clearing(false)
{
}

StateManager::~StateManager()
{
	for (int i = 0; i < m_States.size(); i++)
	{
		DELPTR(m_States.top());
		m_States.pop();
	}

	DELPTR(m_NewState);
}

void StateManager::AddState(State* state)
{
	m_Adding = true;
	m_NewState = state;
}

void StateManager::RemoveState(unsigned char amount)
{
	m_Removing = amount;
}

void StateManager::RemoveAllStates()
{
	m_Clearing = true;
}

void StateManager::GoToFirstState()
{
	m_Removing = (uint32_t)m_States.size() - 1;
}

bool StateManager::IsEmpty()
{
	return m_States.empty();
}

void StateManager::Add()
{
	m_Adding = false;
	m_States.push(m_NewState);
	m_NewState = nullptr;

	m_States.top()->Init();
}
void StateManager::Remove()
{
	if (m_Removing > m_States.size())
	{
		Clear();
		return;
	}

	for (unsigned char i = 0; i < m_Removing; i++)
	{
		m_States.top()->End();
		DELPTR(m_States.top());
		m_States.pop();
	}

	m_Removing = 0;
}
void StateManager::Clear()
{
	m_Clearing = false;

	while (!m_States.empty())
	{
		m_States.top()->End();
		DELPTR(m_States.top());
		m_States.pop();
	}
}
void StateManager::ProcessStateChanges()
{
	if (m_Removing > 0 && !m_States.empty())
		Remove();

	if (m_Clearing && !m_States.empty())
		Clear();

	if (m_Adding)
		Add();
}

State* StateManager::GetActiveState()
{
	return m_States.top();
}