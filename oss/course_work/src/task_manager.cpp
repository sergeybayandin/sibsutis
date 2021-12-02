#include "../include/task_manager.h"
#include <QRegularExpression>
#include <QTextStream>

namespace Tm
{
	
	QList<TaskManager::ProcessItem>
	TaskManager::processesList() const noexcept
	{
		auto count {procDir_.count()};
		QRegularExpression reDigits {"[0-9]+"}, reBrackets {"[()]"};
		QList<ProcessItem> processesList;
		
		for (decltype(count) i {}; i < count; ++i) {
			if (!procDir_[i].contains(reDigits)) {
				continue;
			}

			QFile statFile {
				procDir_.path() +
				"/" +
				procDir_[i] + 
				"/" + 
				"stat"
			};
			
			if (!statFile.open(QIODevice::ReadOnly)) {
				continue;
			}

			QTextStream dataStream {&statFile};
			ProcessItem processItem;
			
			dataStream >> processItem.second >> processItem.first;
		
			processItem.first.remove(reBrackets);

			processesList << processItem;
		}

		return processesList;
	}

	bool TaskManager::killProcess(pid_t pid, int sig) const noexcept
	{
		return !kill(pid, sig);
	}

	TaskManager &TaskManager::instance() noexcept
	{
		static TaskManager taskManager;
		return taskManager;
	}

	TaskManager::TaskManager() : procDir_ {{"/proc"}}
	{}

} // Tm
