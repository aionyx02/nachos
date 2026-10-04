// scheduler.h 
//	Data structures for the thread dispatcher and scheduler.
//	Primarily, the list of threads that are ready to run.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <list>

#include "copyright.h"
#include "list.h"
#include "thread.h"

// The following class defines the scheduler/dispatcher abstraction -- 
// the data structures and operations needed to keep track of which 
// thread is running, and which threads are ready but not running.

enum SchedulerType {
        RR,     // Round Robin
        SJF,
        Priority
};

class Scheduler {
  public:
	Scheduler();		// Initialize list of ready threads 
	~Scheduler();				// De-allocate ready list

	void ReadyToRun(Thread* thread);	
    					// Thread can be dispatched.
	Thread* FindNextToRun();	// Dequeue first thread on the ready 
					// list, if any, and return thread.
	void Run(Thread* nextThread, bool finishing);
	    				// Cause nextThread to start running
	void CheckToBeDestroyed();	// Check if thread that had been
    					// running needs to be deleted
	void Print();			// Print contents of ready list
    
    // SelfTest for scheduler is implemented in class Thread
  private:
	SchedulerType schedulerType;
	List<Thread *> *readyList;	// queue of threads that are ready to run,
					// but not running
	Thread *toBeDestroyed;		// finishing thread to be destroyed
    					// by the next thread that runs
};

// 睡眠佇列：記錄每個 thread 該在第幾次中斷醒來
class sleepFunc {
	public:
		sleepFunc():currentINT(0) {};
		void napTime(Thread* t, int x);	// 讓 t 睡 x 次中斷
		bool wakeUp();				// 叫醒到期的 thread
		bool isEmpty();				// 是否沒人在睡
	private:
		class sleep_T {
			public:
				sleep_T(Thread *t, int x):sleepThread(t), when(x) {};
				Thread* sleepThread;
				int when;				// 醒來的中斷次數
		};

		int currentINT;				// 目前累計的中斷次數
		std::list<sleep_T> T_list;
};
#endif // SCHEDULER_H
