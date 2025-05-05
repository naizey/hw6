#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here

        //split string into 5 groups of 6 becaue max string length is 28 chars. 
        //6x5=30, don't need more than 5 groups to get this
        //convert each group to base-36 int w[i]

        //the 5 digits of the string when in base-36 storing one digit of the 6 chars in the group
        unsigned long long w[5] = {0, 0, 0, 0, 0};

        //loop through groups
        int index = 4; //start at end 
        //loop through string k backwards in groups
        for(int i = k.size() - 1; i >= 0; i -= 6)
        {
            unsigned long long digit_place = 1; //the place value of the base-36 number 36^n
            unsigned long long converted_val = 0; //the value of the base-36 number from the string
            //loop through the 6 chars in the group
            for(int j = 0; j < 6 && i - j >= 0; ++j)
            {
                //get the character at the current index of the string k
                char letter = k[i - j];
                //convert the character to a number from 0-35
                converted_val += letterDigitToNumber(letter) * digit_place;
                //increment the place value for the next digit
                digit_place *= 36; //36^n
            }

            //go to the next group
            w[index] = converted_val; //store the converted value in the 5 digit array
            --index; //go to next group 
        }
        
        //want to multiply each group that contains one part of the string with a "weight"
        unsigned long long hash = 0;
        for(int i = 0; i < 5; ++i)
        {
            //encoding all groups of the string
            hash += w[i] * rValues[i];
        }

        //return a HASH_INDEX_T value = size_t value
        return static_cast<HASH_INDEX_T>(hash); 
        

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it

        //if letter A-Z, a-z convert to lower and subtract 'a' -> 0-25
        if(isalpha(letter))
        {
            return tolower(letter) - 'a';
        }
        //if digit '0'-'9' subtracat '0' and add 26 -> 26-35
        else if(isdigit(letter))
        {
            return letter - '0' + 26;
        }
        else
        {
            return 0;
        }

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
