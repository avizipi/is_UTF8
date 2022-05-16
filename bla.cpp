#include <string>
#include <iostream>
#include <random>

using namespace std;


int is_utf8(const char* str){
    int flagUtf8TwoBytes = 0, flagUtf8ThreeBytes = 0, flagUtf8FourBytes = 0;
    int i=0;
    if (str[i] == 0)
        return 1;
    while(str[i] != 0)
    {
        if ((str[i] & 0x80) == 0x80){  
            // leading 1
            if ((str[i] & 0xc0) == 0x80){
                // This mode is only for trailing bytes in utf 8 suppose to be 10xxxxxx.
                if (flagUtf8TwoBytes == 1){
                    return 2;
                }
                else if (flagUtf8ThreeBytes == 2){
                    return 3;
                }
                else if (flagUtf8ThreeBytes == 1){
                    flagUtf8ThreeBytes ++;
                }
                else if (flagUtf8FourBytes == 3){
                    return 4;
                }
                else if (flagUtf8FourBytes != 0){
                    flagUtf8FourBytes ++;
                }
                else {
                    return 0;
                }
            }
            else if (flagUtf8TwoBytes || flagUtf8ThreeBytes || flagUtf8FourBytes)
                // every trailing byte should be 10xxxxxx
                return 0;
            else if ((str[i] & 0xe0) == 0xc0){
                //maybe utf8 with two bytes. 11000000
                flagUtf8TwoBytes ++;
            }
            else if ((str[i] & 0xf0) == 0xe0){
                //maybe utf8 with three bytes 11100000
                flagUtf8ThreeBytes ++;
            }
            else if ((str[i] & 0xf8) == 0xf0){
                //maybe utf8 with four bytes 11110000
                flagUtf8FourBytes ++;
            }
            else{
                // non valid utf8
                return 0;
            }
               
        }
        else {
            // leading 0
            if (flagUtf8TwoBytes || flagUtf8ThreeBytes || flagUtf8FourBytes)
                // every trailing byte should be 10xxxxxx
                return 0;
            else
                return 1;
        }
        i++;
    }
    // encouter \0 before successful translation.
    return 0;
}

int is_luhn(std::string str){
    int nDigits = str.length();
    int shuma=0;
    int digit, place;
    for (int i = nDigits-1; i >= 0; i--)
        {
            digit = str[i] - '0';   // Char to number
            if  ((nDigits-1 - i) & 1)  // Digit 1, 3, 5 not 0, 2, 4 - "even digits" starting at 1
                if  (10 <= (digit <<= 1)) //  Double it, check >= 10
                    digit -= 9;           //  Same as summing the digits
            if(10 <= (shuma += digit)) //  to prevent overflow from shuma
                shuma -= 10;
        }

if (shuma)
std::cout << "number is not luhn\n";
else
std::cout << "number is luhn\n";
return shuma;
}

std::string get_random_string(int length, int from=32, int to=126){
    mt19937 generator{random_device{}()};
   //modify range according to your need "A-Z","a-z" or "0-9" or whatever you need.
    uniform_int_distribution<int> distribution{from, to};

    string rand_str(length, '\0');
    for(auto& dis: rand_str)
        dis = distribution(generator);
    return rand_str;
}


int main(){
    std::cout << "start\n";
    long long int num = 45801600;
    for(int i=0; i<10;i++){
        std::cout << i+num << " got " << is_luhn(std::to_string(i+num)) << std::endl;
    }
    num = 123456789123456789;
    for(int i=0; i<10;i++){
        std::cout << -i+num << " got " << is_luhn(std::to_string(-i+num)) << std::endl;
    }
    string str;
    str=get_random_string(15);
    cout << str.length() << ' ' << 15 << endl;
    get_random_string(1000);
    cout << str.length() << ' ' << 1000 << endl;
    get_random_string(10000);
    cout << str.length() << ' ' << 10000 << endl;
    return 0;
}