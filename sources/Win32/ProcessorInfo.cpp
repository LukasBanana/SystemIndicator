/*
 * ProcessorInfo.cpp
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProcessorInfo.h"
#include <Windows.h>


namespace SystemIndicator
{


ProcessorInfo::ProcessorInfo() :
    hasSSE_     ( false ),
    hasSSE2_    ( false ),
    hasSSE3_    ( false ),
    hasSSSE3_   ( false ),
    hasSSE4_1_  ( false ),
    hasSSE4_2_  ( false ),
    hasMMX_     ( false ),
    hasExtMMX_  ( false ),
    has3DNow_   ( false ),
    hasExt3DNow_( false ),
    hasHTT_     ( false ),
    stepping_   ( 0     ),
    model_      ( 0     ),
    family_     ( 0     ),
    type_       ( 0     ),
    modelExt_   ( 0     ),
    familyExt_  ( 0     )
{
    ParseCPUID();
}

std::string ProcessorInfo::GetVendorName() const
{
         if (GetVendorID() == "AuthenticAMD") return "AMD";
    else if (GetVendorID() == "GenuineIntel") return "Intel";
    else if (GetVendorID() == "CyrixInstead") return "Cyrix";
    else if (GetVendorID() == "CentaurHauls") return "Centaur";
    else if (GetVendorID() == "RiseRiseRise") return "Rise";
    else if (GetVendorID() == "GenuineTMx86") return "Transmeta";
    else if (GetVendorID() == "SiS SiS SiS ") return "SiS";
    else if (GetVendorID() == "UMC UMC UMC ") return "UMC";
    else if (GetVendorID() == "VIA VIA VIA ") return "VIA";
    else if (GetVendorID() == "VMwareVMware") return "VMware";
    else                                      return GetVendorID();
}


/*
 * ======= Private: =======
 */

/*
This is the 'core' CPU info function:
-> call CPUID instruction and parse output
*/
void ProcessorInfo::ParseCPUID()
{
    #ifdef _MSC_VER

    char            name[49]            = { 0 }; // max 48 chars + null terminator
    char            vendor[13]          = { 0 }; // max 12 chars + null terminator
    unsigned int    cpu_feat_eax        = 0;
    unsigned int    cpu_feat_edx        = 0;
    unsigned int    cpu_feat_ecx        = 0;
    unsigned int    cpu_feat_ext_edx    = 0;

    __asm
    {
        mov     eax, 0x00000000             // first CPUID function, always supported (on reasonable cpu)
        cpuid                               // get info
        mov     DWORD PTR [vendor + 0], ebx // copy vendor id string
        mov     DWORD PTR [vendor + 4], edx
        mov     DWORD PTR [vendor + 8], ecx
        test    eax, eax                     
        jz      no_features                 // if eax is 0, no info will be available

        mov     eax, 0x00000001             // get extended info about cpu
        cpuid
        mov     [cpu_feat_eax], eax         // store data for later processing
        mov     [cpu_feat_edx], edx
        mov     [cpu_feat_ecx], ecx

        mov     eax, 0x80000000             // first extended function
        cpuid

        /* Test which extended functions are supported */
        cmp     eax, 0x80000001             // is eax < 0x80000001
        jb      no_features                 // yes -> jump to no_features label
        cmp     eax, 0x80000004             // is eax < 0x80000004
        jb      ext_feats_only              // yes -> jump to ext_feats_only label

        /* Get name of the cpu */
        mov     eax, 0x80000002
        cpuid
        mov     DWORD PTR [name + 0], eax
        mov     DWORD PTR [name + 4], ebx
        mov     DWORD PTR [name + 8], ecx
        mov     DWORD PTR [name + 12], edx

        mov     eax, 0x80000003
        cpuid
        mov     DWORD PTR [name + 16], eax
        mov     DWORD PTR [name + 20], ebx
        mov     DWORD PTR [name + 24], ecx
        mov     DWORD PTR [name + 28], edx

        mov     eax, 0x80000004
        cpuid
        mov     DWORD PTR [name + 32], eax
        mov     DWORD PTR [name + 36], ebx
        mov     DWORD PTR [name + 40], ecx
        mov     DWORD PTR [name + 44], edx

    ext_feats_only:
        // get extended features
        mov     eax, 0x80000001
        cpuid
        mov     [cpu_feat_ext_edx], edx

    no_features:
        // done
    } // __asm

    /* Now process data we got from cpu */
    name_    = std::string(name);
    vendor_  = std::string(vendor);

    /* Remove front spaces */
    while (!name_.empty() && name_.front() == ' ')
        name_.erase(name_.begin());

    stepping_       = cpu_feat_eax & 0xf;
    model_          = (cpu_feat_eax >> 4) & 0xf;
    family_         = (cpu_feat_eax >> 8) & 0xf;
    type_           = (cpu_feat_eax >> 12) & 0x3;
    modelExt_       = (cpu_feat_eax >> 16) & 0xf;
    familyExt_      = (cpu_feat_eax >> 20) & 0xff;

    hasMMX_         = ((cpu_feat_edx >> 23) & 0x1) != 0;
    hasSSE_         = ((cpu_feat_edx >> 25) & 0x1) != 0;
    hasSSE2_        = ((cpu_feat_edx >> 26) & 0x1) != 0;
    hasHTT_         = ((cpu_feat_edx >> 28) & 0x1) != 0;

    hasSSE3_        = (cpu_feat_ecx & 0x1) != 0;
    hasSSSE3_       = ((cpu_feat_ecx >> 9) & 0x1) != 0;
    hasSSE4_1_      = ((cpu_feat_ecx >> 19) & 0x1) != 0;
    hasSSE4_2_      = ((cpu_feat_ecx >> 20) & 0x1) != 0;

    hasExtMMX_      = ((cpu_feat_ext_edx >> 22) & 0x1) != 0;
    has3DNow_       = ((cpu_feat_ext_edx >> 31) & 0x1) != 0;
    hasExt3DNow_    = ((cpu_feat_ext_edx >> 30) & 0x1) != 0;

    #endif
}


} // /namespace SystemIndicator



// ================================================================================
