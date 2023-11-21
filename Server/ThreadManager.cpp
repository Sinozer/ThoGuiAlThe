#include "stdafx.h"
#include "ThreadManager.h"
#include "vector"

DWORD WINAPI threadFunction(LPVOID lpParam)
{
	int threadID = *(reinterpret_cast<int*>(lpParam));
	std::cout << "Thread " << threadID << " is running" << std::endl;

	for (int i = 0; i < MAXTHREADS; i++)
	{
		std::cout << "Thread " << threadID << " is running " << i << std::endl;
		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI eventFunction(LPVOID lpParam, int nbEvent) 
{
	int eventID = *(reinterpret_cast<int*>(lpParam));
	std::cout << "Event " << eventID << " is running" << std::endl;

	for (int i = 0; i < MAXEVENTS; i++)
	{
		std::cout << "Event " << eventID << " is running " << i << std::endl;
		Sleep(1000);
	}

	return 0;
}

std::vector<HANDLE>* ThreadManager::CreateThreads()
{
	std::vector<int> threadIDs(MAXTHREADS);

	for (int i = 0; i < MAXTHREADS; i++)
	{
		threadIDs[i] = i;
		HANDLE thread = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			threadFunction,       // thread function name
			&threadIDs[i],          // argument to thread function 
			0,                      // use default creation flags 
			NULL);   // returns the thread identifier

		if (thread != NULL) 
		{
			threads->push_back(thread);
		}
		else
		{
			std::cout << "Error creating thread " << i << std::endl;
		}
	}
	return threads;
}

void ThreadManager::CloseThreads(std::vector<HANDLE> threads)
{
	for (int i = 0; i < threads.size(); i++)
	{
		CloseHandle(threads[i]);
	}
}

std::vector<HANDLE>* ThreadManager::CreateEvents(int nbEvent)
{
	std::vector<int> eventIDs(nbEvent);

	for (int i = 0; i < nbEvent; i++)
	{
		eventIDs[i] = i;
		HANDLE event = CreateEvent(
			NULL,   // default security attributes
			FALSE,  // auto-reset event object
			FALSE,  // initial state is nonsignaled
			NULL);  // unnamed object

		if (event != NULL)
		{
			events->push_back(event);
		}
		else
		{
			std::cout << "Error creating event " << i << std::endl;
		}
	}
	return events;
}

void ThreadManager::CloseEvents(std::vector<HANDLE> events)
{
	for (int i = 0; i < events.size(); i++)
	{
		CloseHandle(events[i]);
	}
}

HANDLE ThreadManager::InitMutex()
{
	HANDLE mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (mutex != NULL)
	{
		return mutex;
	}
	else
	{
		std::cout << "Error creating mutex" << std::endl;
		return NULL;
	}
}

void ThreadManager::CloseMutex(HANDLE mutex)
{
	CloseHandle(mutex);
}
