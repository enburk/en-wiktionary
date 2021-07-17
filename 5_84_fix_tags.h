#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> fix_tags = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                str cap = "==== "+title+" ==== "+header+" ==== "+forms+"\n";

                for (str & line : content)
                {
					str fixation;
					str prefix = line;
                    array<str> stack;

                    line.replace_all("<em>", "<i>");
                    line.replace_all("</em>", "</i>");

                    for (int p = 0; ; )
                    {
                        auto ra = line.find("<", str::start_from(p)); int a = ra.offset;
                        auto rb = line.find(">", str::start_from(p)); int b = rb.offset;

                        if (!ra or !rb) break;
                        
                        p = b+1; if (a > b) continue;

                        str tag = line.from(a+1).upto(b);
                        str name, tail; tag.split_by(str::one_of(" /\n"),
                            name, tail, str::delimiter::to_the_right);

                        name = name.ascii_lowercased();
                        if (not name.ascii_isalpha()) {
                            p = a+1; continue; }

                        if (not array<str>{
                            "b", "i", "u", "s",
                            "br", "span", "nowiki", "blockquote", 
                            "math", "code", "chem",
                            "sub", "sup",
                            "big", "small",
                            ""}.contains(name))
                            fixation = "quest0";

                        if (name == "code"
                        or  name == "chem"
                        or  name == "math"
                        or  name == "blockquote")
                        {
                            auto rc = line.find("</"+name+">", str::start_from(p));
                            if (rc) { p = rc.offset + rc.length;
                                str text = line.from(a).upto(p);
                                result.report (cap + text + "\n",
					            "tag " + name);
                                continue;
                            }
                        }
                        else
                        if (tail != "" and
                            not tail.starts_with("/") and
                            not tail.starts_with(" /")) {
                            str text = line.from(a).upto(p);
                            result.report (cap + text + "\n",
					        "tag " + name);
                        }

                        if (name == "" and
                            tail.starts_with("/")) {
                            tail.erase(0);
                            if (stack.size() > 0 and
                                stack.back() == tail)
                                stack.truncate();
                            else fixation = "quest1";
                        }
                        else
                        if (name != "br" and 
                            not tail.ends_with("/"))
                            stack += name;
                    }

                    if (stack.size() > 0) fixation = "quest2";

				    if (fixation != "") result.report (
						cap + prefix + "\n" + line + "\n",
					    "fix " + fixation);

                    if (stack.size() > 0) result.report (
                        "STACK: " + str::list(stack) + "\n",
					    "fix " + fixation);
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
