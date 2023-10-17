#pragma once
#include "TXUIResource.h"
#include "TXUIRenderer.h"
#include "TXUICanvas.h"
#include "TXUIAudio.h"
#include "Toshi/Utils/TSingleton.h"
#include "Toshi2/T2GUI/T2GUIContext.h"
#include "Toshi/File/TTRB.h"

#ifdef FindResource
#undef FindResource
#endif

TOSHI_NAMESPACE_BEGIN

class TXUIResourceTRB
{
public:
	friend class TXUI;

public:
	void Init();
	void Deinit();

private:
	uint8_t* m_pUnk;
	const char* m_pFileName;
	uint8_t* m_pXURBuffer;     // 0x8
	TXUIResource* m_pResource; // 0xC
	TXUIResourceTRB* m_pNext;
	TXUIResourceTRB* m_pPrev;
};

class TXUI : public TSingleton<TXUI>
{
public:
	TXUI();

	TBOOL CreateScene(TXUIResource* a_pResource, uint32_t a_uiUnk);

	void AddResource(TXUIResourceTRB* a_pResourceTrb);
	void RemoveResource(TXUIResourceTRB* a_pResourceTrb);
	void SetDefaultFont(const char* a_pData);

	void SetSkin1(const char* a_szTRBFileName, const char* a_szXURFileName);
	void SetSkin2(const char* a_szTRBFileName, const char* a_szXURFileName);

	TXUIResource* FindResource(const char* a_sName);

	static TMemoryHeap* MemoryBlock() { return ms_pXUIMemoryBlock; }

public:
	static TMemoryHeap* ms_pXUIMemoryBlock;
	static TMemoryHeap* ms_pXUITRBMemoryBlock;
	static TTRB::t_MemoryFuncAlloc AssetTRBAllocator;
	static TTRB::t_MemoryFuncDealloc AssetTRBDeallocator;

public:
	char m_Str[128];                     // 0x0000
	TTRB m_FontTRB;                      // 0x0080
	TTRB m_TRB2;                         // 0x0094
	TTRB m_TRB3;                         // 0x00A8
	TXUIResource* m_pSkin1Resource;      // 0x00BC
	TXUIResource* m_pSkin2Resource;      // 0x00C0
	const char* m_pDefaultFont;          // 0x00C4
	const char* m_sDefaultFont;          // 0x00C8
	char m_sFontName;                    // 0x00CC
	TXUICanvas* m_pCanvas;               // 0x014C
	T2GUIContext* m_pContext;            // 0x015C
	T2GUIRenderer* m_pRenderer;          // 0x0160
	TXUIResourceTRB* m_pHeadTRBResource; // 0x0164
	TXUIAudio* m_pAudio;                 // 0x0170
};

TOSHI_NAMESPACE_END