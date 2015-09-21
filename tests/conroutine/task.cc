

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iostream>

#include "task.h"

std::queue<std::function<void(void)> > callbacks;

class SSOTask : public Task
{
public:
	virtual int run() {
		TASK_BEGIN();

		callbacks.push([this]() {
				std::cout << "recv callback:" << this->id << std::endl;
				char res[100] = {};
				sprintf(res, "user-%llu", this->id);
				this->user = res;

				std::cout << "sso do:" << this->user << std::endl;

				TASK_RESUME(this);
				});

		TASK_WAIT_FINISH();

		TASK_END();
		return 0;
	}
public:
	uint64_t id;
	std::string user;
};

class TinyIdTask : public Task
{
public:
	std::map<uint64_t, std::string> _ids;
	std::vector<uint64_t> _in;
public:
	virtual int run() {
		std::cout << "tinyid test 1" << std::endl;
		TASK_BEGIN();
		std::cout << "tinyid test 2" << std::endl;

		sso.id = _in[0];
		_in.push_back(sso.id);

		TASK_RUN(&sso);

		_ids[sso.id] = sso.user;

		std::cout << "tinyid over:" << sso.user << std::endl;

		TASK_END();
		return true;
	}

public:
	SSOTask sso;
};

class TestTask : public Task
{
public:
	virtual int run()
	{
		TASK_BEGIN();

		_idcvt._in.push_back(taskid);

		std::cout << taskid << "--icv begin" << std::endl;

		TASK_RUN(&_idcvt);

		std::cout << taskid << "--icv over" << std::endl;
		for (auto it = _idcvt._ids.begin(); it != _idcvt._ids.end(); it++) {
			std::cout << taskid << "--\tkey=" << it->first << "; value=" << it->second << std::endl;
		}

		std::cout << taskid << "--test task over" << std::endl;

		TASK_END();
		delete this;
		return true;
	}
public:
	TinyIdTask _idcvt;
	int taskid;
};

class ConcurrentTasksTest : public Task
{
public:
	virtual int run() {
		TASK_BEGIN();

		for (int i = 0; i < 10; i++) {
			TinyIdTask * task = new TinyIdTask;
			task->_in.push_back(i);
			tasks.add_task(task);
		}

		std::cout << "---- before concurrent exec" << std::endl;
		TASK_CONCURRENT_EXEC(&tasks);
		std::cout << "---- end concurrent exec" << std::endl;

		TASK_END();
	}
private:
	ConcurrentTask tasks;
};

void do_callback()
{
	while (!callbacks.empty()) {
		std::cout << "callback start" << std::endl;
		auto fn = callbacks.front();
		fn();
		callbacks.pop();
	}
}

int main()
{
	for (int i = 0; i < 10; i++) {
		TestTask * task = new TestTask;
		task->taskid = i;
		task->run();
	}
	std::cout << "==== begin callback in main ====" << std::endl;

	do_callback();

	std::cout << "==== begin concurrent test in main ====" << std::endl;
	ConcurrentTasksTest concurrent_test;
	concurrent_test.run();

	std::cout << "==== begin concurrent callback in main ====" << std::endl;
	do_callback();
	std::cout << "==== end concurrent test in main ====" << std::endl;
	return 0;
}

