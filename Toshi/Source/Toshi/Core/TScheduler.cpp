#include "pch.h"
#include "TScheduler.h"
#include "TTask.h"

namespace Toshi
{
	TOSHI_CLASS_DERIVED_INITIALIZE(TScheduler, TObject, MKVERSION(1, 0))
		
	TTask* TScheduler::CreateTask(TClass* toshiClass, TTask* task)
	{
		TTask* taskObject = static_cast<TTask*>(toshiClass->CreateObject());
		TScheduler* taskScheduler = taskObject->GetScheduler();

		if (taskScheduler != this)
		{
			if (taskScheduler)
			{
				taskScheduler->DeleteTask();
			}

			taskObject->m_Scheduler = this;
			m_TaskCount += 1;
		}

		TTODO("FUN_100253f0 and everything else");

		return nullptr;
	}
}