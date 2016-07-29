/*
 * ProcessorInfo.h
 * 
 * This file is part of the "SystemIndicator" project (Copyright (c) 2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __SI_PROCESSOR_INFO_H__
#define __SI_PROCESSOR_INFO_H__


#include <string>


namespace SystemIndicator
{


/**
Processor information query class.
\see http://www.gamedev.net/topic/438752-idenitfying-cpu-brand--model-c/
*/
class ProcessorInfo
{

    public:

        ProcessorInfo();

        //! Returns true if 'IA SSE' is supported.
        bool HasSSE() const
        {
            return hasSSE_;
        }

        //! Returns true if 'IA SSE2' is supported.
        bool HasSSE2() const
        {
            return hasSSE2_;
        }

        //! Returns true if 'IA SSE3' is supported.
        bool HasSSE3() const
        {
            return hasSSE3_;
        }

        //! Returns true if 'IA Supplemental SSE3' is supported.
        bool HasSSSE3() const
        {
            return hasSSSE3_;
        }

        //! Returns true if 'IA SSE4.1' is supported.
        bool HasSSE4_1() const
        {
            return hasSSE4_1_;
        }

        //! Returns true if 'IA SSE4.2' is supported.
        bool HasSSE4_2() const
        {
            return hasSSE4_2_;
        }

        //! Returns true if 'IA MMX' is supported.
        bool HasMMX() const
        {
            return hasMMX_;
        }

        //! Returns true if 'Extended MMX' is supported.
        bool HasExtMMX() const
        {
            return hasExtMMX_;
        }

        //! Returns true if 'AMD 3DNow!' is supported.
        bool Has3DNow() const
        {
            return has3DNow_;
        }

        //! Returns true if 'AMD Extended 3DNow!' is supported.
        bool HasExt3DNow() const
        {
            return hasExt3DNow_;
        }

        //! Returns true if HTT (hyper-threading) is supported.
        bool HasHTT() const
        {
            return hasHTT_;
        }

        const std::string& GetName() const
        {
            return name_;
        }

        const std::string& GetVendorID() const
        {
            return vendor_;
        }

        std::string GetVendorName() const;

        int GetStepping() const
        {
            return stepping_;
        }

        int GetModel() const
        {
            return model_;
        }

        int GetFamily() const
        {
            return family_;
        }

        int GetType() const
        {
            return type_;
        }

        int GetExtModel() const
        {
            return modelExt_;
        }

        int GetExtFamily() const
        {
            return familyExt_;
        }

    private:

        void ParseCPUID();

        bool        hasSSE_;
        bool        hasSSE2_;
        bool        hasSSE3_;
        bool        hasSSSE3_;
        bool        hasSSE4_1_;
        bool        hasSSE4_2_;
        bool        hasMMX_;
        bool        hasExtMMX_;
        bool        has3DNow_;
        bool        hasExt3DNow_;
        bool        hasHTT_;

        int         stepping_;
        int         model_;
        int         family_;
        int         type_;
        int         modelExt_;
        int         familyExt_;

        std::string name_;
        std::string vendor_;

};


} // /namespace SystemIndicator


#endif



// ================================================================================
