#pragma once
#include "4.h"
namespace pass4
{
    Pass <entry, entry> meta1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        array<str> groups = {"label", "lang", "etymo", "families", "place"};

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            if (first_time) {
                first_time = false;

                for (auto & [name, text] : Modules)
                {
                    str report = "zzz";
                    for (auto group : groups)
                        if (name.starts_with(group))
                            report = group;

                    if (report == "zzz")
                    if ((name.size() >= 4 and name[3] == '-') or
                        (name.size() >= 3 and name[2] == '-'))
                        report = "xxx";

                    result.report(""
                    + esc + "\n" + ("#Module:" + name)  + "\n"
                    + esc + "\n" + text + "\n",
                    "modules " + report);
                    result.report("Module:" + name,
                    "modules " + report + " #");
                }

                for (auto & [name, text] : Templates)
                {
                    str report = "zzz";
                    for (auto group : groups)
                        if (name.starts_with(group))
                            report = group;

                    if (report == "zzz")
                    if ((name.size() >= 4 and name[3] == '-') or
                        (name.size() >= 3 and name[2] == '-'))
                        report = "xxx";

                    result.report(""
                    + esc + "\n" + ("#Template:" + name)  + "\n"
                    + esc + "\n" + text + "\n",
                    "templates " + report);
                    result.report("Template:" + name,
                    "templates " + report + " #");
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
