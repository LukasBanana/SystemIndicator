SystemIndicator
===============

Simple C++98 Library to Query System Information.


Overview
-------

* **Version**: 1.00 Alpha
* **License**: [3-Clause BSD License](https://github.com/LukasBanana/SystemIndicator/blob/master/LICENSE.txt)


Supported Platforms
-------------------

- Windows
- *more comming ...*


Example
-------

```cpp
#include <SystemIndicator.h>
#include <iostream>

using namespace SystemIndicator;

void showEntry(const InformationEntryMap& entries, InformationEntry entry, const std::string& entryName)
{
	InformationEntryMap::const_iterator it = entries.find(entry);
	if (it != entries.end())
		std::cout << entryName << ": " << it->second << std::endl;
}

#define SHOW(ENTRY) showEntry(desc, ENTRY, #ENTRY)

int main()
{
	InformationEntryMap info = QueryInformation();
	
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
	
	return 0;
}
```
