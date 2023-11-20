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

void ThreadManager::CreateThreads()
{
	std::vector<HANDLE> threads;
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
			threads.push_back(thread);
		}
		else
		{
			std::cout << "Error creating thread " << i << std::endl;
		}
	}
}

void ThreadManager::CloseThreads(std::vector<HANDLE> threads)
{
	for (int i = 0; i < threads.size(); i++)
	{
		CloseHandle(threads[i]);
	}
}