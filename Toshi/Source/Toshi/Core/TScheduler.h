#pragma once
#include "TKernelInterface.h"
#include "TNodeTree.h"

namespace Toshi
{
	class TTask;

	class TScheduler :
		public TGenericClassDerived<TScheduler, TObject, "TScheduler", TMAKEVERSION(1, 0), false>
	{
	public:
		TScheduler(TKernelInterface* kernelInterface);
		~TScheduler() { TTODO("Free all the TTasks"); }

		// Creates task from TClass and binds it to this scheduler
		TTask* CreateTask(TClass* toshiClass, TTask* parentTask);
		
		// Updates all of the attached TTasks
		void Update();

		// Slowdowns time
		void SetDebugSlowTime(bool slowTime);

	public:
		inline TKernelInterface* GetKernelInterface() const { return m_KernelInterface; }
		inline float GetCurrentTimeDelta() const { return m_CurrentTimeDelta; }
		inline float GetTotalTime() const { return m_TotalTime; }
		inline uint32_t GetFrameCount() const { return m_FrameCount; }
		inline TNodeTree<TTask> GetTree() const { return m_TaskTree; }
		
		inline uint32_t TaskCount() const { return m_TaskCount; }
		inline void TaskCount(uint32_t count) { m_TaskCount = count; }

		inline void UpdateKernelPaused()
		{
			UpdateActiveTasksKernelPaused(m_LastTask);
		}

		inline void DeleteTask()
		{
			uint32_t oldTaskCount = TaskCount();
			TaskCount(oldTaskCount - 1);

			if (oldTaskCount == 1) { Delete(); }
		}

	private:
		// Destroys all the dying tasks from the first one to the last one
		void DestroyDyingTasks(TTask* rootTask);

		// Updates all the active tasks from the last one to the first one
		void UpdateActiveTasks(TTask* rootTask);

		// Updates all the active tasks from the last one to the first one when the kernel is paused
		void UpdateActiveTasksKernelPaused(TTask* rootTask);

		// Deletes task recursively
		void DeleteTask(TTask* task);

	private:
		static float s_DebugSlowMaxTimeDeltaAllowed;
		static float s_MaxTimeDeltaAllowed;
			
	private:
		uint32_t m_TaskCount;                   // 0x04
		TKernelInterface* m_KernelInterface;    // 0x08
		TNodeTree<TTask> m_TaskTree;            // 0x0C
		void* m_Unk1;                           // 0x10
		void* m_Unk2;                           // 0x14
		uint32_t m_Unk3;                        // 0x18
		TTask* m_LastTask;                      // 0x1C
		uint32_t m_Unk4;                        // 0x20
		float m_CurrentTimeDelta;               // 0x24
		float m_TotalTime;                      // 0x28
		float m_DeltaTimeLimit;                 // 0x2C
		uint32_t m_FrameCount;                  // 0x30
	};
}