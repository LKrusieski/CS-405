// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Larry E Krusieski CS-405 SNHU

#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    cout << "Buffer Overflow Example" << endl;

    const string account_number = "CharlieBrown42";
    char user_input[20];

    cout << "Enter a value: ";

    // limit the input length to the size of user_input
    cin.width(sizeof(user_input));
    cin >> user_input;

    // check if the cin failed
    if (cin.fail())
    {
        cin.clear();
    }

    // if cin is not at the end of the file and the next char is not a newline char, display the error message
    if (!cin.eof() && cin.peek() != '\n')
    {
        cout << "Error: input was too long and was rejected to prevent a buffer overflow." << endl;

        // discard any chars in the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        user_input[0] = '\0';
    }

    cout << "You entered: " << user_input << endl;
    cout << "Account Number = " << account_number << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
