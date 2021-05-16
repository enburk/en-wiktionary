#pragma once
#include "lua.h"
namespace lua::syntax
{
    struct parser
    {
        array<cluster> output;
        array<token> & input; report& log; parser (
        array<token> & input, report& log) :
        input(input), log(log)
        {
            deque<token*> tokens;
            for (auto & token : input)
                tokens += &token;

            output = 
            delimiting(
            elementing(
            tokens));
        }

        auto elementing (deque<token*> & input, str closing = "") -> array<element>
        {
            auto fulfill = [this, &input](element & e, str closing)
            {
                e.elements = elementing(input, closing);

                if (input.size() > 0)
                    input.pop_front();
                else
                    log.error(
                    e.token, "unclosed " +
                    e.token->text);
            };

            array<element> output;
            
            while (input.size() > 0)
            {
                auto token = input.front();
                if (token->text == closing) break;
                input.pop_front();

                if (token->text == "}"
                or  token->text == ")"
                or  token->text == "]"
                or  token->text == "end") log.error(token, "unexpected " +
                    token->text);

                element e; e.token = token;

                if (token->text == "{") fulfill(e, "}"); else
                if (token->text == "(") fulfill(e, ")"); else
                if (token->text == "[") fulfill(e, "]"); else
                if (token->text == "function" 
                or  token->text == "repeat"
                or  token->text == "then"
                or  token->text == "else"
                or  token->text == "do")
                    fulfill(e, "end");

                output += e;
            }

            return output;
        }

        auto delimiting (array<element> input) -> array<cluster>
        {
            array<cluster> output; cluster out;

            for (auto && e : input)
            {
                if (e.token->text == ";" or
                    e.token->text == "\n")
                {
                    output += std::move(out);
                    out = cluster{};
                }
                else
                if (e.token->text == "(" or
                    e.token->text == "[" or
                    e.token->text == "{")
                {
                    e.elements = bracketing(std::move(e.elements));
                    out.elements += std::move(e);
                }
                else
                if (not e.elements.empty())
                {
                    out.clusters += delimiting(std::move(e.elements));
                    out.elements += std::move(e);
                    output += std::move(out);
                    out = cluster{};
                }
                else out.elements += std::move(e);
            }
    
            return output;
        }

        auto bracketing (array<element> input) -> array<element>
        {
            array<element> output; element o;

            for (auto && i : input)
            {
                if (i.token->text == "\n") continue;
                if (i.token->text == "," or
                    i.token->text == ";")
                {
                    if (o.elements.empty()) continue;
                    output += std::move(o);
                    o = element{};
                }
                else
                if (i.token->text == "(" or
                    i.token->text == "[" or
                    i.token->text == "{")
                {
                    i.elements = bracketing(i.elements);
                    o.elements += std::move(i);
                }
                else
                {
                    o.elements += std::move(i);
                }
            }

            if (not o.elements.empty())
                output += std::move(o);
    
            return output;
        }
    };

    str print (const element & e)
    {
        str s;
        str t = e.token ? e.token->text : "";
        for (auto & ee : e.elements) {
            s += print(ee);
            if (t == "(") s += ",";
            if (t == "[") s += ",";
            if (t == "{") s += ",";
            s += " ";
        }
        s.truncate();
        if (t == "(") s.truncate();
        if (t == "[") s.truncate();
        if (t == "{") s.truncate();
        s = t + s;
        if (t == "(") s += ")";
        if (t == "[") s += "]";
        if (t == "{") s += "}";
        s.replace_all("\n", "\\n");
        return s;
    }

    void print (const cluster & c, array<str> & lines, int indent = 0)
    {
        str line;
        for (auto & e : c.elements)
        line += print(e) + " ";
        line.truncate();

        if (line != "") line = str(' ', indent) + line;
        if (line != "") lines += line;

        for (auto && cc : c.clusters)
            print(cc, lines, indent + 4);
    }
}

