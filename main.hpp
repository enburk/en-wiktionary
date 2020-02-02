#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "../ae/libraries/cpp/aux_abc.hpp"
#include "../ae/libraries/cpp/aux_array.hpp"
#include "../ae/libraries/cpp/aux_string.hpp"
using namespace aux;

#include <set>
#include <map>
#include <queue>
#include <stack>
#include <array>
#include <cassert>   
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <condition_variable>
#include <unordered_set>
#include <chrono>
#include <zlib.h>

std::mutex print_mutex;
template <typename ... Args> void print (Args... args) {
    std::lock_guard lock{print_mutex};
    ((std::cout << args), ...);
    std::cout << std::endl;
}

template<class T>
std::ostream & operator << (std::ostream & out, const array<T> & a)
{
    for (const auto & e : a) out << e << std::endl; return out;
}

struct Time
{
    decltype (std::chrono::high_resolution_clock::now ())
    stamp  =  std::chrono::high_resolution_clock::now ();
    friend auto operator - (Time t1, Time t0) noexcept
        { return t1.stamp - t0.stamp; }
};

std::ostream & operator << (std::ostream & out, std::chrono::high_resolution_clock::duration duration)
{
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration); duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration); duration -= seconds;

    out << minutes.count ()  << " minutes "
        << seconds.count ()  << " seconds "; 

    return out;
}

bool GENERATE_REPORTS = true;
bool UPDATING_REPORTS = false;

#include "_queue.h"
#include "_pass.h"
#include "_result.h"
#include "_common.h"
#include "_bracketer.h"




