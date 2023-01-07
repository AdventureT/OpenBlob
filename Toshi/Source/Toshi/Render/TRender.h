#pragma once
#include "Toshi/Math/Math.h"
#include "Toshi/Core/TStack.h"
#include "Toshi/Core/TRefCounted.h"
#include "Toshi/Render/TResource.h"
#include "Toshi/Render/TAnimation.h"

namespace Toshi
{
	class TRender;

	class TRenderContext
	{
		typedef uint32_t FLAG;
		enum FLAG_ : FLAG
		{
			FLAG_DIRTY = BITFIELD(0),
			FLAG_FOG   = BITFIELD(1)
		};

		struct PROJECTIONPARAMS
		{
			TVector2 m_Centre;  // 0x0
			TVector2 m_Proj;    // 0x8
		};

	protected:
		inline void SetDirty(bool enable) { enable ? m_eFlags |= FLAG_DIRTY : m_eFlags &= ~FLAG_DIRTY; }
		inline void SetFlag(FLAG flag, bool enable) { enable ? m_eFlags |= flag : m_eFlags &= ~flag; }

		inline void EnableFog(bool enable) { enable ? m_eFlags |= FLAG_FOG : m_eFlags &= ~FLAG_FOG; }
		inline bool IsFogEnabled() const { return m_eFlags & FLAG_FOG; }
		inline bool IsDirty() const { return m_eFlags & FLAG_DIRTY; }
	
	public:
		TRenderContext() { m_eFlags = 0; }
		TRenderContext(TRender&) { m_eFlags = 0; }

		virtual void SetModelViewMatrix(const TMatrix44& a_rMatrix)
		{
			m_eFlags |= 0x300;
			m_mModelViewMatrix = a_rMatrix;
			m_eFlags &= ~0x300;
		}

		virtual void SetWorldViewMatrix(const TMatrix44& a_rMatrix)
		{
			m_eFlags |= 0x100;
			m_mModelViewMatrix = a_rMatrix;
			m_eFlags &= ~0x100;
		}

	private:
		FLAG m_eFlags;                          // 0x8
		static PROJECTIONPARAMS m_sProjParams;  // 0x30 (it cannot have an offset if it's static)
		TMatrix44 m_mModelViewMatrix;           // 0x40
		TMatrix44 m_mWorldViewMatrix;           // 0x80
	};

	class TRender :
		public TGenericClassDerived<TRender, TObject, "TRender", TMAKEVERSION(1, 0), false>,
		public TRefCounted,
		public TSingleton<TRender>
	{
	public:
		static constexpr size_t MAXNUMRESOURCES = 6000;
		
		typedef uint8_t SYSRESOURCE;
		enum SYSRESOURCE_ : SYSRESOURCE
		{
			SYSRESOURCE_1,
			SYSRESOURCE_2,
			SYSRESOURCE_3,
			SYSRESOURCE_4,
			SYSRESOURCE_5,
			SYSRESOURCE_6,
			SYSRESOURCE_7,
			SYSRESOURCE_8,
			SYSRESOURCE_9,
			SYSRESOURCE_10,
			SYSRESOURCE_11,
			SYSRESOURCE_12,
			SYSRESOURCE_NUMOF
		};

		struct DisplayParams
		{

		};

	public:
		TRender();

		virtual ~TRender() { }                                 // 0x08 at vftable
		virtual bool CreateDisplay(DisplayParams* params) = 0; // 0x0C at vftable
		virtual bool DestroyDisplay() = 0;                     // 0x10 at vftable
		virtual float Update(float deltatime);                 // 0x14 at vftable
		virtual void BeginScene();                             // 0x18 at vftable
		virtual void EndScene();                               // 0x1C at vftable
		virtual void* GetCurrentDevice() = 0;                  // 0x20 at vftable
		virtual DisplayParams* GetCurrentDisplayParams() = 0;  // 0x24 at vftable
		virtual bool Create();                                 // 0x28 at vftable
		virtual bool Destroy();                                // 0x2C at vftable
		virtual void DumpStats();                              // 0x30 at vftable
		virtual void GetScreenOffset(TVector2* pOutVec);       // 0x34 at vftable
		virtual void SetScreenOffset(TVector2* pVec);          // 0x38 at vftable
		virtual void SetLightDirectionMatrix(TMatrix44* pMat); // 0x3C at vftable
		virtual void SetLightColourMatrix(TMatrix44* pMat);    // 0x40 at vftable
		virtual bool CreateSystemResources();                  // 0x44 at vftable
		virtual void DestroySystemResources();                 // 0x44 at vftable

		// Creates display and returns true if success
		bool CreateDisplay();

		// Destroys resource
		void DestroyResource(TResource* resource);
		
		// Destroys resource recursively
		void DestroyResourceRecurse(TResource* resource);

		// Returns system resource
		TResource* GetSystemResource(SYSRESOURCE systemResource)
		{
			TASSERT(systemResource >= 0 && systemResource < SYSRESOURCE_NUMOF, "Unknown resource");
			return m_SystemResources[systemResource];
		}

		// Sets resource explicit
		void SetResourceExplicit(TResource* resource, SYSRESOURCE systemResource)
		{
			TASSERT(systemResource >= 0 && systemResource < SYSRESOURCE_NUMOF, "Unknown resource");
			TASSERT(m_SystemResources[systemResource] == TNULL, "This resource has already been assigned!");
			m_SystemResources[systemResource] = resource;
		}

		// Creates resource and returns it
		TResource* CreateResource(TClass* pClass, char* name, TResource* parent);

		// Sets new render context and returns the old one
		TRenderContext* SetCurrentRenderContext(TRenderContext* a_pRenderContext)
		{
			auto previousContext = m_pRenderContext;
			m_pRenderContext = a_pRenderContext;
			return previousContext;
		}

		bool IsInScene() { return m_bInScene; }
		bool IsCreated() { return m_bCreated; }
		bool IsDisplayCreated() { return m_bDisplayCreated; }
		TRenderContext* GetCurrentRenderContext() { return m_pRenderContext; }
	
	public:
		/*
		* Platform specific methods
		* Define them in TRender_{Renderer}.cpp
		*/
		TRenderContext* CreateRenderContext();

	protected:
		uint32_t m_Unk1;                                 // 0x04
		bool m_bIsEnabled;                               // 0x08
		bool m_bInScene;                                 // 0x09
		bool m_bCreated = false;                         // 0x0A
		bool m_bDisplayCreated;                          // 0x0B
		TVector2 m_ScreenOffset;                         // 0x0C
		uint32_t m_Unk2;                                 // 0x14
		TRenderContext* m_pRenderContext;                // 0x18
		uint32_t m_Unk5;                                 // 0x1C
		TResource* m_SystemResources[SYSRESOURCE_NUMOF]; // 0x20
		TMatrix44 m_LightDirection;                      // 0x50
		TMatrix44 m_LightColour;                         // 0x90
		//void* m_aSysResources;            // 0x20
		//TNodeList
		void* m_DummyResource;                           // 0xE4 (idk what it is)
		TNodeTree<TResource> m_Resources;                // 0xE8
		size_t m_ResourceCount = 0;                      // 0x100
		uint32_t m_Unk4 = 0;                             // 0x104
		bool m_HasDyingResources;                        // 0x108
		TKeyframeLibraryManager m_KeyframeManager;       // 0x10C
		TStack<TMatrix44, 20> m_Transforms;              // 0x118
	};
}


