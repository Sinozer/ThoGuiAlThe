#pragma once
#include "Exceptions/TgatException.h"

class TgatThread
{

public:
	TgatThread() : m_ThreadHandle(nullptr), m_ThreadID(0) {}

	template<typename ThreadMain, typename... Args>
	TgatThread(ThreadMain&& mainFunction, Args&&... args)
        : m_ThreadHandle(nullptr), m_ThreadID(0)
	{
		m_ThreadHandle = CreateThread(NULL, 0, 
			ThreadWrapper<ThreadMain, Args...>,
			new std::tuple<std::decay_t<ThreadMain>, std::decay_t<Args>...>{ std::forward<ThreadMain>(mainFunction), std::forward<Args>(args)... },
			0, &m_ThreadID
		);

		if (m_ThreadHandle == nullptr)
		{
			throw TgatException("Error creating thread");
		}
	}
	~TgatThread() 
	{
		if (Joinable())
		{
			CloseHandle(m_ThreadHandle);
			m_ThreadHandle = nullptr;
		}
	}

	void Join()
	{
		if (Joinable() == false)
			return;

		WaitForSingleObject(m_ThreadHandle, INFINITE);
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle = nullptr;
		m_ThreadID = 0;
	}

	bool Joinable() const
	{
		return m_ThreadHandle != nullptr;
	}

#pragma region Operators
	TgatThread(const TgatThread&) = delete; // Copy constructor
	TgatThread& operator=(TgatThread&& other) noexcept
	{
		if (this == &other)
			return *this;

		if (Joinable())
		{
			// Ensure the thread has finished before closing the handle
			WaitForSingleObject(m_ThreadHandle, INFINITE);
			CloseHandle(m_ThreadHandle);
		}
		// Move the resources from 'other' to the current instance
		m_ThreadHandle = other.m_ThreadHandle;
		m_ThreadID = other.m_ThreadID;

		// Reset 'other' to a valid state (optional, but recommended)
		other.m_ThreadHandle = nullptr;
		other.m_ThreadID = 0;

		return *this;
	}
#pragma endregion

private:
	HANDLE m_ThreadHandle;
	DWORD m_ThreadID;

	template<typename ThreadMain, typename... Args>
	static DWORD ThreadWrapper(LPVOID lpParam)
	{
        auto params = reinterpret_cast<std::tuple<ThreadMain, Args...>*>(lpParam);
		std::apply([](ThreadMain& mainFunction, Args&... args)
			{
				mainFunction(args...);
			}, *params
		);

        delete params;
        return 0;
    }
};
