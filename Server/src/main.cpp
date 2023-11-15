#include <Windows.h>
#include <stdio.h>

#define THREADCOUNT 2

HANDLE ghEvents[2];
HANDLE hThread;
HANDLE ghMutex;

DWORD WINAPI ThreadProc(LPVOID);
DWORD WINAPI WriteToDatabase(LPVOID);

int main()
{
	HANDLE aThread[THREADCOUNT];
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
		return 1;
	}

	//Wait for the thread to signal one of the event objects
	dwEvent = WaitForMultipleObjects(2, ghEvents, FALSE, 5000);


	//return = indicates which event object was signaled
	switch (dwEvent)
	{
		case WAIT_OBJECT_0 + 0:
			printf("Event 1 was signaled.\n");
			break;

		case WAIT_OBJECT_0 + 1:
			printf("Event 2 was signaled.\n");
			break;
		case WAIT_TIMEOUT:
			printf("Wait timed out.\n");
			break;
	default:
		printf("Wait error: %d\n", GetLastError());
		break;
	}

	// Close event handles
	for (i = 0; i < 2; i++)
	{
		CloseHandle(ghEvents[i]);
	}
}

void ManageMutex() 
{

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

}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{

	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	// Set one event to the signaled state

	if (!SetEvent(ghEvents[0]))
	{
		printf("SetEvent failed (%d)\n", GetLastError());
		return 1;
	}
	return 0;
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


