#include <Windows.h>
#include <stdio.h>
#include <vector>

constexpr auto THREADCOUNT = 2;

HANDLE ghEvents[2];
HANDLE hThread;
HANDLE ghMutex;

DWORD WINAPI ThreadProc(LPVOID);
DWORD WINAPI WriteToDatabase(LPVOID);

CRITICAL_SECTION CriticalSection;


DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);

	// lpParam not used in this example
	DWORD dwWaitResult;

	EnterCriticalSection(&CriticalSection);


	LeaveCriticalSection(&CriticalSection);
	return 1;
}

DWORD WINAPI WriteToDatabase(LPVOID lpParam)
{
	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwCount = 0;
	DWORD dwWaitResult;

	// Request ownership of mutex.

	while (dwCount < 2)
	{
		dwWaitResult = WaitForSingleObject(
			ghMutex,    // handle to mutex
			INFINITE);  // no time-out interval

		switch (dwWaitResult)
		{
			// The thread got ownership of the mutex
		case WAIT_OBJECT_0:
			__try {
				// TODO: Write to the database
				printf("Thread %d writing to database...\n",
					GetCurrentThreadId());

				dwCount++;
			}

			__finally {
				// Release ownership of the mutex object
				if (!ReleaseMutex(ghMutex))
				{
					// Handle error.
				}
			}
			break;

			// The thread got ownership of an abandoned mutex
			// The database is in an indeterminate state
		case WAIT_ABANDONED:
			return FALSE;
		}
	}
	return TRUE;
}

void CreateThreads()
{
	DWORD dwThreadIdArray[THREADCOUNT]{};
	HANDLE hThreadArray[THREADCOUNT]{};

	// Create THREADCOUNT worker threads.

	for (int i = 0; i < THREADCOUNT; i++)
	{
		// Create the thread to begin execution on its own.
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			ThreadProc,             // thread function name
			NULL,                   // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier
	}
}

int main()
{
	if(!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
		return 0;
	std::vector<HANDLE> threads;
	for (int i = 0; i < THREADCOUNT; i++)
	{
		threads.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateThreads, NULL, 0, NULL));
	}

	for (HANDLE thread : threads)
	{
		WaitForSingleObject(thread, INFINITE);
	}

	std::cout << "All threads finished" << std::endl;
	DeleteCriticalSection(&CriticalSection);
	return 0;

}



void ManageEventObjects() {
	DWORD i;
	DWORD dwEvent;
	DWORD dwThreadID;

	// Create two event objects
	for (i = 0; i < 2; i++)
	{
		ghEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (ghEvents[i] == NULL)
		{
			printf("CreateEvent failed (%d)\n", GetLastError());
			ExitProcess(0);
		}
	}

	//Create Thread for event objects
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, &dwThreadID);
	if (hThread == NULL)
	{
		printf("CreateThread failed (%d)\n", GetLastError());
	}

	//Wait for the thread to signal one of the event objects
	dwEvent = WaitForMultipleObjects(2, ghEvents, FALSE, 5000);


	// Close event handles
	for (i = 0; i < 2; i++)
	{
		CloseHandle(ghEvents[i]);
	}

}

void ManageMutex()
{
	HANDLE aThread[THREADCOUNT]{};
	DWORD ThreadID;
	DWORD i;


	// Create a mutex with no initial owner
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
	}

	//Create worker threads
	for (i = 0; i < THREADCOUNT; i++)
	{
		aThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteToDatabase, NULL, 0, &ThreadID);
		if (aThread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
		}
	}
	//Wait for all threads to terminate

	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

	//Close thread and mutex handles
	for (i = 0; i < THREADCOUNT; i++)
	{
		CloseHandle(aThread[i]);
	}
	ReleaseMutex(ghMutex);
	CloseHandle(ghMutex);
}