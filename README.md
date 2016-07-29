SystemIndicator
===============

Simple Cross-Platform C++98 Library to Query System Information.


Overview
-------

* **Version**: 1.00 Alpha
* **License**: [3-Clause BSD License](https://github.com/LukasBanana/SystemIndicator/blob/master/LICENSE.txt)


Supported Platforms
-------------------

- Windows
- Linux
- MacOS


Example
-------

```cpp
#include <SystemIndicator.h>
#include <iostream>

int main()
{
	std::cout << SystemIndicator::QueryInformation();
	return 0;
}
```
