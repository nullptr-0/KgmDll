#pragma comment(lib,"KgmDll.lib")

void __declspec(dllexport) kgmEncS(const char* fn, const char* type);

void __declspec(dllexport) kgmDecS(const char* fn);

void __declspec(dllexport) kgmEncD(const char* fn, const char* pswFn, const char* type);

void __declspec(dllexport) kgmDecD(const char* fn, const char* pswFn);
