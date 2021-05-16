#pragma once
#include "main.hpp"
namespace lua
{
    struct place
    {
        int line = 0;
        int offset = 0;
        auto operator <=> (const place & p) const = default;
    };

    struct range
    {
        place from, upto;
        bool empty () const { return from == upto; }
        bool operator == (const range & r) const = default;
        bool operator != (const range & r) const = default;
    };

    struct token
    {
        str text, kind, info; range range;
        bool operator != (const token & t) const = default;
        bool operator == (const token & t) const = default;
    };

    struct element
    {
        token* token = nullptr;
        array<element> elements;
    };

    struct cluster
    {
        array<element> elements;
        array<cluster> clusters;
    };

    struct report
    {
        struct info { token* token = nullptr; str kind, what; };
        array <info> traces;
        array <info> errors;

        void trace (str what) { trace(nullptr, what); }
        void error (str what) { error(nullptr, what); }

        void trace (token* token, str what) {
            traces += info{token, "trace", what};
        }
        void error (token* token, str what) {
            traces += info{token, "error", what};
            errors += info{token, "error", what};
        }
        str operator () () const
        {
            str s;
            for (auto [token, kind, what] : traces)
            {
                //if (kind == "debug") s += "<font color=#616161>";
                //if (kind == "error") s += "<font color=#B00020>";

                if (token) s += "("
                    + std::to_string(token->range.from.line  ) + ":"
                    + std::to_string(token->range.from.offset) + ") ";

                s += what;

                //if (kind == "debug" or
                //    kind == "error")
                //    s += "</font>";
                //
                //s += "<br>";
                s += "\n";
            }
            return s;
        }
    };
}
