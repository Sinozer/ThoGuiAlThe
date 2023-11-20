
#define MAXTHREADS 4
#pragma once
class ThreadManager
{

public:
	ThreadManager() {};
	~ThreadManager() {};

	void CreateThreads();
	void CloseThreads(std::vector<HANDLE> threads);
private:
};

