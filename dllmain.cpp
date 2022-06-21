// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "kgm.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void __declspec(dllexport) kgmEncS(const char* fn, const char* type)
{
    encrypt(fn, type);
}

void __declspec(dllexport) kgmDecS(const char* fn)
{
    decrypt(fn);
}

void __declspec(dllexport) kgmEncD(const char* fn, const char* pswFn, const char* type)
{
    encrypt(fn, type, pswFn);
}

void __declspec(dllexport) kgmDecD(const char* fn, const char* pswFn)
{
    decrypt(fn, pswFn);
}
