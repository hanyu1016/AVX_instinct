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
	int n = 0;
	BYTE con[16] = {};

	__m128i packed_zero = _mm_setzero_si128();
	for (size_t i = 0; i < 32; i += 4)
	{
		__m128i srcVec_u32x4 = _mm_cvtepu8_epi32(_mm_loadu_si128((__m128i*)&src[i]));

		__m128i tblVec_u32x4 = _mm_cvtepu16_epi32(_mm_loadu_si128((__m128i*)nLightBalanceTbl));

		__m128i mulres_u32x4 = _mm_mullo_epi32(srcVec_u32x4, tblVec_u32x4);
		__m128i shiftRes = _mm_srli_epi32(mulres_u32x4, 12);

		__m128i val_u16 = _mm_packus_epi32(shiftRes, packed_zero);
		__m128i val_u8 = _mm_packus_epi16(val_u16, packed_zero);

		uint32_t s1 = _mm_extract_epi32(val_u8, 0);

		*((uint32_t*)&dest[i]) = s1;
	}
}

void u32x8_imageCompensation(BYTE* src, BYTE* dest, USHORT* nLightBalanceTbl)
{
	__m256i packed_zero = _mm256_setzero_si256();
	for (size_t i = 0; i < 32; i += 8)
	{
		__m256i src_vector = _mm256_cvtepu8_epi32(_mm_loadu_si128((__m128i*)& src[i]));
		__m256i tbl_vector = _mm256_cvtepu16_epi32(_mm_loadu_si128((__m128i*)nLightBalanceTbl));

		__m256i res_vector = _mm256_mullo_epi32(src_vector, tbl_vector);

		__m256i  shiftRes = _mm256_srli_epi32(res_vector, 12);
		__m256i val_u16 = _mm256_packus_epi32(shiftRes, packed_zero);
		__m256i permu_valu16 = _mm256_permute4x64_epi64(val_u16, 0b01011000);
		__m256i val_u8 = _mm256_packus_epi16(permu_valu16, packed_zero);

		uint64_t s1 = _mm256_extract_epi64(val_u8, 0);
		*((uint64_t*)&dest[i]) = s1;

	}
}