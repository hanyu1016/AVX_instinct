// ConsoleApplication1.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "Ch02_01.h"
#include <intrin.h>

static void AddI16(void);
static void AddU16(void);
static void MulI32a(void);
#include <iostream>
#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __GNUC__

void __cpuid(int* cpuinfo, int info)
{
	__asm__ __volatile__(
		"xchg %%ebx, %%edi;"
		"cpuid;"
		"xchg %%ebx, %%edi;"
		:"=a" (cpuinfo[0]), "=D" (cpuinfo[1]), "=c" (cpuinfo[2]), "=d" (cpuinfo[3])
		: "0" (info)
	);
}

unsigned long long _xgetbv(unsigned int index)
{
	unsigned int eax, edx;
	__asm__ __volatile__(
		"xgetbv;"
		: "=a" (eax), "=d"(edx)
		: "c" (index)
	);
	return ((unsigned long long)edx << 32) | eax;
}

#endif

int main()
{
	/*AddI16();
	AddU16();
	return 0;*/

	/*MulI32a();
	return 0;*/


	// AVX instinct code
	
	//const int N = 32;
	//BYTE src[N] = { 10,2, 3, 4, 255, 6, 7, 8,9,10, 11, 12, 13, 14, 15, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 };
	//USHORT nLightBalanceTbl[N] = { 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096 };
	//BYTE dest[N] = {};
	//
	//Test_imageCompensation(src, dest, nLightBalanceTbl);
	////u32x8_imageCompensation(src, dest, nLightBalanceTbl);


	// CPUID code
	bool avxSupportted = false;

	int cpuinfo[4];
	__cpuid(cpuinfo, 1);

	avxSupportted = cpuinfo[2] & (1 << 28) || false;
	bool osxsaveSupported = cpuinfo[2] & (1 << 27) || false;
	if (osxsaveSupported && avxSupportted)
	{
		// _XCR_XFEATURE_ENABLED_MASK = 0
		unsigned long long xcrFeatureMask = _xgetbv(0);
		avxSupportted = (xcrFeatureMask & 0x6) == 0x6;
	}

	std::cout << "AVX:" << (avxSupportted ? 1 : 0) << std::endl;


	if (avxSupportted)
	{
		std::cout << "AVX:" << (avxSupportted ? 1 : 0) << std::endl;
	}
	else
	{
		return 0;
	}
	return 0;
}

static void AddI16(void)
{
	const char nl = '\n';
	XmmVal a, b, c1, c2;
	a.m_I16[0] = 10; b.m_I16[0] = 100;
	a.m_I16[1] = 200; b.m_I16[1] = -200;
	a.m_I16[2] = 30; b.m_I16[2] = 32760;
	a.m_I16[3] = -32766; b.m_I16[3] = -400;
	a.m_I16[4] = 50; b.m_I16[4] = 500;
	a.m_I16[5] = 60; b.m_I16[5] = -600;
	a.m_I16[6] = 32000; b.m_I16[6] = 1200;
	a.m_I16[7] = -32000; b.m_I16[7] = -950;
	AddI16_Iavx(&c1, &c2, &a, &b);
	/*std::cout << "\nResults for AddI16_Iavx - Wraparound Addition\n";
	std::cout << "a: " << a.ToStringI16() << nl;
	std::cout << "b: " << b.ToStringI16() << nl;
	std::cout << "c1: " << c1.ToStringI16() << nl;
	std::cout << "\nResults for AddI16_Iavx - Saturated Addition\n";
	std::cout << "a: " << a.ToStringI16() << nl;
	std::cout << "b: " << b.ToStringI16() << nl;
	std::cout << "c2: " << c2.ToStringI16() << nl;*/
}
static void AddU16(void)
{
	const char nl = '\n';
	XmmVal a, b, c1, c2;
	a.m_U16[0] = 10; b.m_U16[0] = 100;
	a.m_U16[1] = 200; b.m_U16[1] = 200;
	a.m_U16[2] = 300; b.m_U16[2] = 65530;
	a.m_U16[3] = 32766; b.m_U16[3] = 40000;
	a.m_U16[4] = 50; b.m_U16[4] = 500;
	a.m_U16[5] = 20000; b.m_U16[5] = 25000;
	a.m_U16[6] = 32000; b.m_U16[6] = 1200;
	a.m_U16[7] = 32000; b.m_U16[7] = 50000;
	AddU16_Iavx(&c1, &c2, &a, &b);
	/*std::cout << "\nResults for AddU16_Iavx - Wraparound Addition\n";
	std::cout << "a: " << a.ToStringU16() << nl;
	std::cout << "b: " << b.ToStringU16() << nl;
	std::cout << "c1: " << c1.ToStringU16() << nl;
	std::cout << "\nResults for AddU16_Iavx - Saturated Addition\n";
	std::cout << "a: " << a.ToStringU16() << nl;
	std::cout << "b: " << b.ToStringU16() << nl;
	std::cout << "c2: " << c2.ToStringU16() << nl;*/
}

static void MulI32a(void)
{
	const char nl = '\n';
	XmmVal a, b, c;
	a.m_I32[0] = 10; b.m_I32[0] = -500;
	a.m_I32[1] = 3000; b.m_I32[1] = 100;
	a.m_I32[2] = -2000; b.m_I32[2] = -12000;
	a.m_I32[3] = 4200; b.m_I32[3] = 1000;
	MulI32a_Iavx(&c, &a, &b);
	std::cout << "\nResults for MulI32a_Iavx\n";
	for (size_t i = 0; i < 4; i++)
	{
		std::cout << "a[" << i << "]: " << std::setw(10) << a.m_I32[i] << " ";
		std::cout << "b[" << i << "]: " << std::setw(10) << b.m_I32[i] << " ";
		std::cout << "c[" << i << "]: " << std::setw(10) << c.m_I32[i] << nl;
	}
}



