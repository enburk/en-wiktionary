#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> fix = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                str cap = "==== "+title+" ==== "+header+" ==== "+forms+"\n";

                for (str & line : content)
                {
					int n = 9;
					str fixation;
					str prefix = line;
					auto fix = [&](str what, auto expr)
					{
						n++; while (expr())
							fixation = std::to_string(n) +
								" " + what;
					};

					fix("...", [&](){ return 0 <
						line.replace_all('.', 3, "…");
						});

					if (not line.contains(str::one_of("<>")))
					fix("spaces", [&](){ return 0 <
						line.replace_all("  ", " ");
						});
					fix("spaces+", [&](){ return 0 <
						line.replace_all("now  rare",    "now rare");
						line.replace_all("now  chiefly", "now chiefly");
						});
					if (line.contains(str::one_of("<>")))
					fix("spaces++", [&](){ return 0 <
						line.replace_all("  ", " ");
						});

					fix("..end", [&](){
						if (not line.ends_with("..")) return false;
						line.truncate(); return true;
						});
					fix("-.end", [&](){
						if (not line.ends_with(" - .")) return false;
						line.truncate(); line.truncate();
						line.truncate(); line.truncate();
						line += ".";
						return true;
						});

					fix(", ,", [&](){ return 0 <
				        line.replace_all(". .", ".")+
				        line.replace_all(", ,", ",")+
				        line.replace_all(": :", ":")+
				        line.replace_all("; ;", ";")+

				        line.replace_all(". ,", ",")+
				        line.replace_all(". :", ":")+
				        line.replace_all(". ;", ";")+

				        line.replace_all(", .", ".")+
				        line.replace_all(", :", ":")+
				        line.replace_all(", ;", ";")+

						line.replace_all(": .", ".")+
						line.replace_all(": ,", ":")+
						line.replace_all(": ;", ":")+

						line.replace_all("; .", ".")+
						line.replace_all("; ,", ";")+
						line.replace_all("; :", ";");
						});

					if (not title.starts_with(".") and
						not title.starts_with(",") and
						not title.starts_with(":") and
						not title.starts_with(";"))
					fix("_ ,", [&](){ return 0 <
				        line.replace_all(") .", ").")+
				        line.replace_all(") ,", "),")+
				        line.replace_all(") :", "):")+
				        line.replace_all(") ;", ");")+

				        line.replace_all("' .", "'.")+
				        line.replace_all("' ,", "',")+
				        line.replace_all("' :", "':")+
				        line.replace_all("' ;", "';");
						});

					fix("the an", [&](){ return 0 <
				        line.replace_all(" en:", " ")+
				        line.replace_all(" in in ", " in ")+
				        line.replace_all(" of of ", " of ")+
				        line.replace_all(" on on ", " on ")+
				        line.replace_all(" the the ", " the ")+
				        line.replace_all(" an an ", " an ")+
				        line.replace_all(" a a ", " a ");
						});

					fix("see", [&](){
						str s1, s2; if (
						line.split_by("; see ", s1, s2)) {
						line = s1 + "."; return true; }
						return false;
						});
					fix("see", [&](){
						str s1, s2; if (
						line.split_by("; See ", s1, s2)) {
						line = s1 + "."; return true; }
						return false;
						});

				    if (fixation != "")
						result.report (
						cap + prefix + "\n"
							+ line + "\n",
					    "fix " + fixation);
                }

                for (str line : content)
                {
					int n = 49;
					str fixation;
					str prefix = line;
					auto fix = [&](str what, auto expr)
					{
						n++; while (expr())
							fixation = std::to_string(n) +
								" " + what;
					};

					fix("...", [&](){ return 0 <
				        line.replace_all("..", ".")+
				        line.replace_all(",,", ",")+
				        line.replace_all(";;", ";");
						});

					fix("_ ,", [&](){ return 0 <
				        line.replace_all(" .", ".")+
				        line.replace_all(" ,", ",")+
				        line.replace_all(" :", ":")+
				        line.replace_all(" ;", ";");
						});

					fix(",,,", [&](){ return 0 <
				        line.replace_all(".,", ",")+
				        line.replace_all(".:", ":")+
				        line.replace_all(".;", ";")+

				        line.replace_all(",.", ".")+
				        line.replace_all(",:", ":")+
				        line.replace_all(",;", ";")+

						line.replace_all(":.", ".")+
						line.replace_all(":,", ":")+
						line.replace_all(":;", ":")+

						line.replace_all(";.", ".")+
						line.replace_all(";,", ";")+
						line.replace_all(";:", ";");
						});

					fix("the an", [&](){ return 0 <
				        line.replace_all(" in, the, ", " in the ")+
				        line.replace_all(" on, the, ", " on the ")+
				        line.replace_all(" of, the, ", " of the ")+
				        line.replace_all(" the, ", " the ")+
				        line.replace_all(" an, ", " an ")+
				        line.replace_all(" a, ", " a ");
						});

				    if (fixation != "")
						result.report (
						cap + prefix + "\n"
							+ line + "\n",
					    "skip " + fixation);
				}

                for (str & line : content)
                {
                    line.replace_all("[[.]]", ".");
                    line.replace_all("[[,]]", ",");
                    line.replace_all("[[:]]", ":");
                    line.replace_all("[[;]]", ";");
                    line.replace_all("_COLON_", ":");
                    line.replace_all("_COLONEQ_", ":=");
                    line.replace_all("_SMILE1_", ":)");
                    line.replace_all("_SMILE2_", ":-)");
                    line.replace_all("_PIPE_", "|");
                    line.replace_all("_EQUAL_", "=");
                    line.replace_all("_LCURLY_", "{");
                    line.replace_all("_RCURLY_", "}");
				}
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
