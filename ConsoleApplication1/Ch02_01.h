#pragma once
#include "XmmVal.h"

extern void AddI16_Iavx(XmmVal* c1, XmmVal* c2, const XmmVal* a, const XmmVal* b);
extern void AddU16_Iavx(XmmVal* c1, XmmVal* c2, const XmmVal* a, const XmmVal* b);
extern void Test_imageCompensation(BYTE* src, BYTE* dest ,USHORT* nLightBalanceTbl);
extern void u32x8_imageCompensation(BYTE * src, BYTE * dest, USHORT * nLightBalanceTbl);
extern void MulI32a_Iavx(XmmVal* c, const XmmVal* a, const XmmVal* b);
