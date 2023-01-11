#include "ToshiPCH.h"
#include "TScheduler.h"
#include "TSystem.h"
#include "TTask.h"

namespace Toshi
{
	float TScheduler::s_MaxTimeDeltaAllowed = 0.25f;
	float TScheduler::s_DebugSlowMaxTimeDeltaAllowed = 1.0f;

	TScheduler::TScheduler()
	{
		m_DeltaTime = 0.0;
		m_TasksUpdateTime = 0.0;
		m_TotalTime = 0.0;
		m_MaxDeltaTime = 0.25;
		m_FrameCount = 0;
		m_UseFixedMaxFps = false;
		m_FixedMaxFps = 0.0;
		m_UseDebugDeltaTime = false;
		m_DebugDeltaTime = 0.01;
		m_DebugDeltaTimeMult = 1.0;
	}

	TTask* TScheduler::CreateTask(TClass* pClass, TTask* pParent)
	{
		TASSERT(pClass->IsA(TGetClass(TTask)));
		
		TTask* pTask = static_cast<TTask*>(pClass->CreateObject());
		TASSERT(pTask != TNULL);

		if (pParent == TNULL)
		{
			m_TaskTree.InsertAtRoot(pTask);
		}
		else
		{
			m_TaskTree.Insert(pParent, pTask);
		}

		return pTask;
	}

	void TScheduler::DeleteTaskAtomic(TTask* task)
	{
		if (task != TNULL)
		{
			TTask* taskParent = TNULL;

			if (task->IsChildOfDefaultRoot() == TFALSE)
			{
				taskParent = task->Parent();

				if (taskParent->IsDying() == TFALSE && taskParent->OnChildDying(task) == TFALSE)
				{
					// The parent is not dying and doesn't allow us to kill the task
					return;
				}
			}

			if (task->Attached() != TNULL)
			{
				TTask* node = task->Attached()->Attached();

				while (node != TNULL)
				{
					auto next = (node->Prev() != node) ? node->Prev() : TNULL;
					DeleteTaskAtomic(node);
					node = next;
				}
			}

			TClass* pClass = task->GetClass();
			task->OnDestroy();
			m_TaskTree.Remove(task, false);
			task->Delete();

			if (taskParent != TNULL)
			{
				taskParent->OnChildDied(pClass, task);
			}
		}
	}

	void TScheduler::DestroyDyingTasks(TTask* task)
	{
		if (task != TNULL)
		{
			TTask* currentTask = task->Prev();

			while (currentTask != TNULL)
			{
				TTask* nextTask = (currentTask->Prev() != task) ? currentTask->Prev() : TNULL;

				if (!currentTask->IsDying())
				{
					if (currentTask->Attached() != TNULL)
					{
						DestroyDyingTasks(currentTask->Attached());
					}
				}
				else
				{
					DeleteTask(currentTask);
				}

				currentTask = nextTask;
			}
		}
	}

	void TScheduler::UpdateActiveTasks(TTask* task)
	{
		TTask* currentTask = task;

		while (currentTask != TNULL)
		{
			TTask* nextTask = (currentTask->Next() != task) ? currentTask->Next() : TNULL;

			bool recurse = true;
			if (task->IsCreated() && task->IsActive())
			{
				recurse = currentTask->OnUpdate(m_DeltaTime);
			}

			if (currentTask->Attached() != TNULL && recurse)
			{
				UpdateActiveTasks(currentTask->Attached());
			}

			currentTask = nextTask;
		}
	}
}