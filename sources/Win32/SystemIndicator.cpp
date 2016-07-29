/*
 * SystemIndicator.cpp (Win32)
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>
#include "ProcessorInfo.h"
#include <Windows.h>
#include <sstream>
#include <vector>
#include <array>

#ifndef _WIN32_WINNT_WIN10
#   define _WIN32_WINNT_WIN10 0x0A00
#endif
#if WINVER >= _WIN32_WINNT_WIN10
#   include <VersionHelpers.h>
#endif


namespace SystemIndicator
{


template <typename T>
static std::string ToString(const T& value)
{
    std::stringstream s;
    s << value;
    return s.str();
}

static DWORD QueryProcessorSpeed()
{
    /* Open register key */
    HKEY key;
    LSTATUS error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key);

    if (error != ERROR_SUCCESS)
        return 0;

    /* Query processor speed */
    DWORD speed = 0;
    DWORD size = sizeof(speed);
    error = RegQueryValueEx(key, "~MHz", 0, 0, reinterpret_cast<LPBYTE>(&speed), &size);

    /* Close register key */
    RegCloseKey(key);

    return speed;
}

static BOOL IsWoW64()
{
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    BOOL value = FALSE;

    LPFN_ISWOW64PROCESS PfnIsWoW64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");

    if (!PfnIsWoW64Process || !PfnIsWoW64Process(GetCurrentProcess(), &value))
        return FALSE;

    return value;
}

static std::string QueryCompilerVersion()
{
    #if defined(__clang__)

    return "clang " + std::string(__clang_version__);

    #elif defined(__GNUC__)
    
    return "GCC " + ToString(__GNUC__) + "." + ToString(__GNUC_MINOR__) + "." + ToString(__GNUC_PATCHLEVEL__);
    
    #elif defined(_MSC_VER)
    
    std::string version = "Microsoft ";
    
    #   if _MSC_VER == 600
    version += "C Compiler 6.0";            // C 6.0
    #   elif _MSC_VER == 700
    version += "C/C++ Compiler 7.0";        // C/C++ 7.0
    #   elif _MSC_VER == 800
    version += "Visual C++ 1.0";            // 1.0
    #   elif _MSC_VER == 900
    version += "Visual C++ 2.0";            // 2.0
    #   elif _MSC_VER == 1000
    version += "Visual C++ 4.0";            // 4.0
    #   elif _MSC_VER == 1100
    version += "Visual C++ 5.0";            // 5.0
    #   elif _MSC_VER == 1200
    version += "Visual C++ 6.0";            // 6.0
    #   elif _MSC_VER == 1300
    version += "Visual C++ .NET (7.0)";     // .NET
    #   elif _MSC_VER == 1310
    version += "Visual C++ 2003 (7.1)";     // 2003
    #   elif _MSC_VER == 1400
    version += "Visual C++ 2005 (8.0)";     // 2005
    #   elif _MSC_VER == 1500
    version += "Visual C++ 2008 (9.0)";     // 2008
    #   elif _MSC_VER == 1600
    version += "Visual C++ 2010 (10.0)";    // 2010
    #   elif _MSC_VER == 1700
    version += "Visual C++ 2012 (11.0)";    // 2012
    #   elif _MSC_VER == 1800
    version += "Visual C++ 2013 (12.0)";    // 2013
    #   elif _MSC_VER == 1900
    version += "Visual C++ 2015 (14.0)";    // 2015
    #   else
    version += "Visual C++";
    #   endif
    
    return version;
    
    #else
    
    return "Unknown compiler";
    
    #endif
}

static std::string QueryCPUArchitecture()
{
    /* Query common system info */
    SYSTEM_INFO sysInfo;
    ZeroMemory(&sysInfo, sizeof(sysInfo));
    GetNativeSystemInfo(&sysInfo);

    switch (sysInfo.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            return "AMD64 (x86-64)";
        case PROCESSOR_ARCHITECTURE_ARM:
            return "ARM";
        case PROCESSOR_ARCHITECTURE_IA64:
            return "IA-64 (Itanium-based)";
        case PROCESSOR_ARCHITECTURE_INTEL:
            return "IA-32 (x86)";
    }

    return "";
}

static void QueryMemoryStatus(std::string& total, std::string& avail)
{
    /* Query memory status */
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    static const DWORDLONG divMB = 1024*1024;

    total = ToString(memoryStatus.ullTotalPhys / divMB);
    avail = ToString(memoryStatus.ullAvailPhys / divMB);
}

static std::string QueryOSName()
{
    static const char* UNKNOWN_WIN_VER = "Microsoft Windows";

    /* Query OS version information */
    OSVERSIONINFOEX versionInfo;
    ZeroMemory(&versionInfo, sizeof(versionInfo));

    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    const BOOL osVersionInfoEx = GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&versionInfo));

    if (!osVersionInfoEx)
    {
        versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&versionInfo)))
            return UNKNOWN_WIN_VER;
    }

    switch (versionInfo.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_NT:
        {
            std::string name = "Microsoft ";

            if (versionInfo.dwMajorVersion == 5 && versionInfo.dwMinorVersion >= 1)
            {
                if (GetSystemMetrics(SM_SERVERR2) == 0)
                    name += "Windows Server 2003";
                else
                    name += "Windows XP";
            }
            else if (versionInfo.wProductType != VER_NT_WORKSTATION)
            {
                if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 0)
                    name += "Windows Server 2008";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 1)
                    name += "Windows Server 2008 R2";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 2)
                    name += "Windows Server 2012";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 3)
                    name += "Windows Server 2012 R2";
                else if (versionInfo.dwMajorVersion == 10 && versionInfo.dwMinorVersion == 0)
                    name += "Windows Server 2016 Technical Preview";
                else
                    return UNKNOWN_WIN_VER;
            }
            else
            {
                #if WINVER >= _WIN32_WINNT_WIN10

                if (IsWindows10OrGreater())
                    name += "Windows 10";
                else if (IsWindows8Point1OrGreater())
                    name += "Windows 8.1";
                else if (IsWindows8OrGreater())
                    name += "Windows 8";
                else if (IsWindows7OrGreater())
                    name += "Windows 7";
                else if (IsWindowsVistaOrGreater())
                    name += "Windows Vista";

                #else

                if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 0)
                    name += "Windows Vista";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 1)
                    name += "Windows 7";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 2)
                    name += "Windows 8";
                else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 3)
                    name += "Windows 8.1";
                else if (versionInfo.dwMajorVersion == 10 && versionInfo.dwMinorVersion == 0)
                    name += "Windows 10";
                else
                    return UNKNOWN_WIN_VER;

                #endif
            }

            if (osVersionInfoEx)
            {
                #ifdef VER_SUITE_ENTERPRISE
                
                if (versionInfo.wProductType == VER_NT_WORKSTATION)
                {
                    if (versionInfo.wSuiteMask & VER_SUITE_PERSONAL)
                        name += " Personal";
                    else
                        name += " Professional";
                }
                else if (versionInfo.wProductType == VER_NT_SERVER)
                {
                    if (versionInfo.wSuiteMask & VER_SUITE_DATACENTER)
                        name += " DataCenter";
                    else if (versionInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
                        name += " Advanced";
                }

                #endif
            }
            else
            {
                char szProductType[80];
                DWORD dwBufLen = 0;

                /* Query product type via register key */
                HKEY key;
                RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &key);

                RegQueryValueEx(key, "ProductType", 0, 0, reinterpret_cast<LPBYTE>(szProductType), &dwBufLen);
                RegCloseKey(key);

                /* Compare string case insensitive */
                if (lstrcmpi("WINNT", szProductType) == 0)
                    name += " Professional";
                if (lstrcmpi("LANMANNT", szProductType) == 0)
                    name += " Server";
                if (lstrcmpi("SERVERNT", szProductType) == 0)
                    name += " Advanced Server";
            }

            /* Display version, service pack (if any), and build number. */
            name += " (";

            std::string verCSD(versionInfo.szCSDVersion);
            if (!verCSD.empty())
                name += std::string(verCSD.begin(), verCSD.end()) + ", ";

            name += "Build " + ToString(versionInfo.dwBuildNumber & 0xFFFF) + ")";

            return name;
        }
        break;

        default:
        break;
    }

    return UNKNOWN_WIN_VER;
}

static void AddCPUExt(std::string& ext, bool enabled, const std::string& feature)
{
    if (enabled)
    {
        if (!ext.empty())
            ext += ", ";
        ext += feature;
    }
}

static std::string QueryCPUExtensions(const ProcessorInfo& cpuInfo)
{
    std::string ext;

    AddCPUExt( ext, cpuInfo.HasSSE     (), "SSE"         );
    AddCPUExt( ext, cpuInfo.HasSSE2    (), "SSE2"        );
    AddCPUExt( ext, cpuInfo.HasSSE3    (), "SSE3"        );
    AddCPUExt( ext, cpuInfo.HasSSSE3   (), "SSSE3"       );
    AddCPUExt( ext, cpuInfo.HasSSE4_1  (), "SSE4.1"      );
    AddCPUExt( ext, cpuInfo.HasSSE4_2  (), "SSE4.2"      );
    AddCPUExt( ext, cpuInfo.HasMMX     (), "MMX"         );
    AddCPUExt( ext, cpuInfo.HasExtMMX  (), "Ext. MMX"    );
    AddCPUExt( ext, cpuInfo.Has3DNow   (), "3DNow!"      );
    AddCPUExt( ext, cpuInfo.HasExt3DNow(), "Ext. 3DNow!" );
    AddCPUExt( ext, cpuInfo.HasHTT     (), "HTT"         );

    if (ext.empty())
        ext = "<none>";

    return ext;
}

struct LogicalCPUInfo
{
    LogicalCPUInfo() :
        numCores        ( 0 ),
        numLogicalCores ( 0 )
    {
    }

