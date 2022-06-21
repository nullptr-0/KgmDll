# KgmDll
Kgm Encryption & Decryption Dynamic Link Library

# How to use
```
Other operations
1: Place the KgmDll.lib in yout project directory
2: Add `#include "KgmDll.hpp"` to your source
Other operations
```

# Functions
Name|Usage|Params
--|:--|:--|:
kgmEncS|Encrypt, Single(No seperated key file)|Param1 (const char* fn): Name of file to encrypt, Param2 (const char* type): Encryption type("kgm" or "vpr")
kgmDecS|Decrypt, Single(No seperated key file)|Param (const char* fn): Name of file to decrypt
kgmEncD|Encrypt, Dual(With seperated key file)|Param1 (const char* fn) & Param3 (const char* type): see kgmEncS, Param2 (const char* pswFn): Name of file to store the key
kgmDecD|Decrypt, Dual(With seperated key file)|Param1 (const char* fn): see kgmDecS, Param2 (const char* pswFn): Name of file that stores the key