#pragma once
#include "TClass.h"
#include "TMemory.h"
#include "Toshi/Typedefs.h"

#define TOSHI_CLASS_DEFINE(CLASSNAME) \
public: \
	virtual Toshi::TClass* Class() { return &s_Class; } \
	static void* CreateTObject() { return new CLASSNAME; } \
	static void* CreateTObjectInPlace(void* block) { return new (block) CLASSNAME(); } \
	static Toshi::TClass s_Class;

#define TOSHI_CLASS_STATIC_DEFINE(CLASSNAME) \
public: \
	virtual Toshi::TClass* Class() { return Toshi::TClassFromProps(s_Class); } \
	static void* CreateTObject() { return new CLASSNAME; } \
	static void* CreateTObjectInPlace(void* block) { return new (block) CLASSNAME(); } \
	static constinit Toshi::TClassProps s_Class;

#define TOSHI_CLASS_DERIVED_INITIALIZE(CLASSNAME, PARENT, VER) \
Toshi::TClass CLASSNAME::s_Class(#CLASSNAME, &PARENT::s_Class, VER, sizeof(CLASSNAME), CLASSNAME::CreateTObject, CLASSNAME::CreateTObjectInPlace, 0, 0);

#define TOSHI_CLASS_INITIALIZE(CLASSNAME, VER) \
Toshi::TClass CLASSNAME::s_Class(#CLASSNAME, TNULL, VER, sizeof(CLASSNAME), CLASSNAME::CreateTObject, CLASSNAME::CreateTObjectInPlace, 0, 0);

namespace Toshi
{
	class TObjectInterface
	{
	public:
		virtual TClass* Class() = 0;
		virtual void Destroy();
		virtual ~TObjectInterface();

		// Operators
		static inline void* operator new(size_t size) { return tmalloc((uint32_t)size); }
		static inline void* operator new(size_t size, void* at) { return at; }

		static inline void operator delete(void* block) { tfree(block); }
		static inline void operator delete(void* block, void* at) { tfree(block); }
	};

	class TObject : public TObjectInterface
	{
		TOSHI_CLASS_STATIC_DEFINE(TObject)
	};
}

