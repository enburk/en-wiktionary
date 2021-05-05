#include "main.hpp"

#include "0_00_unzip.h"
#include "0_10_unxml.h"
#include "0_20_unuse.h"
#include "0_30_unmeta.h"
#include "0_50_html.h"
#include "0_90_sort.h"
#include "0_99_save.h"

#include "1_00_load.h"
#include "1_10_redirect.h"
#include "1_20_modules.h"
#include "1_50_english.h"
#include "1_60_headers.h"
#include "1_99_save.h"

#include "2_00_load.h"
#include "2_10_brackets.h"
#include "2_20_lexforms1.h"
#include "2_20_lexforms2.h"
#include "2_20_lexforms22.h"
#include "2_99_save.h"

#include "3_00_load.h"
#include "3_10_lexforms1n.h"
#include "3_20_lexforms1v.h"
#include "3_30_lexforms1a.h"
#include "3_50_lexitems.h"
#include "3_50_multiline1.h"
/*
#include "2_51_template_label.h"
#include "2_70_tags.h"
#include "2_90_save.h"

#include "3_00_paragraphs.h"
#include "3_10_lex_items.h"
#include "3_20_rel_items.h"
#include "3_30_lex_notes.h"
#include "3_50_templates.h"

#include "4_00_paragraphs.h"
#include "4_10_lexforms1.h"
#include "3_40_altforms.h"
#include "4_20_lexforms2.h"
#include "4_90_out.h"
*/

Pass <pass2::entry, nothing> stop = [](auto & input, auto &)
{
    Time t0; for (auto && e: input) { (void)e; }
    Time t1; std::cout
    << std::endl << "total: " << t1-t0
    << std::endl; 
};

int main ()
{
    /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20210420-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Then run and wait...

    pass0::unzip  >> pass0::unxml >> pass0::unuse >>
    pass0::unmeta >> pass0::html  >> pass0::sort  >> pass0::save  >>

    pass1::load >> pass1::redirects >> pass1::modules >>
    pass1::lineup >> pass1::english >> pass1::headers >> pass1::save >>

    pass2::load >>
    pass2::brackets >>
    pass2::lexforms1 >> pass2::lexforms2 >> pass2::lexforms22 >>
    pass2::save >>

    pass3::load >> 
    pass3::lexforms1n >> pass3::lexforms1v >> pass3::lexforms1a >>
    pass3::lexitems   >>
    pass3::multiline1 >>

/*
    pass3::templates  >> //pass3::templates_label >>
    pass2::tags >>

    pass3::paragraphs >> pass3::lex_items >> pass3::rel_items >> pass3::lex_notes >>
    pass3::templates  >>

    pass4::paragraphs >> pass4::lexforms1 >> pass4::lexforms2 >> 
    pass4::out        >>
*/
    stop >> terminator; return 0;
}