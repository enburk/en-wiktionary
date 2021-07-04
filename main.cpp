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
#include "3_01_meta.h"
#include "3_10_lexforms1n.h"
#include "3_20_lexforms1v.h"
#include "3_30_lexforms1a.h"
#include "3_40_multiline.h"
#include "3_50_unquot.h"
#include "3_60_broken.h"
#include "3_70_tags.h"
#include "3_99_save.h"

#include "4_00_load.h"
#include "4_01_meta1.h"
#include "4_02_meta2.h"
#include "4_10_templates.h"
#include "4_20_templates_label.h"
#include "4_30_templates_mention.h"
#include "4_35_templates_names.h"
#include "4_37_templates_places.h"
#include "4_40_templates_taxon.h"
#include "4_50_templates_xfix.h"
#include "4_88_templates_zzz.h"
#include "4_89_templates_zz.h"
#include "4_90_templates_z.h"

#include "5_05_unlink1.h"
#include "5_10_lex_items.h"
#include "5_20_rel_items.h"
#include "5_30_lex_notes.h"
#include "5_50_altforms.h"
#include "5_80_unlink2.h"

#include "6_00_load.h"
#include "6_70_redirects.h"
#include "6_90_out.h"

Pass <pass6::entry, nothing> stop = [](auto & input, auto &)
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
    pass0::unmeta >> pass0::html  >> pass0::sort  >>
    pass0::save >>

    pass1::load >> pass1::redirects >> pass1::modules >>
    pass1::lineup >> pass1::english >> pass1::headers >>
    pass1::save >>

    pass2::load >>
    pass2::brackets >>
    pass2::lexforms1 >> pass2::lexforms2 >> pass2::lexforms22 >>
    pass2::save >>

    pass3::load >>
    pass3::meta >>
    pass3::lexforms1n >> pass3::lexforms1v >> pass3::lexforms1a >>
    pass3::multiline  >> pass3::unquot     >> pass3::broken >>
    pass3::tags >>
    pass3::save >>

    pass4::load >>
    pass4::meta1 >>
    pass4::meta2 >>
    pass4::templates >>
    pass4::templates_label >>
    pass4::templates_mention >>
    pass4::templates_names >>
    pass4::templates_places >>
    pass4::templates_taxon >>
    pass4::templates_xfix >>
    pass4::templates_zzz >>
    pass4::templates_zz >>
    pass4::templates_z >>

    pass5::unlink1 >>
    pass5::lex_items >>
    pass5::rel_items >>
    pass5::lex_notes >>
    pass5::altforms  >>
    pass5::unlink2 >>
    
    pass6::load >>
    pass6::redirects >> 
    pass6::out >>

    stop >> terminator;
    return 0;
}