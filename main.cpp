#include "main.hpp"

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

#include "2_00_paragraphs.h"
#include "2_10_brackets.h"
#include "2_15_unlink.h"
#include "2_20_lexforms1.h"
#include "2_20_lexforms2.h"
#include "2_50_templates.h"
#include "2_70_tags.h"
#include "2_90_save.h"

#include "3_00_paragraphs.h"
#include "3_10_lex_items.h"
#include "3_20_rel_items.h"
#include "3_30_lex_notes.h"
#include "3_50_templates.h"

#include "4_00_paragraphs.h"
#include "4_10_lexforms1.h"
#include "4_20_lexforms2.h"
#include "4_90_out.h"

Pass <pass4::entry, nothing> stop = [](auto & input, auto &)
{
    Time t0; for (auto && e : input) { ; }
    Time t1; std::cout
    << std::endl << "total: " << t1-t0
    << std::endl; 
};

int main ()
{
    /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20200101-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Then run and wait...

    pass0::unzip >> pass0::unxml >> pass0::unuse >>
    pass0::meta  >> pass0::untag >> pass0::sort  >> pass0::rezip >> 

    pass1::unmeta  >> pass1::lineup  >>
    pass1::english >> pass1::headers >> pass1::cleanup >>

    pass2::paragraphs >> pass2::brackets  >> pass2::unlink    >>
    pass2::lexforms1  >> pass2::lexforms2 >> pass2::save >> pass2::templates >> pass2::tags >>

    pass3::paragraphs >> pass3::lex_items >> pass3::rel_items >> pass3::lex_notes >>
    pass3::templates  >>

    pass4::paragraphs >> pass4::lexforms1 >> pass4::lexforms2 >> 
    pass4::out        >>

    stop >> terminator; return 0;
}