#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> modules2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            if (first_time) {
                first_time = false;

                for (auto & [name, text] : Modules)
                {
                    if (name.starts_with("label"))
                    {
                        str report = name;
                        report.replace_all("/", "~");
                        result.report(text, report);

                        Repo[name] = lua::syntax::analysis(text);

                        auto & analysis = Repo[name];

                        result.report(analysis.log(), report + ".log");

                        array<str> lines;
                        for (auto & c : analysis.clusters)
                            lua::syntax::print(c, lines);

                        result.report(str(lines), report + ".lines");
                    }
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
