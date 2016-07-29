/*
 * Helper.h
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __SI_HELPER_H__
#define __SI_HELPER_H__


#include <string>
#include <sstream>


namespace SystemIndicator
{


template <typename T>
std::string ToString(const T& value)
{
    std::stringstream s;
    s << value;
    return s.str();
}


} // /namespace SystemIndicator


#endif



// ================================================================================
