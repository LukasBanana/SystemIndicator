/*
 * MacOSSystemIndicator.mm
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <SystemIndicator.h>


namespace SystemIndicator
{


InformationEntryMap QueryInformation()
{
    /* Setup output entries */
    InformationEntryMap info;

    info[ ENTRY_OS_FAMILY ] = "MACOS";

    return info;
}


} // /namespace SystemIndicator



// ================================================================================
