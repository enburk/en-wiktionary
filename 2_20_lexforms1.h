#pragma once
#include "2.h"
namespace pass2
{
    namespace lexforms1_internal
    {
        std::unordered_map<str, str> list_of_forms =
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

        std::map<str, array<str>> wrongs;
    }

    str lexforms1_(str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);

        auto it = lexforms1_internal::list_of_forms.find(args.name);
        if (it == lexforms1_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_item = it->second;
        str report = header;
        str arg = args.body;

        if (header != lexical_item)
        {
            lexforms1_internal::wrongs[header] +=
                title + " ==== " + header + " ==== {{" + body + "}}";
            return "";
        }
        else
        if (header == "noun"
        or  header == "proper noun"
        or  header == "adjective"
        or  header == "adverb"
        or  header == "verb")
        {
            if (args.name == "en-plural noun" or
                args.name == "en-plural-noun")
                arg = "##" + arg;

            report = "++ " + report;
            if (arg == "#") report = "#";
            if (arg == "") arg = "#";
            forms = arg;
        }

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return "";
    }

    Pass<entry, entry> lexforms1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("lexforms1", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;
                auto & f = forms;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return lexforms1_(t, h, f, s, result); };
                b.proceed(content);
        
                content = std::move(b.output);
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }

        for (auto & [header, logs] : lexforms1_internal::wrongs) {
            for (auto & log : logs)
            result.report (log, "wrong");
            result.report ("\n", "wrong");
        }
    };
}
