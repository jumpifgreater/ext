#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <sstream>

void print_line(const std::string& line, int time) {
    for (char c : line) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    std::cout << std::endl;
}

void log(const std::string& line, int type) {
    switch (type)
    {
    case 0:
        print_line("[...] " + line, 2);
        break;
    case 1:
        print_line("[+] " + line, 2);
        break;
    case 2:
        print_line("[-] " + line, 2);
        break;
    default:
        break;
    }

}

template<typename T>
std::string toHexString(T value, bool prefix = false, bool uppercase = false) {                                                                        
    std::stringstream stream;
    if (uppercase)
        stream << std::uppercase;

    if (prefix)
        stream << "0x";

    stream << std::hex << value;
    return stream.str();
}
