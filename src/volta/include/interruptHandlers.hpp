/*
Defines Exception Handling
*/

#ifndef INTERRUPT_HANDLERS_HPP
#define INTERRUPT_HANDLERS_HPP

namespace Volta
{
    void ConfigureExceptions();

    void handleExceptionDivideError();
    void handleExceptionDebug();
    void handleExceptionNMI();
    void handleExceptionOverflow();
    void handleExceptionInvalidOpcode();
    void handleExceptionGeneralProtection();
    void handleExceptionPageFault();
};
#endif // INTERRUPT_HANDLERS_HPP