    struct Cache
    {
        Cache() :
            count   ( 0 ),
            size    ( 0 ),
            lineSize( 0 )
        {
        }

        unsigned int count;
        unsigned int size;
        unsigned int lineSize;
    };
    
    unsigned int            numCores;
    unsigned int            numLogicalCores;
    std::array<Cache, 3>    caches;
};

static unsigned int CountSetBits(ULONG_PTR bitMask)
{
    unsigned int count = 0;

    for (int i = 0; i < sizeof(ULONG_PTR)*8; ++i)
    {
        if ((bitMask & 0x1) != 0)
            ++count;
        bitMask >>= 1;
    }
    return count;
}

/*
Query information about physical- and logical processors,
and information about L1, L2, and L3 caches.
*/
static bool QueryLogicalProcessorInfo(LogicalCPUInfo& cpuInfo)
{
    /* Query buffer size */
    DWORD bufferSize = 0, byteOffset = 0;
    GetLogicalProcessorInformation(NULL, &bufferSize);

    /* Query processor information buffer */
    std::vector<char> buffer;
    buffer.resize(bufferSize);

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* infoBuffer = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION*>(buffer.data());
    if (!GetLogicalProcessorInformation(infoBuffer, &bufferSize))
        return false;

    /* Enumerate all cache information */
    for ( SYSTEM_LOGICAL_PROCESSOR_INFORMATION* info = infoBuffer;
          byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= bufferSize;
          byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION), ++info )
    {
        switch (info->Relationship)
        {
            case RelationProcessorCore:
            {
                ++cpuInfo.numCores;
                cpuInfo.numLogicalCores += CountSetBits(info->ProcessorMask);
            }
            break;

            case RelationCache:
            {
                const auto& cache = info->Cache;
                if (cache.Level >= 1 && cache.Level <= 3)
                {
                    auto& cacheInfo = cpuInfo.caches[cache.Level - 1];
                    ++cacheInfo.count;
                    cacheInfo.size = cache.Size;
                    cacheInfo.lineSize = cache.LineSize;
                }
            }
            break;

            default:
            break;
        }
    }

    return true;
}

InformationEntryMap QueryInformation()
{
    static const unsigned int divKB = 1024;

    /* Query processor information */
    ProcessorInfo cpuInfo;
    LogicalCPUInfo cpuInfo2;

    QueryLogicalProcessorInfo(cpuInfo2);

    /* Query memory status */
    std::string totalMem, availMem;
    QueryMemoryStatus(totalMem, availMem);

    /* Setup output entries */
    InformationEntryMap info;

    info[ ENTRY_OS_FAMILY          ] = "WIN32";
    info[ ENTRY_OS_NAME            ] = QueryOSName();
    info[ ENTRY_COMPILER           ] = QueryCompilerVersion();

    info[ ENTRY_CPU_NAME           ] = cpuInfo.GetName();
    info[ ENTRY_CPU_VENDOR         ] = cpuInfo.GetVendorName();
    info[ ENTRY_CPU_TYPE           ] = (IsWoW64() ? "64-Bit" : "32-Bit");
    info[ ENTRY_CPU_ARCH           ] = QueryCPUArchitecture();
    info[ ENTRY_CPU_EXT            ] = QueryCPUExtensions(cpuInfo);

    info[ ENTRY_PROCESSORS         ] = std::to_string(cpuInfo2.numCores);
    info[ ENTRY_LOGICAL_PROCESSORS ] = std::to_string(cpuInfo2.numLogicalCores);
    info[ ENTRY_PROCESSOR_SPEED    ] = std::to_string(QueryProcessorSpeed());

    info[ ENTRY_L1CACHES           ] = std::to_string(cpuInfo2.caches[0].count);
    info[ ENTRY_L1CACHE_SIZE       ] = std::to_string(cpuInfo2.caches[0].size / divKB);
    info[ ENTRY_L1CACHE_LINE_SIZE  ] = std::to_string(cpuInfo2.caches[0].lineSize);

    info[ ENTRY_L2CACHES           ] = std::to_string(cpuInfo2.caches[1].count);
    info[ ENTRY_L2CACHE_SIZE       ] = std::to_string(cpuInfo2.caches[1].size / divKB);
    info[ ENTRY_L2CACHE_LINE_SIZE  ] = std::to_string(cpuInfo2.caches[1].lineSize);

    if (cpuInfo2.caches[2].count > 0)
    {
        info[ ENTRY_L3CACHES          ] = std::to_string(cpuInfo2.caches[2].count);
        info[ ENTRY_L3CACHE_SIZE      ] = std::to_string(cpuInfo2.caches[2].size / divKB);
        info[ ENTRY_L3CACHE_LINE_SIZE ] = std::to_string(cpuInfo2.caches[2].lineSize);
    }

    info[ ENTRY_TOTAL_MEMORY       ] = totalMem;
    info[ ENTRY_FREE_MEMORY        ] = availMem;

    return info;
}


} // /namespace SystemIndicator



// ================================================================================
