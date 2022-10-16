#pragma once

#include "Toshi/Core/TRefCounted.h"
#include "Toshi/File/TFile.h"
#include "Toshi/Render/TRenderContext.h"

namespace Toshi
{

	class TRenderInterface :
		public TGenericClassDerived<TRenderInterface, TObject, "TRenderInterface", TMAKEVERSION(1, 0), false>,
		TRefCounted,
		TSingleton<TRenderInterface>
	{

		const int SYSRESOURCE_NUMOF = 12;

		uint8_t m_bInScene;			// 0x9
		uint8_t m_bCreated;			// 0xA
		uint8_t m_bDisplayCreated;	// 0xB
		TRenderContext* m_pRenderContext; // 0x14
		void* m_aSysResources;		// 0x20
		
	public:

		TRenderInterface() {}

		inline TRenderContext* GetCurrentRenderContext() { return m_pRenderContext; }

		bool CreateDisplay();
		inline bool IsInScene() { return m_bInScene; }
		inline bool IsCreated() { return m_bCreated; }
		inline bool IsDisplayCreated() { return m_bDisplayCreated; }
		virtual void DumpStats();

	};
}


