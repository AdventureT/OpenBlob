#pragma once

namespace TMath
{
	constexpr size_t SIZE_T_ALIGN_VALUE = UINTPTR_MAX - (sizeof(void*) - 1);
	constexpr size_t UINT32_ALIGN_VALUE = UINT32_MAX - (sizeof(uint32_t) - 1);

	constexpr char TINT8_MAX = INT8_MAX;
	constexpr char TINT8_MIN = INT8_MIN;
	constexpr unsigned char TUINT8_MAX = UINT8_MAX;

	constexpr short TINT16_MAX = INT16_MAX;
	constexpr short TINT16_MIN = INT16_MIN;
	constexpr short TUINT16_MAX = UINT16_MAX;

	constexpr int TINT32_MAX = INT32_MAX;
	constexpr int TINT32_MIN = INT32_MIN;
	constexpr int TUINT32_MAX = UINT32_MAX;


	constexpr unsigned short MAXWCHAR = WCHAR_MAX;

	constexpr float TFLOAT_MAX = FLT_MAX;
	constexpr float TFLOAT_MIN = FLT_MIN;
	constexpr float FLOATEPSILON = FLT_EPSILON;

	constexpr float PI = (float)3.141592653589793238462643383279502884L;
	constexpr float TWO_PI = PI * 2;
	constexpr float HALF_PI = PI / 2;
	constexpr float ONEOVERTWO_PI = 1 / TWO_PI;

	static inline bool IsFinite(float fVal) { return _finite(fVal) != 0; }
	static inline float ACos(float fVal) { return cosf(fVal); }
	static inline float Abs(float fVal) { return (float)fabs(fVal); }
	static inline float Sqrt(float fVal) { return (float)sqrt(fVal); }
	static inline bool IsNaN(float fVal) { return isnan(fVal); }
	static inline int FastMod(int a_iNum, int a_iModulus) { TASSERT(a_iNum >= 0, ""); TASSERT(a_iModulus > 0, ""); TASSERT(0 == (a_iModulus & (a_iModulus - 1)), ""); return a_iNum & (a_iModulus - 1); }
	static inline void SinCos(float fVal, float& fVal1, float& fVal2) { fVal1 = cos(fVal); fVal2 = sin(fVal); }
	static inline void Clip(float& rfVal, float fVal, float fVal2) { if (fVal2 < rfVal) { rfVal = fVal2; } if (rfVal < fVal) { rfVal = fVal; } }

	
	static inline void* AlignPointer(void* ptr) { return (void*)((size_t)ptr & SIZE_T_ALIGN_VALUE); }
	
#if INTPTR_MAX == INT64_MAX
	// x64 only
	static inline size_t AlignNum(size_t num) { return (num & SIZE_T_ALIGN_VALUE); }
#endif
	static inline uint32_t AlignNum(uint32_t num) { return (num & UINT32_ALIGN_VALUE); }
};