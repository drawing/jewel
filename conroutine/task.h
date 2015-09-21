#ifndef TASK_H
#define TASK_H

#include <pt.h>

#include <vector>

#define TASK_BEGIN()	PT_BEGIN(&this->_pt)
#define TASK_END()	this->_finish = true;				\
		PT_END(&this->_pt)

#define TASK_RUN(task)  do {					\
		this->add_task(task);				\
		PT_WAIT_UNTIL(&this->_pt, (task)->start());	\
	} while (false)

#define TASK_INIT()	PT_INIT(&this->_pt)

#define TASK_RESUME(task)	do {				\
		this->_finish = true;				\
		Task * _task = (task);				\
		while (_task->_parent != nullptr) {		\
			_task = _task->_parent;			\
		};						\
		_task->run();					\
	} while (false)

#define TASK_WAIT_FINISH()	PT_WAIT_UNTIL(&this->_pt, this->_finish)

#define TASK_CONCURRENT_EXEC(tasks)	do {				\
		(tasks)->set_parent(this);				\
		PT_WAIT_UNTIL(&this->_pt, (tasks)->execute());		\
	} while (false)

#define TASK_ABORT()	this->_finish = true;	\
			return 0;

class Task
{
public:
	virtual ~Task() {}

	Task() : _finish(false), _parent(nullptr) {
		TASK_INIT();
	}
public:
	virtual int run() = 0;
	void finish() {
		_finish = true;
	}
public:
	bool start() {
		if (_finish) {
			return true;
		}
		else {
			this->run();
			return _finish;
		}
	}
	
	void add_task(Task * _task) {
		_task->_parent = this;
	}
public:
	struct pt _pt;
	bool _finish;
	Task * _parent;
};

class ConcurrentTask
{
public:
	void add_task(Task * task)
	{
		tasks.push_back(task);
	}

	bool execute() {
		bool finish = true;
		for (int i = 0; i < tasks.size(); i++) {
			if (!tasks[i]->start()) {
				finish = false;
			}
		}
		return finish;
	}

	void set_parent(Task * task) {
		for (int i = 0; i < tasks.size(); i++) {
			tasks[i]->_parent = task;
		}
	}

	~ConcurrentTask() {
		for (int i = 0; i < tasks.size(); i++) {
			delete tasks[i];
			tasks[i] = nullptr;
		}
	}
private:
	std::vector<Task*> tasks;
};

#endif // TASK_H

