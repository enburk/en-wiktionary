#define _CRT_SECURE_NO_DEPRECATE
#include <set>
#include <map>
#include <queue>
#include <stack>
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
#include <unordered_set>
#include <chrono>
#include <zlib.h>

inline std::mutex print_mutex;
template <typename ... Args> void print (Args... args) {
    std::lock_guard lock{print_mutex};
    ((std::cout << args), ...);
    std::cout << std::endl;
}

const bool GENERATE_REPORTS = true;

#include "_aux.h"
#include "_bracketer.h"
#include "_ioqueue.h"
#include "_pass.h"
#include "_result.h"

inline std::unordered_map<str,str> redirects;
inline std::unordered_map<str,str> redirects_templates;
inline std::unordered_map<str,str> templates;

std::unordered_set<str> lexical_items
{
    "noun", "pronoun", "proper noun", "verb", "adjective", "adverb", "numeral", "number", "article", "particle", 
    "preposition", "postposition", "determiner", "conjunction", "interjection",
    "abbreviation", "initialism", "contraction", "acronym", 
    "letter", "symbol", "punctuation mark", "diacritical mark",
    "suffix", "prefix", "infix", "affix", "interfix",
    "phrase", "prepositional phrase", "proverb", 
};
std::unordered_set<str> related_items
{
    "alternative forms", "abbreviations",
    "synonyms", "antonyms", "hypernyms", "hyponyms", "meronyms", "holonyms", "troponyms",
    "derived terms", "related terms", "coordinate terms",
    "see also", 
};
std::unordered_set<str> lexical_notes
{
    "pronunciation", "etymology",
    "usage notes", "trivia",
};


#include "1.h"
#include "2.h"
#include "3.h"

Pass <pass3::entry, nothing> stop = [](auto & input, auto & output)
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
    /// Gzip it to: enwiktionary-pages-articles.xml.gz
    /// Then run and wait...

    pass1::unzip >> pass1::unxml >> pass1::skip >> pass1::untag >>

    pass2::english >> pass2::headers >> pass2::cleanup >>

    pass3::paragraphs >> pass3::brackets >>
        
        // pass3::templating >> 

//  pass3::gather >> 

//  pass2::unquote >> 
    
    stop >> terminator; return 0;
}