#pragma once
#include "Toshi2/T2DList.h"

namespace Toshi
{
	class T2GUITransform
	{
	public:
		static constexpr float FLOAT_QUALITY = 256;
		static constexpr float FLOAT_SCALE = 1 / FLOAT_QUALITY;

		class Rotation
		{
		public:
			Rotation(unsigned short x, unsigned short y) : m_X(x), m_Y(y)
			{
				
			}

			void SetX(float x)
			{
				m_X = PackFloat(x);
			}

			void SetY(float y)
			{
				m_Y = PackFloat(y);
			}

			float GetX() const
			{
				return UnpackFloat(m_X);
			}

			float GetY() const
			{
				return UnpackFloat(m_Y);
			}

		private:
			unsigned short m_X;
			unsigned short m_Y;
		};

	public:
		T2GUITransform() : m_Rot{ { PackFloat(1.0f), 0 }, { 0, PackFloat(1.0f) } }
		{
			
		}

		void Reset()
		{
			m_Rot[0] = { PackFloat(1.0f), 0 };
			m_Rot[1] = { 0, PackFloat(1.0f) };
		}

		void Rotate(float angle);
		void RotateTo(float angle);
		void PreMultiply(const T2GUITransform& transform);
		void PostMultiply(const T2GUITransform& transform);
		
		void Transform(TVector2& outVec, const TVector2& transformVec) const
		{
			outVec.x = m_Vec.x + m_Rot[0].GetX() * transformVec.x + m_Rot[1].GetX() * transformVec.y;
			outVec.y = m_Vec.y + m_Rot[0].GetY() * transformVec.x + m_Rot[1].GetY() * transformVec.y;
		}

		static void Multiply(T2GUITransform& outTransform, const T2GUITransform& a, const T2GUITransform& b);

		static constexpr float UnpackFloat(unsigned short value)
		{
			return value * FLOAT_SCALE;
		}

		static constexpr unsigned short PackFloat(float value)
		{
			return static_cast<unsigned short>(value / FLOAT_QUALITY);
		}

	private:
		Rotation m_Rot[2];
		TVector2 m_Vec;
	};

	/*
	class T2GUIElementNode : public T2DList<T2GUIElementNode>::Node
	{
	public:
		virtual ~T2GUIElementNode() = default;
	};*/

	class T2GUIElement : public T2DList<T2GUIElement>::Node
	{
	public:
		static constexpr float FLOAT_QUALITY = 6;
		static constexpr float FLOAT_SCALE = 1 / FLOAT_QUALITY;

		using t_PostRender = void(*)();

		typedef uint8_t State;
		enum State_ : State
		{
			State_Zero,
			State_Visible,
			State_Focused,
		};

	public:
		T2GUIElement();
		virtual ~T2GUIElement();

		virtual void Tick(float deltaTime);
		virtual bool IsPaused() const;
		virtual bool SkipRender();
		virtual void PreRender();
		virtual void Render();
		virtual void PostRender();
		virtual void GetDimensions(float& width, float& height);
		virtual float GetWidth();
		virtual float GetHeight();
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);
		virtual void SetDimensions(float width, float height);
		virtual void GetMins(float& x, float& y);
		virtual void GetMaxs(float& x, float& y);
		virtual void SetColour(uint32_t colour);
		virtual void SetAlpha(float alpha);
		virtual float GetAlpha();
		virtual void SetVisible(bool visible);
		virtual void SetFocus(bool focused);
		virtual void Unk(void* unk);
		virtual bool IsPointInside(const TVector2& point);
		virtual const TVector2& GetPivot();

		void SetTransform(float x, float y, float angle)
		{
			m_Transform.Reset();
			m_Transform.Rotate(angle);
			m_fX = x;
			m_fY = y;
		}

		static float UnpackFloat(unsigned short value)
		{
			return value * FLOAT_SCALE;
		}

		static unsigned short PackFloat(float value)
		{
			return static_cast<unsigned short>(value / FLOAT_QUALITY);
		}

	private:
		static constinit uint32_t s_uiGlobalVisMask;

	protected:
		void* m_Unk1;
		T2GUITransform m_Transform;
		float m_fX;
		float m_fY;
		T2DList<T2GUIElement> m_Children;
		unsigned short m_Width;
		unsigned short m_Height;
		uint32_t m_Color;
		t_PostRender m_cbPostRender;
		uint8_t m_Flags1;                     // 0x3B
		uint8_t m_Flags2;                     // 0x3B
		uint8_t m_Flags3;                     // 0x3B
		State m_State;                        // 0x3B
	};
}