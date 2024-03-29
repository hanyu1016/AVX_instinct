//------------------------------------------------
// Ch02_01_fcpp.cpp
//------------------------------------------------
#include <immintrin.h>
#include <smmintrin.h>
#include "Ch02_01.h"
#include <iostream>

void AddI16_Iavx(XmmVal* c1, XmmVal* c2, const XmmVal* a, const XmmVal* b)
{
	__m128i a_vals = _mm_load_si128((__m128i*)a);
	__m128i b_vals = _mm_load_si128((__m128i*)b);
	__m128i c1_vals = _mm_add_epi16(a_vals, b_vals);
	__m128i c2_vals = _mm_adds_epi16(a_vals, b_vals);
	_mm_store_si128((__m128i*)c1, c1_vals);
	_mm_store_si128((__m128i*)c2, c2_vals);
}
void AddU16_Iavx(XmmVal* c1, XmmVal* c2, const XmmVal* a, const XmmVal* b)
{
	__m128i a_vals = _mm_load_si128((__m128i*)a);
	__m128i b_vals = _mm_load_si128((__m128i*)b);
	__m128i c1_vals = _mm_add_epi16(a_vals, b_vals);
	__m128i c2_vals = _mm_adds_epu16(a_vals, b_vals);
	_mm_store_si128((__m128i*)c1, c1_vals);
	_mm_store_si128((__m128i*)c2, c2_vals);
}

void MulI32a_Iavx(XmmVal* c, const XmmVal* a, const XmmVal* b)
{
	__m128i a_vals = _mm_load_si128((__m128i*)a);
	__m128i b_vals = _mm_load_si128((__m128i*)b);
	__m128i c_vals = _mm_mullo_epi32(a_vals, b_vals);
	_mm_store_si128((__m128i*)c, c_vals);
}

void Test_imageCompensation(BYTE *src, BYTE *dest, USHORT *nLightBalanceTbl)
{
	int temp[4] = {};
	for (size_t i = 0; i < 16; i += 4)
	{
		__m128i srcVec = _mm_cvtepu8_epi32(_mm_loadu_si128((__m128i*)src));
		__m128i tblVec = _mm_cvtepu16_epi32(_mm_loadu_si128((__m128i*)nLightBalanceTbl));

		__m128i res = _mm_mullo_epi32(srcVec, tblVec);
		__m128i shiftRes = _mm_srli_epi32(res, 12);

		//_mm_store_si128(reinterpret_cast<__m128i*>(&dest[i]),shiftRes);
		_mm_store_si128((__m128i*)dest, shiftRes);
		_mm_store_si128((__m128i*)dest, shiftRes);
		_mm_store_si128((__m128i*)dest, shiftRes);
		_mm_store_si128((__m128i*)dest, shiftRes);


		for (int i = 0; i < 4; ++i)
		{
			std::cout << (int)dest[i] << " ";
		}

		src += 4;

	}

	//	/*for (int j = 0; j < 4; j++)
	//	{
	//		_mm_store_si128((BYTE*)dest[i], shiftRes);
	//	}*/
	




}