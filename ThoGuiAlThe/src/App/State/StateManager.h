#pragma once

class State;

class StateManager
{
#pragma region Singleton
public:
	static StateManager* GetInstance();
	static const void DestroyInstance();
private:
	static StateManager* s_Instance;
#pragma endregion

public:
	StateManager();
	~StateManager();

	void AddState(State* state);
	void RemoveState(unsigned char amount = 1);
	void RemoveAllStates();
	void GoToFirstState();

	bool IsEmpty();

	void ProcessStateChanges();

	State* GetActiveState();

private:
	std::stack<State*> m_States;

	State* m_NewState; 
	CRITICAL_SECTION m_StateCS;

	bool m_Adding;
	uint32_t m_Removing;
	bool m_Clearing;

	void Add();
	void Remove();
	void Clear();

};