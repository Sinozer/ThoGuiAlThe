#pragma once
class ThreadExemple
{

public:
	ThreadExemple();
	~ThreadExemple();
	void Start();
	void Stop();

	static void ThreadProc();


private:
};

