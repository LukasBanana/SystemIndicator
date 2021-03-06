/*
 * LinuxSystemIndicator.cpp
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>
#include <unistd.h>
#include <sys/utsname.h>
#include "../Helper.h"


namespace SystemIndicator
{


static std::string QueryCompilerVersion()
{
    #if defined(__clang__)

    return "clang " + std::string(__clang_version__);

    #elif defined(__GNUC__)
    
    return "GCC " + ToString(__GNUC__) + "." + ToString(__GNUC_MINOR__) + "." + ToString(__GNUC_PATCHLEVEL__);
    
    #else
    
    return "";
    
    #endif
}

static void QueryKernelInfo(std::string& version, std::string& machine)
{
    /* Get Linux version by POSIX function 'uname' */
    utsname name;
    if (uname(&name) == 0)
    {
        version = std::string(name.sysname) + ' ' + std::string(name.release) + " (" + std::string(name.version) + ")";
        machine = std::string(name.machine);
    }
    else
    {
        version = "Linux";
        machine = "";
    }
}

static std::string QueryProcessorCount()
{
    return ToString(sysconf(_SC_NPROCESSORS_ONLN));
}

static void AddEntry(InformationEntryMap& entries, const InformationEntry entry, const std::string& value)
{
    if (!value.empty())
        entries[entry] = value;
}

InformationEntryMap QueryInformation()
{
    /* Query kernel information */
    std::string version, machine;
    QueryKernelInfo(version, machine);
    
    /* Setup output entries */
    InformationEntryMap info;
    
    AddEntry(info, ENTRY_OS_FAMILY, "LINUX");
    AddEntry(info, ENTRY_OS_NAME, version);
    AddEntry(info, ENTRY_COMPILER, QueryCompilerVersion());
    AddEntry(info, ENTRY_CPU_ARCH, machine);
    AddEntry(info, ENTRY_PROCESSORS, QueryProcessorCount());
    
    
    return info;
}


} // /namespace SystemIndicator



// ================================================================================
