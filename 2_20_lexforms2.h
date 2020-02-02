#pragma once
#include "2.h"
namespace pass2
{
    namespace lexforms2_internal
    {
        static std::unordered_map<str, str> list_of_forms =
        {
        {   "abbreviation of"                   , "Abbreviation of"                                                },
        {   "acronym of"                        , "Acronym of"                                                     },
        {   "alternative form of"               , "Alternative form of"                                            },
        {   "alternate form of"                 , "Alternative form of"                                            },
        {   "alternative name of"               , "Alternative name of"                                            },
        {   "alternative spelling of"           , "Alternative spelling of"                                        },
        {   "alternative capitalisation of"     , "Alternative capitalisation of"                                  },
        {   "alternative capitalization of"     , "Alternative capitalization of"                                  },
        {   "apocopic form of"                  , "Apocopic form of"                                               },
        {   "archaic form of"                   , "Archaic form of"                                                },
        {   "archaic spelling of"               , "Archaic spelling of"                                            },
        {"en-archaic third-person singular of"  , "Archaic third-person singular simple present indicative form of"},
        {   "comparative of"                    , "Comparative form of"                                            },
        {"en-comparative of"                    , "Comparative form of"                                            },
        {   "contraction of"                    , "Contraction of"                                                 },
        {   "dated form of"                     , "Dated form of"                                                  },
        {   "eye dialect of"                    , "Eye dialect spelling of"                                        },
        {   "initialism of"                     , "Initialism of"                                                  },
        {   "irregular plural of"               , "Irregular plural of"                                            },
        {   "irregular simple past of"          , "Simple past of"                                                 },
        {   "misconstruction of"                , "Common misconstruction of"                                      },
        {   "misspelling of"                    , "Common misspelling of"                                          },
        {   "nonstandard spelling of"           , "Nonstandard spelling of"                                        },
        {   "obsolete form of"                  , "Obsolete form of"                                               },
        {   "obsolete spelling of"              , "Obsolete spelling of"                                           },
        {   "past participle of"                , "Past participle of"                                             },
        {   "past tense of"                     , "Simple past tense of"                                           },
        {"en-simple past of"                    , "Simple past tense of"                                           },
        {   "past of"                           , "Simple past tense and past participle of"                       },
        {"en-past of"                           , "Simple past tense and past participle of"                       },
        {   "plural of"                         , "Plural form of"                                                 },
        {   "present participle of"             , "Present participle of"                                          },
        {   "second-person singular of"         , "Second-person singular simple present form of"                  },
        {   "simple past of"                    , "Simple past of"                                                 },
        {   "superlative of"                    , "Superlative form of"                                            },
        {"en-superlative of"                    , "Superlative form of"                                            },
        {   "third-person singular of"          , "Third-person singular simple present indicative form of"        },
        {"en-third-person singular of"          , "Third-person singular simple present indicative form of"        },
        };
    }

    str proceed_lexforms2 (str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);

        auto it = lexforms2_internal::list_of_forms.find(args.name);
        if (it == lexforms2_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_form = it->second;
        str output = "{{" + body + "}}";
        str report = lexical_form;

        str dot = ""; // lexical_form == "Eye dialect spelling of" ? "." : "";

        if (args.complexity != 1) report += " - quest";

        else output = "''" + lexical_form + "'' '''" + args[0] + "'''" + dot;

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return output;
    }

    Pass<entry, entry> lexforms2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("lexforms2", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return proceed_lexforms2 (title, header, forms, s, result); };
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
