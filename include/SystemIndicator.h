/*
 * SystemIndicator.h
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __SI_INCLUDE_H__
#define __SI_INCLUDE_H__


#include <map>
#include <string>


namespace SystemIndicator
{


//! Entry enumeration of all available system information.
enum InformationEntry
{
    ENTRY_OS_FAMILY,            //!< Identifier of the operating system family, either "WIN32", "LINUX", or "MACOS".
    ENTRY_OS_NAME,              //!< Name and version of the operating system, e.g. "Microsoft Windows 10 Home Edition Service Pack 1 (6100)".
    ENTRY_COMPILER,             //!< Name and version of the compiler used to compile this library, e.g. "Microsoft VisualC++ 2015 (14.0)".

    ENTRY_CPU_NAME,             //!< CPU name, e.g. "Intel(R) Core(TM) i7-3770K CPU @ 3.50GHz".
    ENTRY_CPU_VENDOR,           //!< CPU vendor, e.g. "Intel".
    ENTRY_CPU_TYPE,             //!< CPU type, e.g. "64-Bit".
    ENTRY_CPU_ARCH,             //!< CPU architecture, e.g. "x86-64".
    ENTRY_CPU_EXT,              //!< CPU extensions, e.g. "SSE, SSE2, SSE3".

    ENTRY_PROCESSORS,           //!< Number of processors.
    ENTRY_LOGICAL_PROCESSORS,   //!< Number of logical processors (this is larger than 'ENTRY_PROCESSORS' if hyper-threading is supported).
    ENTRY_PROCESSOR_SPEED,      //!< Processor speed (in MHz).

    ENTRY_L1CACHES,             //!< Number of L1 caches.
    ENTRY_L1CACHE_SIZE,         //!< Size of L1 cache (in KBs).
    ENTRY_L1CACHE_LINE_SIZE,    //!< Line size of the L1 cache (in Bytes).

    ENTRY_L2CACHES,             //!< Number of L2 caches.
    ENTRY_L2CACHE_SIZE,         //!< Size of L2 cache (in KBs).
    ENTRY_L2CACHE_LINE_SIZE,    //!< Line size of the L2 cache (in Bytes).

    ENTRY_L3CACHES,             //!< Number of L3 caches.
    ENTRY_L3CACHE_SIZE,         //!< Size of L3 cache (in KBs).
    ENTRY_L3CACHE_LINE_SIZE,    //!< Line size of the L3 cache (in Bytes).

    ENTRY_TOTAL_MEMORY,         //!< Total physical memory (in MBs).
    ENTRY_FREE_MEMORY,          //!< Free physical memory (in MBs).
};


/**
\brief Returns type of the "QueryInformation" function.
\remarks The keys of this map are from the enumeration type 'InformationEntry' and the values are from the type 'std::string'.
*/
typedef std::map<InformationEntry, std::string> InformationEntryMap;


/**
\brief Main function to query system information.
\return Map of all information entries available for the host system.
*/
InformationEntryMap QueryInformation();


} // /namespace SystemIndicator


#endif



// ================================================================================
