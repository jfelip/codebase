//
// Created by jfelip on 10/18/17.
//

#ifndef JFELIP_CODEBASE_GENERICMESSAGES_H
#define JFELIP_CODEBASE_GENERICMESSAGES_H



#define _ACTIVATE_GENERIC_MESSAGES_
#ifdef _ACTIVATE_GENERIC_MESSAGES_
#define _GENERIC_ERROR_(msg)            CGenericMessages::OnGenericError(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#define _GENERIC_EXCEPTION_(Exception)  CGenericMessages::OnGenericException(Exception,__PRETTY_FUNCTION__,__FILE__,__LINE__);
#define _GENERIC_WARNING_(msg)          CGenericMessages::OnGenericWarning(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#define _GENERIC_MESSAGE_(Content)      CGenericMessages::Message(Content);
#define _GENERIC_DEBUG_(msg)        CGenericMessages::OnGenericDebug(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#else
#define _GENERIC_ERROR_(msg) {};
	#define _GENERIC_EXCEPTION_(Exception) {};
	#define _GENERIC_WARNING_(msg) {};
	#define _GENERIC_MESSAGE_(Content) {};
	#define _GENERIC_DEBUG_(Content) {};
#endif

#include <string>
#include <sstream>
#include <iostream>

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

#endif //JFELIP_CODEBASE_GENERICMESSAGES_H
