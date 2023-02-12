#pragma once
#include "Toshi/Core/TFifo.h"
#include "Toshi/Thread/TThread.h"
#include "Toshi/File/TTRB.h"

namespace Toshi
{
	class TFileStreamJob
	{
	public:
		friend class TFileStream;

	public:
		TFileStreamJob(TSemaphore* semaphore) : m_pSemaphore(semaphore), m_bIsProcessed(false) { }

		virtual ~TFileStreamJob() = 0;

		virtual void Process() = 0;

	protected:
		TSemaphore* m_pSemaphore;
		bool m_bIsProcessed;
	};

	class TFileStream : public TThread
	{
	public:
		TFileStream()
		{
			m_Jobs = TFifo<TFileStreamJob*, 32>();
		}

		virtual ~TFileStream() = default;
		
		// This method will be executed by the thread
		virtual void Main() override;

		// Adds job to the FIFO
		void AddStream(TFileStreamJob* job);

	private:
		TFifo<TFileStreamJob*, 32> m_Jobs;
	};

	class TTRBStreamJob : public TFileStreamJob
	{
	public:
		TTRBStreamJob() : TFileStreamJob(TNULL)
		{
			m_trb = TNULL;
			m_fileName = TNULL;
		}

		virtual void Process()
		{
			m_trb->Load(m_fileName);
		}

		void Init(TTRB* trb, const char* fileName)
		{
			m_trb = trb;
			T2String8::Copy(m_fileName, fileName, -1);
		}
	public:
		TTRB* m_trb;
		char* m_fileName;
	};
}