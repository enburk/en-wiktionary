std::unordered_map<str, str> list_of_forms =
{
    { "en-abbr"                   , "abbreviation"            },
    { "en-acronym"                , "acronym"                 },
    { "en-adj"                    , "adjective"               },
    { "en-adjective"              , "adjective"               },
    { "en-adv"                    , "adverb"                  },
    { "en-adverb"                 , "adverb"                  },
    { "en-con"                    , "conjunction"             },
    { "en-conjunction"            , "conjunction"             },
    { "en-cont"                   , "contraction"             },
    { "en-contraction"            , "contraction"             },
    { "en-det"                    , "determiner"              },
    { "en-initialism"             , "initialism"              },
    { "en-intj"                   , "interjection"            },
    { "en-interj"                 , "interjection"            },
    { "en-interjection"           , "interjection"            },
    { "en-letter"                 , "letter"                  },
    { "en-noun"                   , "noun"                    },
    { "en-plural noun"            , "noun"                    },
    { "en-plural-noun"            , "noun"                    },
    { "en-number"                 , "number"                  },
    { "en-particle"               , "particle"                },
    { "en-part"                   , "particle"                },
    { "en-phrase"                 , "phrase"                  },
    { "en-PP"                     , "prepositional phrase"    },
    { "en-pnoun"                  , "proper noun"             },
    { "en-prefix"                 , "prefix"                  },
    { "en-prep"                   , "preposition"             },
    { "en-preposition"            , "preposition"             },
    { "en-pron"                   , "pronoun"                 },
    { "en-pronoun"                , "pronoun"                 },
    { "en-proper noun"            , "proper noun"             },
    { "en-proper-noun"            , "proper noun"             },
    { "en-proper_noun"            , "proper noun"             },
    { "en-prop"                   , "proper noun"             },
    { "en-proverb"                , "proverb"                 },
    { "en-prov"                   , "proverb"                 },
    { "en-punctuation mark"       , "punctuation mark"        },
    { "en-suffix"                 , "suffix"                  },
    { "en-symbol"                 , "symbol"                  },
    { "en-verb"                   , "verb"                    },
    { "en-verb form"              , "verb"                    },
};
// {
//     "noun", "pronoun", "proper noun", "verb", "numeral", "number", "article", "particle", 
//     "preposition", "postposition", 
//     "symbol", "punctuation mark", "diacritical mark",
//     "suffix", "prefix", "infix", "affix", "interfix",
//     "phrase", "prepositional phrase", "proverb", 
// };

// "abbreviation", "acronym", "adjective", "adverb", "conjunction", "contraction", "determiner", 
// "initialism", "interjection", "letter", 

// str ending (str s)
// {
//     if (s.ends_with("y")) { s.truncate();
//     if (s.ends_with("e"))   s.truncate(); s += "i"; };
//     "ie"
//     "g", "p", "d", "t", "m"
//         "l"
//     return s;
// }

// toey ==== adjective+ {{en-adj|toeier|more|toeyer}}
// verye ==== adjective {{en-adj|verier}}
// sare ==== adjective {{en-adj|sarer|sup=sarest}}


str comparative (str s) { if (s.ends_with("y")) { s.truncate(); if (s.ends_with("e")) s.truncate(); return s + "ier";  }; return s + "er";  }
str superlative (str s) { if (s.ends_with("y")) { s.truncate(); if (s.ends_with("e")) s.truncate(); return s + "iest"; }; return s + "est"; }

str proceed_forms (str title, str & header, str body, Result<entry> & result)
{
    str name; if (int n = body.find('|'); n != str::npos) name = body.head(n); else name = body; name.strip(" \t\n");

    auto it = list_of_forms.find(name);
    if (it == list_of_forms.end()) return "{{" + body + "}}";
    auto [templ, item] = *it;

    array<str> args = body.split_by("|");
    for (auto & arg : args) arg.strip(" \t\n");
    args.erase(args.begin());
    array<str> unnamed;
    std::map<str,str> opt;
    for (str arg : args) { str key, value;
        if (arg.split_by ("=", key, value)) {
            if (key == "head") continue;
            else opt [key] = value; }
        else unnamed += arg;
    }
    args = unnamed;
    str report = item;

    if (item != header) report += " wrong"; else
    if (item == "adjective" ||
        item == "adverb")
    {
        bool er = false, more = false, nope = false, quest = false; array<str> raw;
        for (auto arg : args)
            if (arg == "more") more = true; else
            if (arg == "er") er = true; else
            if (arg == "-") nope = true; else
            if (arg == "?") quest = true; else
                raw += arg;
        bool complex = raw.size() > 0 || opt.size() > 0;
        if (!complex && !er && !more && !nope &&  quest) { header += " # ?";   report += " q";   } else
        if (!complex && !er && !more &&  nope && !quest) { header += " # -";   report += " -";   } else
        if (!complex && !er && !more && !nope && !quest) { header += " # +";   report += " +";   } else
        if (!complex && !er &&  more && !nope && !quest) { header += " # +";   report += " +";   } else
        if (!complex && !er &&  more &&  nope && !quest) { header += " # +-";  report += " +-";  } else // (not generally comparable, comparative more godforsaken, superlative most godforsaken)
        if (!complex &&  er && !more && !nope && !quest) { header += " # er";  report += " er";  } else
        if (!complex &&  er &&  more && !nope && !quest) { header += " # er+"; report += " er+"; } else
        if (!complex &&  er && !more &&  nope && !quest) { header += " # er-"; report += " er-"; } else // (not generally comparable, comparative doozier, superlative dooziest)
        if (!complex) report += " simple"; else
        {
            report += " complex";
            str sup1, sup2, sup3; for (auto [key, value] : opt) {
                if (key == "sup" ) sup1 = value; else
                if (key == "sup2") sup2 = value; else sup3 += value;
            }
            if (sup2 == "" && sup3 == "")
            {
                str cmp = raw.size() >= 1 ? raw[0] : "";
                str sup = raw.size() >= 2 ? raw[1] : sup;

                if (cmp == comparative(title))
                if (sup == superlative(title) || sup == "")
                     { header += " # er"; report += " er="; }
                else { header += " # " + cmp + "|" + sup; report += " raw"; }

                if (more) header += "+";
                if (nope) header += "-";
            }
            else report += " quest";
        }
    }
    else
    if (item == "noun")
    {
        if (body == "en-noun") { header += " # +";   report += " +";   } else
        {
        }
    }

    result.report (title + " ==== " + header + " {{" + body + "}}", report);
    return "";
}

Pass <entry, entry> forms = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("forms     ", nn, " entries ", input.cargo, " cargo ");

        for (auto & [header, content] : topic)
        {
            bracketer b;
            b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
            b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
            b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
            b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
            b.proceed_template  = [&] (str s) { return proceed_forms (title, header, s, result); };
            b.proceed(content);
            
            if (b.report.size() > 0)
            {
                entry report;
                report.title = title;
                report.topic += paragraph {header, content};
                report.topic.back().content += "\n=================================================================\n";
                report.topic.back().content += str(b.report);
                result.report(report, "- broken brackets");
            }
        
            content = std::move(b.output);
        }

        result.accept (entry {std::move(title), std::move(topic)});
    }
};

