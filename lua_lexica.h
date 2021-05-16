#pragma once
#include <set>
#include "lua_glyph.h"
namespace lua::lexica
{
    const std::set<str> keywords = 
    {
        "and"   , "break" , "do"   , "else"    , "elseif", 
        "end"   , "false" , "for"  , "function", "if"    , 
        "in"    , "local" , "nil"  , "not"     , "or"    , 
        "repeat", "return", "then" , "true"    , "until" , "while"
    };

    bool unclosed_literal (const str & text, str symbol = "\"")
    {
        return text == symbol or
           not text.ends_with(symbol) or
               text.ends_with("\\"+symbol);
    }

    array<token> parse (array<glyph> text)
    {
        array<token> tokens; token t; text += "\n";

        int line = 1; int offset = 0;
        
        for (glyph c : text)
        {
            if (c == "\n") { line++; offset = 1; } else offset++;

            if (t.kind.starts_with("-")
            or  t.kind.starts_with("["))
                t.kind += c;

            if (t.kind.starts_with("--[") and c == "]")
            if((t.kind.starts_with("--[[") and t.kind.ends_with("]]"))
            or (t.kind.starts_with("--[=[") and t.kind.ends_with("]=]"))
            or (t.kind.starts_with("--[==[") and t.kind.ends_with("]==]"))
            or (t.kind.starts_with("--[===[") and t.kind.ends_with("]===]"))
            or (t.kind.starts_with("--[====[") and t.kind.ends_with("]====]"))
            or (t.kind.starts_with("--[=====[") and t.kind.ends_with("]=====]")))
            { t = token{}; continue; }

            if (t.kind.starts_with("--") and c == "\n" and
            not t.kind.starts_with("--["))
            { t = token{}; continue; }

            if (t.kind.starts_with("[") and c == "]")
            if((t.kind.starts_with("[[") and t.kind.ends_with("]]"))
            or (t.kind.starts_with("[=[") and t.kind.ends_with("]=]"))
            or (t.kind.starts_with("[==[") and t.kind.ends_with("]==]"))
            or (t.kind.starts_with("[===[") and t.kind.ends_with("]===]"))
            or (t.kind.starts_with("[====[") and t.kind.ends_with("]====]"))
            or (t.kind.starts_with("[=====[") and t.kind.ends_with("]=====]")))
            {
                t.text = t.kind;
                t.kind = "literalll";
                t.range.upto = place{line, offset};
                tokens += t;
                t = token{};
                continue;
            }

            if (t.kind.starts_with("-")
            or  t.kind.starts_with("["))
                continue;

            bool same = false;

            if (t.kind == "space"   ) same = c.space (); else
            if (t.kind == "name"    ) same = c.letter() || c.digit() || c == "_"; else
            if (t.kind == "number"  ) same = c.letter() || c.digit() || c == "."; else
            if (t.kind == "literall") same = unclosed_literal(t.text, "\""); else
            if (t.kind == "literal" ) same = unclosed_literal(t.text, "\'"); else
            if (t.kind == "symbol"  ) same = false

            || (t.text == "<" && c == "=")
            || (t.text == ">" && c == "=")
            || (t.text == "~" && c == "=")
            || (t.text == "=" && c == "=")
            || (t.text == "." && c == ".")
            || (t.text ==".." && c == ".")

            || (t.text == "-" && c == "-")
            || (t.text == "[" && c == "[")
            || (t.text == "[" && c == "=")
            ;
            if (same)
            {
                t.text += c;
                t.range.upto = place{line, offset};

                if (t.text == "--" or
                    t.text == "[[" or
                    t.text == "[=") {
                    t.kind = t.text;
                    t.text = "";
                }
            }
            else
            {
                if (t.kind == "name" and
                    keywords.contains(t.text))
                    t.kind = "keyword";

                if (t.text != "" and t.kind != "space")
                {
                    if (t.text == "until"
                    or  t.text == "elseif"
                    or  t.text == "else") {
                    tokens += t; tokens.back().text = "end"; }
                    tokens += t;
                }

                t = token {c, "", "", range{
                    {line, offset},
                    {line, offset}}
                };
                t.kind =
                    c.letter() ? "name"    :
                    c.digit () ? "number"  :
                    c.space () ? "space"   : 
                    c == "\n"  ? "\n"      : 
                    c == "\""  ? "literall": 
                    c == "\'"  ? "literal" : 
                                 "symbol"  ;
            }
        }

        if (t.text != "\n") throw
            std::runtime_error(
                "lua::lexica: "
                    + t.text);

        return tokens;
    }

    str print (array<token> const& tokens)
    {
        str s;
        for (auto t : tokens)
        {
            if (t.kind == "space") continue;
            if (t.text == "\n") s += "\n"; else {
                t.text.replace_all("\n", "\\n");
                s += t.text + " ";
            }
        }
        return s;
    }
}

