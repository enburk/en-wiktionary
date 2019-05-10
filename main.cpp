#include <map>
#include <queue>
#include <array>
#include <vector>
#include <string>
#include <variant>
#include <cassert>   
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <chrono>

template <typename ... Args> void print (Args... args) {
//    static std::recursive_mutex mutex;
//    std::lock_guard lock{mutex};
    ((std::cout << args), ...);
    std::cout << std::endl;
}

const bool GENERATE_REPORTS = true;

#include "_aux.h"
#include "_ioqueue.h"
#include "_pass.h"
#include "_result.h"
#include "1.h"
#include "2.h"

Pass <pass2::entry, nothing> stop = [](auto & input, auto & output)
{
    auto start = std::chrono::high_resolution_clock::now ();

    for (auto && e : input) { ; }

    auto stop  = std::chrono::high_resolution_clock::now ();

    auto duration = stop - start;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration); duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration); duration -= seconds;

    std::cout << std::endl << "total: "
    << minutes.count ()  << " minutes "
    << seconds.count ()  << " seconds " << std::endl; 
};

int main ()
{
    /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20190320-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Then run and wait...

    //pass::start >>
        
    pass1::unzip >> pass1::unxml >>

    pass2::english >> pass2::headers >>
    
    stop >> terminator; return 0;
}