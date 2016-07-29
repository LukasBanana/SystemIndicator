/*
 * Test.cpp
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>
#include <cstdlib>
#include <iostream>


using namespace SystemIndicator;

void showEntry(const InformationEntryMap& entries, InformationEntry entry, const std::string& entryName)
{
    static const std::size_t maxLen = 30;
    InformationEntryMap::const_iterator it = entries.find(entry);
    if (it != entries.end())
    {
        std::cout << entryName << ": ";
        if (entryName.size() < maxLen)
            std::cout << std::string(maxLen - entryName.size(), ' ');
        std::cout << it->second << std::endl;
    }
}

#define SHOW(ENTRY) \
    showEntry(desc, ENTRY, #ENTRY)

int main()
{
    InformationEntryMap desc = QueryInformation();

    SHOW( ENTRY_OS_FAMILY );
    SHOW( ENTRY_OS_NAME );
    SHOW( ENTRY_COMPILER );

    SHOW( ENTRY_CPU_NAME );
    SHOW( ENTRY_CPU_VENDOR );
    SHOW( ENTRY_CPU_TYPE );
    SHOW( ENTRY_CPU_ARCH );
    SHOW( ENTRY_CPU_EXT );

    SHOW( ENTRY_PROCESSORS );
    SHOW( ENTRY_LOGICAL_PROCESSORS );
    SHOW( ENTRY_PROCESSOR_SPEED );

    SHOW( ENTRY_L1CACHES );
    SHOW( ENTRY_L1CACHE_SIZE );
    SHOW( ENTRY_L1CACHE_LINE_SIZE );

    SHOW( ENTRY_L2CACHES );
    SHOW( ENTRY_L2CACHE_SIZE );
    SHOW( ENTRY_L2CACHE_LINE_SIZE );

    SHOW( ENTRY_L3CACHES );
    SHOW( ENTRY_L3CACHE_SIZE );
    SHOW( ENTRY_L3CACHE_LINE_SIZE );

    SHOW( ENTRY_TOTAL_MEMORY );
    SHOW( ENTRY_FREE_MEMORY );

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}

