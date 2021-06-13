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
        {   "alternative typography of"             , "Alternative spelling of"                                          },
        {   "alternative capitalisation of"         , "Alternative capitalisation of"                                    },
        {   "alternative capitalization of"         , "Alternative capitalization of"                                    },
        {   "archaic form of"                       , "Archaic form of"                                                  },
        {   "archaic spelling of"                   , "Archaic spelling of"                                              },
        {"en-en-obsolete past participle of"        , "(archaic) past participle of"                                     },
        {"en-archaic second-person singular of"     , "(archaic) second-person singular simple present form of"          },
        {"en-archaic second-person singular past of", "(archaic) second-person singular simple past form of"             },
        {"en-archaic third-person singular of"      , "(archaic) third-person singular simple present indicative form of"},
        {   "comparative of"                        , "comparative form of"                                              },
        {"en-comparative of"                        , "comparative form of"                                              },
        {   "contraction of"                        , "Contraction of"                                                   },
        {   "dated form of"                         , "Dated form of"                                                    },
        {   "dated spelling of"                     , "Dated spelling of"                                                },
        {   "deliberate misspelling of"             , "Deliberate misspelling of"                                        },
        {   "initialism of"                         , "Initialism of"                                                    },
        {   "irregular plural of"                   , "plural of"                                                        },
        {"en-irregular plural of"                   , "plural of"                                                        },
        {   "irregular simple past of"              , "simple past tense of"                                             },
        {   "misconstruction of"                    , "Common misconstruction of"                                        },
        {   "misspelling of"                        , "Common misspelling of"                                            },
        {   "nonstandard spelling of"               , "Nonstandard spelling of"                                          },
        {   "obsolete form of"                      , "Obsolete form of"                                                 },
        {   "obsolete_form_of"                      , "Obsolete form of"                                                 },
        {   "obsolete spelling of"                  , "Obsolete spelling of"                                             },
        {   "obsolete typography of"                , "obsolete typography of"                                           },
        {   "past participle of"                    , "past participle of"                                               },
        {   "past tense of"                         , "simple past tense of"                                             },
        {"en-simple past of"                        , "simple past tense of"                                             },
        {   "past of"                               , "simple past tense and past participle of"                         },
        {"en-past of"                               , "simple past tense and past participle of"                         },
        {   "plural of"                             , "plural of"                                                        },
        {   "present participle of"                 , "present participle of"                                            },
        {"en-ing form of"                           , "present participle of"                                            },
        {   "pronunciation spelling of"             , "Pronunciation spelling of"                                        },
        {   "present tense of"                      , "second-person singular simple present form of"                    },
        {   "second-person singular of"             , "second-person singular simple present form of"                    },
        {   "simple past of"                        , "simple past tense of"                                             },
        {   "standard spelling of"                  , "Standard spelling of"                                             },
        {   "standard form of"                      , "Standard form of"                                                 },
        {   "superlative of"                        , "superlative form of"                                              },
        {"en-superlative of"                        , "superlative form of"                                              },
        {   "synonym of"                            , "Synonym of"                                                       },
        {   "third-person singular of"              , "third-person singular simple present indicative form of"          },
        {"en-third-person singular of"              , "third-person singular simple present indicative form of"          },

        {   "agent noun of"                         , "agent noun of"                                                    },
        {   "apocopic form of"                      , "Apocopic form of"                                                 },
        {   "aphetic form of"                       , "Aphetic form of"                                                  },
        {   "attributive form of"                   , "attributive form of"                                              },
        {   "eggcorn of"                            , "Eggcorn of"                                                       },
        {   "ellipsis of"                           , "Ellipsis of"                                                      },
        {   "elongated form of"                     , "Elongated form of"                                                },
        {   "eye dialect of"                        , "Eye dialect spelling of"                                          },
        {   "diminutive of"                         , "Diminutive of"                                                    },
        {   "euphemistic form of"                   , "Euphemistic form of"                                              },
        {   "euphemistic spelling of"               , "Euphemistic spelling of"                                          },
        {   "female equivalent of"                  , "female equivalent of"                                             },
        {   "feminine of"                           , "feminine of"                                                      },
        {   "feminine plural of"                    , "feminine plural of"                                               },
        {   "former name of"                        , "Former name of"                                                   },
        {   "gerund of"                             , "gerund of"                                                        },
        {   "honorific alternative case form of"    , "honorific alternative case form of"                               },
        {   "informal form of"                      , "Informal form of"                                                 },
        {   "informal spelling of"                  , "Informal spelling of"                                             },
        {   "masculine plural of"                   , "masculine plural of"                                              },
        {   "nonstandard form of"                   , "Nonstandard form of"                                              },
        {   "rare form of"                          , "Rare form of"                                                     },
        {   "rare spelling of"                      , "Rare spelling of"                                                 },
        {   "singular of"                           , "singular of"                                                      },
        {   "syncopic form of"                      , "Syncopic form of"                                                 },
        {   "uncommon form of"                      , "Uncommon form of"                                                 },
        {   "uncommon spelling of"                  , "Uncommon spelling of"                                             },
        {   ""                        , ""                                          },
        {   ""                        , ""                                          },
        {   ""                        , ""                                          },
        };
    }

    str lexforms2_(str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);

        if (args.name == "form of")
        {
            str output = "{{" + body + "}}";
            str kind   = "{{form of}}";
            str nodot = args.acquire("nodot");
            if (args.complexity == 2) {
                output = "''" + args[0] + " of " + args[1] + "''";
                output += nodot == "" ? "." : "";
            }
            else
            if (args.complexity == 3) {
                output = "''" + args[0] + " of " + args[2] + "''";
                output += nodot == "" ? "." : "";
            }
            else kind += " quest";
            result.report (title + " ==== " + header + " ==== {{" + body + "}}", kind);
            return output;
        }

        auto it = lexforms2_internal::list_of_forms.find(args.name);
        if (it == lexforms2_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_form = it->second;
        str output = "{{" + body + "}}";
        str report = lexical_form;

        if (args.complexity == 1 && args[0] != "")
        {
            str out = args[0];

            if (out.starts_with("W:")) out.upto(2).erase();
            if (out.starts_with("w:")) out.upto(2).erase();
            if (out.starts_with("s:")) out.upto(2).erase();

            output = out;
            if (not output.starts_with("'''")) output = "'''" + output;
            if (not output.ends_with  ("'''")) output = output + "'''";
            output = "''" + lexical_form + "'' " + output;

            str first = lexical_form.upto(1);
            if (first != first.ascii_lowercased())
                output += ".";

            {
                std::lock_guard lock{lexforms_mutex};
                lexforms[out] += lexform{lexical_form, "-", title};
            }

            out.replace_all("[[", "");
            out.replace_all("]]", "");

            if (not out.contains_only(str::one_of(ALnum))) report = "- alnum1";
            if (not out.contains_only(str::one_of(ALNUM))) report = "- alnum2";
        }
        else report = "- quest";

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return output;
    }

    Pass<entry, entry> lexforms2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("lexforms2", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                for (auto & [from, to] : lexforms2_internal::list_of_forms)
                    if (content.contains("# " + to + " ")) result.report (
                        title + " ==== " + header + " ==== \n"
                            + content + "\n", "- raw");

                auto t = title;
                auto h = header;
                auto & f = forms;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return lexforms2_(t, h, f, s, result); };
                b.proceed(content);
        
                content = std::move(b.output);
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
