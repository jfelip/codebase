
#include <GenericMessages.hpp>
#include <CScopedTimer.hpp>
#include <StringHelpers.hpp>


int main() {

    ///////////////////////////////////////////////////////////////////////////////////
    // Generic messages and scoped timer
    ///////////////////////////////////////////////////////////////////////////////////


    _SCOPED_TIMER_("Execution took: ");

    _GENERIC_MESSAGE_ ("This is a generic message");
    _GENERIC_ERROR_   ("This is a generic ERROR message");
    _GENERIC_DEBUG_   ("This is a generic DEBUG message");
    _GENERIC_WARNING_ ("This is a generic WARNING message");

    try
    {
        throw std::exception();
    }
    catch (const std::exception& e)
    {
        _GENERIC_EXCEPTION_(e);
    }


    ///////////////////////////////////////////////////////////////////////////////////
    // String helpers
    ///////////////////////////////////////////////////////////////////////////////////

    std::string str = "  Hello, World!  ";
    std::string ltrimstr = StringHelpers::ltrimCopy(str);
    std::string rtrimstr = StringHelpers::rtrimCopy(str);
    std::string trimstr  = StringHelpers::trimCopy(str);

    std::cout << "Original string: " << str << "EOF" << std::endl;
    std::cout << "Trim from start: " << ltrimstr << "EOF" << std::endl;
    std::cout << "Trim from end  : " << rtrimstr << "EOF" << std::endl;
    std::cout << "Trim from both : " << trimstr << "EOF" << std::endl;
    return 0;
}