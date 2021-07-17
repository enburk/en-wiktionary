#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> fix_bold = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                str cap = "==== "+title+" ==== "+header+"\n";

                for (str & line : content)
                {
					str fixation;
					str prefix = line;

                    bool bold = false;
                    bool ital = false;
                    int apostrophes = 0;

                    for(char c : line + " ")
                    {
                        if (c == '\'' ) apostrophes++; else
                        {
                            switch (apostrophes) {
                            case 0: break;
                            case 1: break;
                            case 2: ital = not ital; break;
                            case 3: bold = not bold; break;
                            case 5: bold = not bold; ital = not ital; break;
                            default: fixation += " quest1"; }

                            apostrophes = 0;
                        }
                    }

                    if (bold or ital)
                        fixation += " quest2";

                    if (fixation != "")
                    {
                        fixation = "";

                        if (title.starts_with("'")) {
                            str s1 = "'"+ucfirst(title.from(1));
                            str s2 = "'"+lcfirst(title.from(1));
                            line.replace_all(s1, zws+s1);
                            line.replace_all(s2, zws+s2);
                            fixation = "title";
                        }
                        if (title.ends_with("'")) {
                            str s1 = ucfirst(title);
                            str s2 = lcfirst(title);
                            line.replace_all(s1, s1+zws);
                            line.replace_all(s2, s2+zws);
                            fixation = "title";
                        }

                        if (line.replace_all("'''''''", "'''"+zws+"'"+zws+"'''")+
                            line.replace_all("''''d best''", "'''"+zws+"'d best"+zws+"'''")
                             > 0)
                            fixation = "'''";
                    }

                    bold = false;
                    ital = false;
                    apostrophes = 0;

                    for(char c : line + " ")
                    {
                        if (c == '\'' ) apostrophes++; else
                        {
                            switch (apostrophes) {
                            case 2: if (bold and fixation == "") fixation = "nested ital"; break;
                            case 3: if (ital and fixation == "") fixation = "nested bold"; break;
                            case 5: if (fixation == "") fixation = "both"; break;
                            default: break; }

                            switch (apostrophes) {
                            case 0: break;
                            case 1: break;
                            case 2: ital = not ital; break;
                            case 3: bold = not bold; break;
                            case 4: bold = not bold; fixation = "4"; break;
                            case 5: bold = not bold; ital = not ital; break;
                            case 6: bold = not bold; ital = not ital; fixation = "6"; break;
                            default: fixation += " quest1"; }

                            apostrophes = 0;
                        }
                    }

                    if (bold or ital)
                        fixation += " quest2";

				    if (fixation != "") result.report (
						cap + prefix + "\n" + line + "\n",
					    fixation);
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
