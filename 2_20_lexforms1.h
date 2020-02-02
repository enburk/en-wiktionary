#pragma once
#include "2.h"
namespace pass2
{
    namespace lexforms1_internal
    {
        static std::unordered_map<str, str> list_of_forms =
        {
            { "en-adj"                    , "adjective"               },
            { "en-adjective"              , "adjective"               },
            { "en-adv"                    , "adverb"                  },
            { "en-adverb"                 , "adverb"                  },
            { "en-noun"                   , "noun"                    },
            { "en-plural noun"            , "noun"                    },
            { "en-plural-noun"            , "noun"                    },
            { "en-proper noun"            , "proper noun"             },
            { "en-proper-noun"            , "proper noun"             },
            { "en-proper_noun"            , "proper noun"             },
            { "en-prop"                   , "proper noun"             },
            { "en-verb"                   , "verb"                    },

            { "en-abbr"                   , "abbreviation"            },
            { "en-acronym"                , "acronym"                 },
            { "en-con"                    , "conjunction"             },
            { "en-conjunction"            , "conjunction"             },
            { "en-cont"                   , "contraction"             },
            { "en-contraction"            , "contraction"             },
            { "en-det"                    , "determiner"              },
            { "en-diacritical mark"       , "diacritical mark"        },
            {"mul-diacritical mark"       , "diacritical mark"        },
            { "en-initialism"             , "initialism"              },
            { "en-intj"                   , "interjection"            },
            { "en-interj"                 , "interjection"            },
            { "en-interjection"           , "interjection"            },
            { "en-letter"                 , "letter"                  },
            { "en-number"                 , "number"                  },
            { "en-particle"               , "particle"                },
            { "en-part"                   , "particle"                },
            { "en-phrase"                 , "phrase"                  },
            { "en-PP"                     , "prepositional phrase"    },
            { "en-prep phrase"            , "prepositional phrase"    },
            { "en-pnoun"                  , "proper noun"             },
            { "en-prefix"                 , "prefix"                  },
            { "en-prep"                   , "preposition"             },
            { "en-preposition"            , "preposition"             },
            { "en-pron"                   , "pronoun"                 },
            { "en-pronoun"                , "pronoun"                 },
            { "en-proverb"                , "proverb"                 },
            { "en-prov"                   , "proverb"                 },
            { "en-punctuation mark"       , "punctuation mark"        },
            {"mul-punctuation mark"       , "punctuation mark"        },
            { "en-suffix"                 , "suffix"                  },
            { "en-symbol"                 , "symbol"                  },
            {"mul-symbol"                 , "symbol"                  },
        };
    }

    str proceed_lexforms1 (str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);

        auto it = lexforms1_internal::list_of_forms.find(args.name);
        if (it == lexforms1_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_item = it->second;
        str report = header;
        str arg = args.body;

        if (header != lexical_item)
        {
            report += " wrong";
        }
        else
        if (header == "adjective" || header == "adverb")
        {
            if (arg == "") arg = "+"; forms = arg;

            if (arg == "+"      ) { ; } else
            if (arg == "-"      ) { ; } else
            if (arg == "?"      ) { ; } else
            if (arg == "er"     ) { ; } else
            if (arg == "more"   ) { ; } else
            if (arg == "er|more") { ; } else
            if (arg == "more|er") { ; } else
                
                report += " quest"; report = "+ " + report;
        }
        else
        if (header == "noun" || header == "proper noun")
        {
            if (arg == "") arg = "+"; forms = arg;

            if (arg == "+"      ) { ; } else
            if (arg == "-"      ) { ; } else
            if (arg == "?"      ) { ; } else
            if (arg == "~"      ) { ; } else
            if (arg == "s"      ) { ; } else
            if (arg == "-|s"    ) { ; } else

            if (args.complexity <= 2
            && (args.complexity <= 2 || args[1].contains_only(str::one_of(alnum+" -'")))
            && (args.complexity <= 1 || args[0].contains_only(str::one_of(alnum+" -'")))) { ; } else
                
                report += " quest"; report = "+ " + report;
        }
        else
        if (header == "verb")
        {
            if (arg == "") arg = "+"; forms = arg;

            if (arg == "+"      ) { ; } else

            if (args.complexity <= 4
            && (args.complexity <= 4 || args[3].contains_only(str::one_of(alnum+" -'")))
            && (args.complexity <= 3 || args[2].contains_only(str::one_of(alnum+" -'")))
            && (args.complexity <= 2 || args[1].contains_only(str::one_of(alnum+" -'")))
            && (args.complexity <= 1 || args[0].contains_only(str::one_of(alnum+" -'")))) { ; } else
                
                report += " quest"; report = "+ " + report;
        }

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return "";
    }

    Pass<entry, entry> lexforms1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("lexforms1", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return proceed_lexforms1 (title, header, forms, s, result); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n=================================================================\n";
                    report.topic.back().content += str(b.report);
                    result.report(report, "- broken brackets");
                }
        
                content = std::move(b.output);
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }
    };
}
