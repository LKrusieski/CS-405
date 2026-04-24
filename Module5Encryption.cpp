//Larry E Krusieski CS-405 5 April 2026

// Module5Encryption.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include <stdexcept>

using namespace std;

/// <summary>
/// encrypt or decrypt a source string using the provided key
/// </summary>
/// <param name="source">input string to process</param>
/// <param name="key">key to use in encryption / decryption</param>
/// <returns>transformed string</returns>
string encrypt_decrypt(const string& source, const string& key)
{
    // get lengths now instead of calling the function every time.
    // this would have most likely been inlined by the compiler, but design for perfomance.
    const auto key_length = key.length();
    const auto source_length = source.length();

    // assert that our input data is good
    assert(key_length > 0);
    assert(source_length > 0);

    string output = source;

    // loop through the source string char by char
    for (size_t i = 0; i < source_length; ++i)
    {
        // XOR each source character with the matching key character.
        // The modulo keeps the key index within the key length so that
        // the password repeats across the full source string.
        output[i] = source[i] ^ key[i % key_length];
    }

    // our output length must equal our source length
    assert(output.length() == source_length);

    // return the transformed string
    return output;
}

string read_file(const string& filename)
{
    // Open the input file in binary mode so the program preserves
    // all characters exactly as they appear in the source file.
    ifstream input_file(filename, ios::binary);

    if (!input_file.is_open())
    {
        throw runtime_error("Unable to open input file: " + filename);
    }

    // Read the entire file into a string using a string stream.
    ostringstream buffer;
    buffer << input_file.rdbuf();

    if (input_file.fail() && !input_file.eof())
    {
        throw runtime_error("An error occurred while reading file: " + filename);
    }

    const string file_text = buffer.str();

    // Validate that the file was not empty.
    if (file_text.empty())
    {
        throw runtime_error("Input file is empty: " + filename);
    }

    return file_text;
}

string get_student_name(const string& string_data)
{
    string student_name;

    // find the first newline
    size_t pos = string_data.find('\n');
    // did we find a newline
    if (pos != string::npos)
    { // we did, so copy that substring as the student name
        student_name = string_data.substr(0, pos);
    }
    else
    {
        // If there is no newline, treat the whole string as the name.
        student_name = string_data;
    }

    return student_name;
}

void save_data_file(const string& filename, const string& student_name, const string& key, const string& data)
{
    // Open the destination file in binary mode so encrypted data is written exactly.
    ofstream output_file(filename, ios::binary);

    if (!output_file.is_open())
    {
        throw runtime_error("Unable to open output file: " + filename);
    }

    // Get the current date and format it as yyyy-mm-dd.
    time_t now = time(nullptr);
    tm local_time{};

#ifdef _WIN32
    localtime_s(&local_time, &now);
#else
    local_time = *localtime(&now);
#endif

    // file format
    // Line 1: student name
    // Line 2: timestamp (yyyy-mm-dd)
    // Line 3: key used
    // Line 4+: data
    output_file << student_name << '\n';
    output_file << put_time(&local_time, "%Y-%m-%d") << '\n';
    output_file << key << '\n';
    output_file << data;

    if (!output_file.good())
    {
        throw runtime_error("An error occurred while writing file: " + filename);
    }
}

int main()
{
    try
    {
        cout << "Encyption Decryption Test!" << endl;

        // input file format
        // Line 1: <students name>
        // Line 2: <Lorem Ipsum Generator website used> https://pirateipsum.me/ (could be https://www.lipsum.com/ or one of https://www.shopify.com/partners/blog/79940998-15-funny-lorem-ipsum-generators-to-shake-up-your-design-mockups)
        // Lines 3+: <lorem ipsum generated with 3 paragraphs> 
        //  Fire in the hole bowsprit Jack Tar gally holystone sloop grog heave to grapple Sea Legs. Gally hearties case shot crimp spirits pillage galleon chase guns skysail yo-ho-ho. Jury mast coxswain measured fer yer chains man-of-war Privateer yardarm aft handsomely Jolly Roger mutiny.
        //  Hulk coffer doubloon Shiver me timbers long clothes skysail Nelsons folly reef sails Jack Tar Davy Jones' Locker. Splice the main brace ye fathom me bilge water walk the plank bowsprit gun Blimey wench. Parrel Gold Road clap of thunder Shiver me timbers hempen halter yardarm grapple wench bilged on her anchor American Main.
        //  Brigantine coxswain interloper jolly boat heave down cutlass crow's nest wherry dance the hempen jig spirits. Interloper Sea Legs plunder shrouds knave sloop run a shot across the bow Jack Ketch mutiny barkadeer. Heave to gun matey Arr draft jolly boat marooned Cat o'nine tails topsail Blimey.

        const string file_name = "inputdatafile.txt";
        const string encrypted_file_name = "encrypteddatafile.txt";
        const string decrypted_file_name = "decrytpteddatafile.txt";
        const string source_string = read_file(file_name);
        const string key = "SNHUCS405Krusieski";

        // get the student name from the data file
        const string student_name = get_student_name(source_string);

        // encrypt sourceString with key
        const string encrypted_string = encrypt_decrypt(source_string, key);

        // save encrypted_string to file
        save_data_file(encrypted_file_name, student_name, key, encrypted_string);

        // decrypt encryptedString with key
        const string decrypted_string = encrypt_decrypt(encrypted_string, key);

        // save decrypted_string to file
        save_data_file(decrypted_file_name, student_name, key, decrypted_string);

        cout << "Read File: " << file_name << " - Encrypted To: " << encrypted_file_name
            << " - Decrypted To: " << decrypted_file_name << endl;

        // students submit input file, encrypted file, decrypted file, source code file, and key used
    }
    catch (const exception& ex)
    {
        cerr << "Program error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu