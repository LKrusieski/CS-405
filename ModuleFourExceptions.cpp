// ModuleFourExceptions.cpp : This file contains the 'main' function.Program execution begins and ends there.
// Larry E Krusieski, CS-405 SNHU 29 March 2026

#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>

using namespace std;

// Custom exception class derived from exception.
// This to create, throw, and catch a custom exception.

class CustomApplicationException : public exception
{
private:
    string message;

public:
    explicit CustomApplicationException(const string& msg) : message(msg)
    {
    }

    // Override what() so the custom mesage can be displayed
    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

bool do_even_more_custom_application_logic()
{
    // TODO: Throw any standard exception

    cout << "Running Even More Custom Application Logic." << endl;

    //  runtime_error is a standard exception
    throw runtime_error("Standard Exception from do_even_more_custom_application_logic().");

    return true;
}
void do_custom_application_logic()
{
    // TODO: Wrap the call to do_even_more_custom_application_logic()
    //  with an exception handler that catches exception, displays
    //  a message and the exception.what(), then continues processing

    cout << "Running Custom Application Logic." << endl;

    try {

        if (do_even_more_custom_application_logic())
        {
            cout << "Even More Custom Application Logic Succeeded." << endl;
        }
    }

    catch (const exception& ex) 
    {
        cout << "Caught exception in do_custom_applicaton_logic: " << ex.what() << endl;
    }

    // TODO: Throw a custom exception derived from exception
    //  and catch it explictly in main

    throw CustomApplicationException("Custom application exception throw from do_custom_application_logic().");

    cout << "Leaving Custom Application Logic." << endl;

}

float divide(float num, float den)
{
    // TODO: Throw an exception to deal with divide by zero errors using
    //  a standard C++ defined exception

    if (den == 0.0f)
    {
        throw runtime_error("Divide by zero error.");
    }

    return (num / den);
}

void do_division() noexcept
{
    //  TODO: create an exception handler to capture ONLY the exception thrown
    //  by divide.

    float numerator = 10.0f;
    float denominator = 0;
    
    try 
    {
        auto result = divide(numerator, denominator);
        cout << "divide(" << numerator << ", " << denominator << ") = " << result << endl;
    }
        
    
    catch(const runtime_error& ex)
    {
        cout << "Caught divide exception in do_division: " << ex.what() << endl;
    }
  
}

int main()
{
    cout << "Exceptions Tests!" << endl;

    // TODO: Create exception handlers that catch (in this order):
    //  your custom exception
    //  exception
    //  uncaught exception 
    //  that wraps the whole main function, and displays a message to the console.
    
    try
    {
        do_division();
        do_custom_application_logic();
    }

    catch (const CustomApplicationException& ex)
    {
        cout << "Caught CustomApplicationException in main: " << ex.what() << endl;
    }

    catch (const exception& ex)
    {
        cout << "Caught exception in main: " << ex.what() << endl;
    }

    catch (...)
    {
        cout << "Caught an unknown exception in main." << endl;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu