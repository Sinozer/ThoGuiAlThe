
#define MAXTHREADS 4
#define MAXEVENTS 2
#pragma once
class ThreadManager
{

public:
	ThreadManager() {};
	~ThreadManager() {};

	/* Threads */
	std::vector<HANDLE>* threads;
	std::vector<HANDLE>* CreateThreads(DWORD __stdcall appfunc);
	void CloseThreads(std::vector<HANDLE> threads);


	std::vector<HANDLE>* getThreads() { return threads; }
	void setThreads(std::vector<HANDLE>* threads) { this->threads = threads; }
	/* Threads */

	/* Events */
	std::vector<HANDLE>* events;
	std::vector<HANDLE>* CreateEvents(int nbEvent);
	void CloseEvents(std::vector<HANDLE> events);

	std::vector<HANDLE>* getEvents() { return events; }
	void setEvents(std::vector<HANDLE>* events) { this->events = events; }
	/* Events */

	/* Mutex */
	HANDLE mutex;
	HANDLE InitMutex();
	void CloseMutex(HANDLE mutex);
	/* Mutex */

private:


};

