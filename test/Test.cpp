/*
 * Test.cpp
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << SystemIndicator::QueryInformation();

    #ifdef _WIN32
    system("pause");
    #endif

    return 0;
}

