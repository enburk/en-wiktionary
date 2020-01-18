#include "main.hpp"

str digit = "0123456789";
str Latin = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
str latin = "abcdefghijklmnopqrstuvwxyz";
str alnum = Latin + latin + digit;

std::unordered_map<str,
std::unordered_map<str,
std::unordered_map<str, str>>> formmap;

#include "0_00_unzip.h"
#include "0_10_unxml.h"
#include "0_20_unuse.h"
#include "0_30_meta.h"
#include "0_30_untag.h"
#include "0_90_sort.h"

#include "1_00_unmeta.h"
#include "1_10_english.h"
#include "1_20_headers.h"
#include "1_50_cleanup.h"

Pass <pass1::entry, nothing> stop = [](auto & input, auto &)
{
    Time t0; for (auto && e : input) { ; }
    Time t1; std::cout
    << std::endl << "total: " << t1-t0
    << std::endl; 
};

int main ()
{
    /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20190320-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Gzip it to: enwiktionary-pages-articles.xml.gz
    /// Then run and wait...

    pass0::unzip >> pass0::unxml >> pass0::unuse >>
    pass0::meta  >> pass0::untag >> pass0::sort  >> pass0::rezip >> 

    pass1::unmeta  >> pass1::lineup  >>
    pass1::english >> pass1::headers >> pass1::cleanup >>


    // pass1::unzip >> pass1::unxml >> pass1::skip >> pass1::untag >>
    // 
    // pass2::english >> pass2::headers >> pass2::cleanup >>
    // 
    // pass3::paragraphs >> pass3::brackets >> pass3::forms >>
    //     
    //     // pass3::templating >> 
    // 
    //     //  pass2::unquote >> 
    
    stop >> terminator; return 0;
}