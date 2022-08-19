#pragma once
#include <cstdint>

namespace Toshi
{
	// TClassProps definitions
	typedef void* (*t_CreateTObject)();
	typedef void* (*t_CreateTObjectInPlace)(void*);
	typedef void  (*t_InitializeStatic)();
	typedef void  (*t_UninitializeStatic)();

	// RecurseTree definitions
	typedef void  (*t_ResurceTreeCheck)(class TClass*);
	typedef void  (*t_ResurceTreeAction)(class TClass*);

	// This is a separate struct because we want to do compile-time initialization
	struct TClassProps
	{
		const char* m_Name = 0;                                 // 0x00
		t_CreateTObject m_Create = 0;                           // 0x04
		t_CreateTObjectInPlace m_CreateInPlace = 0;             // 0x08
		t_InitializeStatic m_Initialize = 0;                    // 0x0C
		t_UninitializeStatic m_Uninitialize = 0;                // 0x10
		TClassProps* m_Parent = 0;                              // 0x14
		TClassProps* m_Previous = 0;                            // 0x18
		TClassProps* m_LastAttached = 0;                        // 0x1C
		uint32_t m_Version = 0;                                 // 0x20
		size_t m_Size = 0;                                      // 0x24
		uint32_t m_Unk = 0;                                     // 0x28
		bool m_Initialized = 0;                                 // 0x2C
	};

	class TClass : public TClassProps
	{
	public:
		TClass(const char* name, TClassProps* parent, uint32_t version, size_t size, t_CreateTObject fCreate, t_CreateTObjectInPlace m_CreateInPlace, t_InitializeStatic fInit, t_UninitializeStatic fUninit);
		~TClass();

		void Initialize();
		void RecurseTree();
		void RecurseTree2();

		inline bool IsInitialized() { return m_Initialized; }
		inline const char* GetName() const { return m_Name; }
		inline TClassProps& GetPropsRef() { return *this; }
		inline TClassProps* GetProps() { return this; }

		// todo: move this function away from this class
		static bool TryInitialize(TClass* tClass);
	};

	// TClass should be equal to TClassProps
	static_assert(sizeof(TClass) == sizeof(TClassProps));

	// Helper cast function
	static inline TClass* TClassFromProps(TClassProps& props)
	{
		return static_cast<TClass*>(&props);
	}
}

