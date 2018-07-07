//
// Created by jfelip on 10/18/17.
//

#ifndef JFELIP_CODEBASE_GENERICMESSAGES_H
#define JFELIP_CODEBASE_GENERICMESSAGES_H

#include <string>
#include <sstream>
#include <iostream>

#ifdef _ACTIVATE_GENERIC_MESSAGES_
#ifndef _GENERIC_ERROR_
#define _GENERIC_ERROR_(msg)            CGenericMessages::OnGenericError(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#endif
#ifndef _GENERIC_EXCEPTION_
    #define _GENERIC_EXCEPTION_(Exception)  CGenericMessages::OnGenericException(Exception,__PRETTY_FUNCTION__,__FILE__,__LINE__);
#endif
#ifndef _GENERIC_WARNING_
    #define _GENERIC_WARNING_(msg)          CGenericMessages::OnGenericWarning(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#endif
#ifndef _GENERIC_MESSAGE_
    #define _GENERIC_MESSAGE_(Content)      CGenericMessages::Message(Content);
#endif
#ifndef _GENERIC_DEBUG_
    #define _GENERIC_DEBUG_(msg)        CGenericMessages::OnGenericDebug(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#endif
#else
#define _GENERIC_ERROR_(msg) {};
	#define _GENERIC_EXCEPTION_(Exception) {};
	#define _GENERIC_WARNING_(msg) {};
	#define _GENERIC_MESSAGE_(Content) {};
	#define _GENERIC_DEBUG_(Content) {};
#endif

#ifndef _GENERIC_MESSAGES_INCLUDED_
#define _GENERIC_MESSAGES_INCLUDED_
class CGenericMessages {
public:

    static void OnGenericError(const char* pCallerFunction,
                               const char* pCallerFile, const uint SourceCodeLine, const std::string msg = "")
    {
        std::stringstream Stream;

        Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : " << SourceCodeLine << " ] {"<< msg <<"}";

        std::cout << "\033[31m" << "[ Error ] " << Stream.str() << "\033[0m" << std::endl;

    }

    static void OnGenericException(const std::exception& Exception,
                                   const char* pCallerFunction, const char* pCallerFile,
                                   const uint SourceCodeLine)
    {
        std::stringstream Stream;

        Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : " << SourceCodeLine << " ] {" << Exception.what() << "}";

        std::cout << "\033[36m" << "[ Excep ] " << Stream.str() << " \033[0m" << std::endl;

    }

    static void OnGenericWarning(const char* pCallerFunction,
                                 const char* pCallerFile, const uint SourceCodeLine, const std::string& msg = "")
    {
        std::stringstream Stream;
        Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : " << SourceCodeLine << " ] {"<< msg <<"}";
        std::cout << "\033[33m" << "[Warning] " << Stream.str() << "\033[0m" << std::endl;
    }

    static void OnGenericDebug(const char* pCallerFunction,
                                 const char* pCallerFile, const uint SourceCodeLine, const std::string& msg = "")
    {
        std::stringstream Stream;
        Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : " << SourceCodeLine << " ] {"<< msg <<"}";
        std::cout << "\033[35m" << "[ Debug ] " << Stream.str() << "\033[0m" << std::endl;
    }

    static void Message(const std::string& Content)
    {
        std::stringstream Stream;
        Stream << "\033[34m" << "[Message] [" << Content << "] " << "\033[0m";
        std::cout << Stream.str() << std::endl;
    }
};
#endif

#endif //JFELIP_CODEBASE_GENERICMESSAGES_H
