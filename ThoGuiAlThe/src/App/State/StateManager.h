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
	~StateManager() = default;

	void AddState(State* state);
	void RemoveState();
	void RemoveAllStates();

	bool IsEmpty();

	void ProcessStateChanges();

	State* GetActiveState();

private:
	std::stack<State*> m_States;

	State* m_NewState;

	bool m_Adding;
	bool m_Removing;
	bool m_Clearing;

	void Add();
	void Remove();
	void Clear();

};