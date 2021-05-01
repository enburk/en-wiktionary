#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "aux_abc.h"
#include "aux_array.h"
#include "aux_string.h"
#include "aux_queue.h"
using namespace aux;

#include <set>
#include <map>
#include <stack>
#include <array>
#include <unordered_set>
#include <cassert>   
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <chrono>
#include <codecvt>

std::mutex print_mutex;
template <typename ... Args> void print (Args... args) {
    std::lock_guard lock{print_mutex};
    ((std::cout << args), ...);
    std::cout << std::endl;
}

template<class T>
std::ostream & operator << (std::ostream & out, const array<T> & a)
{
    for (const auto & e : a) out << e << "\n"; return out;
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

#include "_pass.h"
#include "_result.h"
#include "_common.h"
#include "_bracketer.h"
#include "_args.h"




