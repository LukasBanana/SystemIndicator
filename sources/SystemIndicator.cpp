/*
 * SystemIndicator.cpp
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>

#ifdef _WIN32
#define NOMINMAX
#endif

#include <algorithm>


namespace SystemIndicator
{


typedef std::map<InformationEntry, std::string> EntryNameMap;

static void PrintBlank(std::ostream& stream, std::size_t& counter)
{
    if (counter > 0)
    {
        stream << std::endl;
        counter = 0;
    }
}

static void PrintEntry(
    std::ostream& stream, std::size_t& counter, std::size_t maxLen,
    const InformationEntryMap& entries, const std::string& entryName, const InformationEntry entry)
{
    InformationEntryMap::const_iterator it = entries.find(entry);
    if (it != entries.end())
    {
        stream << entryName << ':' << std::string(maxLen + 1 - entryName.size(), ' ') << it->second << std::endl;
        ++counter;
    }
}

static void PrintCacheEntry(
    std::ostream& stream, std::size_t& counter, std::size_t maxLen,
    const InformationEntryMap& entries, const std::string& entryName,
    const InformationEntry cacheEntry, const InformationEntry cacheSizeEntry)
{
    InformationEntryMap::const_iterator itC = entries.find(cacheEntry);
    InformationEntryMap::const_iterator itS = entries.find(cacheSizeEntry);
    if (itC != entries.end() && itS != entries.end())
    {
        stream << entryName << ':' << std::string(maxLen + 1 - entryName.size(), ' ') << itC->second << "x " << itS->second << " KB" << std::endl;
        ++counter;
    }
}

std::ostream& operator << (std::ostream& stream, InformationEntryMap entries)
{
    /* Setup entry names */
    EntryNameMap entryNames;

    entryNames[ ENTRY_OS_NAME            ] = "Operating System";
    entryNames[ ENTRY_COMPILER           ] = "Compiler";

    entryNames[ ENTRY_CPU_NAME           ] = "CPU Name";
    entryNames[ ENTRY_CPU_VENDOR         ] = "CPU Vendor";
    entryNames[ ENTRY_CPU_TYPE           ] = "CPU Type";
    entryNames[ ENTRY_CPU_ARCH           ] = "CPU Architecture";
    entryNames[ ENTRY_CPU_EXT            ] = "CPU Extensions";

    entryNames[ ENTRY_PROCESSORS         ] = "Processors";
    entryNames[ ENTRY_LOGICAL_PROCESSORS ] = "Logical Processors";
    entryNames[ ENTRY_PROCESSOR_SPEED    ] = "Processor Speed";

    entryNames[ ENTRY_L1CACHES           ] = "L1 Cache";
    entryNames[ ENTRY_L2CACHES           ] = "L2 Cache";
    entryNames[ ENTRY_L3CACHES           ] = "L3 Cache";

    entryNames[ ENTRY_TOTAL_MEMORY       ] = "Total Memory";
    entryNames[ ENTRY_FREE_MEMORY        ] = "Free Memory";

    /* Get longest available entry name */
    std::size_t maxLen = 0;

    for (EntryNameMap::const_iterator it = entryNames.begin(); it != entryNames.end(); ++it)
    {
        if (entries.find(it->first) != entries.end())
            maxLen = std::max(maxLen, it->second.size());
    }

    /* Extend some value */
    if (entries.find(ENTRY_PROCESSOR_SPEED) != entries.end())
        entries[ENTRY_PROCESSOR_SPEED] += " MHz";
    if (entries.find(ENTRY_TOTAL_MEMORY) != entries.end())
        entries[ENTRY_TOTAL_MEMORY] += " MB";
    if (entries.find(ENTRY_FREE_MEMORY) != entries.end())
        entries[ENTRY_FREE_MEMORY] += " MB";

    /* Write information to output stream */
    std::size_t num = 0;

    #define PRINT_ENTRY(ENTRY) \
        PrintEntry(stream, num, maxLen, entries, entryNames[ENTRY], ENTRY)
    #define PRINT_CACHE_ENTRY(CENTRY, SENTRY) \
        PrintCacheEntry(stream, num, maxLen, entries, entryNames[CENTRY], CENTRY, SENTRY)
    #define PRINT_BLANK \
        PrintBlank(stream, num)

    PRINT_ENTRY         ( ENTRY_OS_NAME                      );
    PRINT_ENTRY         ( ENTRY_COMPILER                     );
    PRINT_BLANK;
    PRINT_ENTRY         ( ENTRY_CPU_NAME                     );
    PRINT_ENTRY         ( ENTRY_CPU_VENDOR                   );
    PRINT_ENTRY         ( ENTRY_CPU_TYPE                     );
    PRINT_ENTRY         ( ENTRY_CPU_ARCH                     );
    PRINT_ENTRY         ( ENTRY_CPU_EXT                      );
    PRINT_BLANK;
    PRINT_ENTRY         ( ENTRY_PROCESSORS                   );
    PRINT_ENTRY         ( ENTRY_LOGICAL_PROCESSORS           );
    PRINT_ENTRY         ( ENTRY_PROCESSOR_SPEED              );
    PRINT_BLANK;
    PRINT_CACHE_ENTRY   ( ENTRY_L1CACHES, ENTRY_L1CACHE_SIZE );
    PRINT_CACHE_ENTRY   ( ENTRY_L2CACHES, ENTRY_L2CACHE_SIZE );
    PRINT_CACHE_ENTRY   ( ENTRY_L3CACHES, ENTRY_L3CACHE_SIZE );
    PRINT_BLANK;
    PRINT_ENTRY         ( ENTRY_TOTAL_MEMORY                 );
    PRINT_ENTRY         ( ENTRY_FREE_MEMORY                  );

    #undef PRINT_BLANK
    #undef PRINT_CACHE_ENTRY
    #undef PRINT_ENTRY

    return stream;
}


} // /namespace SystemIndicator



// ================================================================================
