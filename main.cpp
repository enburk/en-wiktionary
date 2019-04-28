#include <map>
#include <queue>
#include <array>
#include <vector>
#include <string>
#include <cassert>   
#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <chrono>
using namespace std::chrono_literals;

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
//#include "_report.h"
#include "_result.h"
#include "1.h"

int main ()
{
    /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20190320-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Then run and wait...

    //pass::start >>
        
    pass1::unzip >> pass1::unxml >>
    
    pass1::stop >>
        
    terminator;
    return 0;
}