#pragma once
#include "2.h"
namespace pass2
{
    namespace lexforms2_internal
    {
        static std::unordered_map<str, str> list_of_forms =
        {
        {   "abbreviation of"                       , "Abbreviation of"                                                  },
        {   "acronym of"                            , "Acronym of"                                                       },
        {   "alternate form of"                     , "Alternative form of"                                              },
        {   "alternative form of"                   , "Alternative form of"                                              },
        {   "alternative case form of"              , "Alternative letter-case of"                                       },
        {   "alternative name of"                   , "Alternative name of"                                              },
        {   "alternative spelling of"               , "Alternative spelling of"                                          },
        {   "alternative capitalisation of"         , "Alternative capitalisation of"                                    },
        {   "alternative capitalization of"         , "Alternative capitalization of"                                    },
        {   "apocopic form of"                      , "Apocopic form of"                                                 },
        {   "archaic form of"                       , "Archaic form of"                                                  },
        {   "archaic spelling of"                   , "Archaic spelling of"                                              },
        {"en-archaic second-person singular of"     , "(archaic) second-person singular simple present form of"          },
        {"en-archaic second-person singular past of", "(archaic) second-person singular simple past form of"             },
        {"en-archaic third-person singular of"      , "(archaic) third-person singular simple present indicative form of"},
        {   "comparative of"                        , "comparative form of"                                              },
        {"en-comparative of"                        , "comparative form of"                                              },
        {   "contraction of"                        , "Contraction of"                                                   },
        {   "dated form of"                         , "Dated form of"                                                    },
        {   "eye dialect of"                        , "Eye dialect spelling of"                                          },
        {   "initialism of"                         , "Initialism of"                                                    },
        {   "irregular plural of"                   , "plural of"                                                        },
        {"en-irregular plural of"                   , "plural of"                                                        },
        {   "irregular simple past of"              , "simple past tense of"                                             },
        {   "misconstruction of"                    , "Common misconstruction of"                                        },
        {   "misspelling of"                        , "Common misspelling of"                                            },
        {   "nonstandard spelling of"               , "Nonstandard spelling of"                                          },
        {   "obsolete form of"                      , "Obsolete form of"                                                 },
        {   "obsolete spelling of"                  , "Obsolete spelling of"                                             },
        {   "obsolete typography of"                , "obsolete typography of"                                           },
        {   "past participle of"                    , "past participle of"                                               },
        {   "past tense of"                         , "simple past tense of"                                             },
        {"en-simple past of"                        , "simple past tense of"                                             },
        {   "past of"                               , "simple past tense and past participle of"                         },
        {"en-past of"                               , "simple past tense and past participle of"                         },
        {   "plural of"                             , "plural of"                                                        },
        {   "present participle of"                 , "present participle of"                                            },
        {   "pronunciation spelling of"             , "Pronunciation spelling of"                                        },
        {   "second-person singular of"             , "second-person singular simple present form of"                    },
        {   "simple past of"                        , "simple past tense of"                                             },
        {   "superlative of"                        , "superlative form of"                                              },
        {"en-superlative of"                        , "superlative form of"                                              },
        {   "synonym of"                            , "Synonym of"                                                       },
        {   "third-person singular of"              , "third-person singular simple present indicative form of"          },
        {"en-third-person singular of"              , "third-person singular simple present indicative form of"          },
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

        str opts;
        str first = lexical_form.upto(1);
        str from  = args.acquire("from" );
        str dot   = args.acquire("dot"  );
        str nodot = args.acquire("nodot"); if (nodot == "yes" || nodot == "y" || nodot == "t" || nodot == "a") nodot = "1";
        str nocap = args.acquire("nocap"); if (nocap == "yes" || nocap == "y" || nocap == "t" || nocap == "a") nocap = "1";
        if (dot != "" && dot != "." && dot != "," && dot != ":" && dot != ";") { opts += " - dot"; }
        if (nodot != "" && nodot != "1") { opts += " - nodot"; }
        if (nocap != "" && nocap != "1") { opts += " - nocap"; }
        if (nocap != "") lexical_form.upto(1).replace_by(first.ascii_lowercased());
        if (dot   == "") dot = first == first.ascii_lowercased() ? "" : ".";
        if (nodot != "") dot = "";
        if (opts  != "") report = opts;
        if (from  != "") report = " - from";
        if (from  != "") args.complexity += 10;

        str a1, a2, a3;
        args.ignore("t"); args.ignore("tr"); args.ignore("gloss"); args.ignore("pos"); args.ignore("id");
        if (args.unnamed.size() > 0 && args[0] != "") a1 = "'''"+args[0]+"'''"; 
        if (args.unnamed.size() > 1 && args[1] != "") a2 = "'''"+args[1]+"'''"; 
        if (args.unnamed.size() > 2 && args[2] != "") a3 = "("+oquot+args[2]+cquot+")";

        str out;
        if (args.complexity == 1 && a1 != "") { out = a1; } else // report += " 1"; } else
        if (args.complexity == 2 && a2 == "") { out = a1; } else // report += " 2"; } else
        if (args.complexity == 2 && a2 != "") { out = a2; } else // report += " 2"; } else
        if (args.complexity == 3 && a1 == "" && a2 != "") { out = a2 + " " + a3; } else // report += " 3"; } else
        if (args.complexity == 3 && a1 != "" && a2 == "") { out = a1 + " " + a3; } else // report += " 3"; } else
        if (args.complexity == 3 && a1 != "" && a2 != "") { out = a2 + " " + a3; } else // report += " 3"; } else
        {}

        if (out != "") output = "''" + lexical_form + "'' " + out + dot;

        else report += " - quest";

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
