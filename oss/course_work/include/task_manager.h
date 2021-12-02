#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <QList>
#include <QDir>
#include <sys/types.h>
#include <signal.h>

namespace Tm
{

	class TaskManager final
	{
		using ProcessItem = QPair<QString, pid_t>;
		
		TaskManager();

	public:
		TaskManager(const TaskManager&) = delete;
		TaskManager(TaskManager&&) = delete;

		TaskManager operator=(const TaskManager&) = delete;

		QList<ProcessItem>
		processesList() const noexcept;

		bool killProcess(pid_t pid, int sig = SIGTERM) const noexcept;

		static TaskManager &instance() noexcept;

	private:
		const QDir procDir_;
	};

} // Tm

#endif // TASK_MANAGER_H
