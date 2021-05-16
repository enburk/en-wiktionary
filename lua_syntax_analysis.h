#pragma once
#include "lua_lexica.h"
#include "lua_syntax_parser.h"
namespace lua::syntax
{
    struct analysis
    {
        report log;
        array<token> tokens;
        array<cluster> clusters;

        analysis () = default;
        
        explicit analysis (str text)
        {
            tokens = lexica::parse(glyph::parse(text));

            clusters =
            //  schema(
                parser(tokens
                , log).output
            //  , log).output
                ;
        }
    };
}