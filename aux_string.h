#pragma once
#include <string> 
#include <variant> 
#include <stdexcept> 
#include <functional>
#include "aux_array.h"
namespace aux
{
    using array_of_chars = array<char, std::string>;

    struct str : array_of_chars
    {
        using base = array_of_chars;
        using base:: array;

        str (const char * s ) : base (s) {}
        str (const char8_t * s ) : base ((char*)s) {}
        str (char c, int n=1) { if (n > 0) *this = std::string (n,c); }
        str (std::string_view s) { *this = std::string (s); }

        str (const_iterator f, const_iterator l) { *this = std::string (f, l); }
        str (const char *   f, const char *   l) { *this = std::string (f, l); }

        explicit str (array<char> text) { *this = std::string(text.data(), text.data()+text.size()); }
        explicit str (array<str> lines, str delimiter = "\n") {
            for (auto line : lines) {
                *this += line;
                *this += delimiter; }
            if (size() > 0) truncate(size() - delimiter.size());
        }

        void operator += (      char   c) { std::string::operator+=(c); }
        void operator += (const char * s) { std::string::operator+=(s); }
        void operator += (const str  & s) { base::operator+=(s); }
        void operator += (      str && s) { base::operator+=(s); }

        friend bool operator == (const range & l, const char  * r) { return std::string_view(l.arr->data() + l.offset, l.length) == r; }
        friend bool operator != (const range & l, const char  * r) { return std::string_view(l.arr->data() + l.offset, l.length) != r; }
        friend bool operator == (const char  * l, const range & r) { return std::string_view(r.arr->data() + r.offset, r.length) == l; }
        friend bool operator != (const char  * l, const range & r) { return std::string_view(r.arr->data() + r.offset, r.length) != l; }

        ////////////////////////////////////////////////////////////////////////////

        struct one_of     { base chars; one_of    (str s = "") : chars(s) {}};
        struct one_not_of { base chars; one_not_of(str s = "") : chars(s) {}};

        typedef std::variant<str, one_of, one_not_of> pattern;

        static int size_of(pattern pattern) { return
            std::holds_alternative<str>(pattern) ? std::get<0>(pattern).size() :
            std::holds_alternative<one_of>(pattern) ? (std::get<1>(pattern).chars.size() == 0 ? 0 : 1) :
            std::holds_alternative<one_not_of>(pattern) ? (std::get<2>(pattern).chars.size() == 0 ? 0 : 1) : 0;
        }

        struct start { int offset; bool from_end; };
        static start start_from     (int offset = 0){ return start {offset, false}; }
        static start start_from_end (int offset = 0){ return start {offset, true }; }

        str substr (range range) const { return str(range); }
        str substr (start start) const { return start.from_end ? upto(size()-start.offset) : from(start.offset); }

        auto find (pattern pattern, start start = start_from(0)) const
        {
            const_iterator i;

            if (size_of(pattern) == 0) throw std::logic_error("str::find : empty pattern");

            if (std::holds_alternative<str>(pattern))
            {
                if (start.from_end) { auto it = 
                    std::search(rbegin() + start.offset, rend(),
                    std::boyer_moore_horspool_searcher(
                    std::get<str>(pattern).rbegin(),
                    std::get<str>(pattern).rend ()));
                    i = it == rend() ? end() : std::next(it).base();
                }

                else i = 
                    std::search(begin() + start.offset, end(),
                    std::boyer_moore_horspool_searcher(
                    std::get<str>(pattern).begin(),
                    std::get<str>(pattern).end ()));
            }

            else

            if (std::holds_alternative<one_of>(pattern))
            {
                auto n = start.from_end ?
                    find_last_of (std::get<one_of>(pattern).chars, size() - 1 - start.offset):
                    find_first_of(std::get<one_of>(pattern).chars, start.offset);
                i = n == npos ? end() : begin() + n;
            }

            else

            if (std::holds_alternative<one_not_of>(pattern))
            {
                auto n = start.from_end ?
                    find_last_not_of (std::get<one_not_of>(pattern).chars, size() - 1 - start.offset):
                    find_first_not_of(std::get<one_not_of>(pattern).chars, start.offset);
                i = n == npos ? end() : begin() + n;
            }

            int offset = (int)(i - begin());
            int length = i == end() ? 0 : size_of(pattern);
            return from(offset).size(length);
        }

        bool contains (pattern pattern, start start = start_from(0)) const { return bool(find(pattern, start)); }

        bool contains_only (pattern pattern, start start = start_from(0)) const { return
            std::holds_alternative<str>(pattern) ?         substr(start) == std::get<0>(pattern) :
            std::holds_alternative<one_of>(pattern) ? bool(!find(one_not_of(std::get<1>(pattern).chars), start)) :
            std::holds_alternative<one_not_of>(pattern) ? bool(!find(one_of(std::get<2>(pattern).chars), start)) :
            false;
        }

        /////////////////////////////////////////////////

        enum class delimiter { exclude, to_the_left, to_the_right };

        bool split_by (pattern pattern, start start, str& str1, str& str2, delimiter delimiter = delimiter::exclude) const
        {
            auto range = find (pattern, start); int n = range.offset; int m = range.length;
            str1 = m > 0 ? upto (n     + (delimiter == delimiter::to_the_left  ? m : 0)) : *this;
            str2 = m > 0 ? from (n + m - (delimiter == delimiter::to_the_right ? m : 0)) : str{};
            return m > 0;
        }
        bool split_by (pattern pattern, str& str1, str& str2, delimiter delimiter = delimiter::exclude) const
        {
            return split_by (pattern, start_from(0), str1, str2, delimiter);
        }
        array<str> split_by (pattern pattern) const
        {
            array<str> result;
            if (size() == 0) return result;
            int start = 0; while (true) {
                auto range = find(pattern, start_from(start));
                bool found = bool(range);
                result += from(start).size(found ? range.offset - start : size() - start);
                start = range.offset + range.length;
                if (start >= size()) { if (found) result += ""; break; }
            }
            return result;
        }
        array<str> split () const
        {
            array<str> result;
            if (size() == 0) return result;
            std::size_t start = 0, end = 0;
            while ((end = std::string::find('\n', start)) != std::string::npos) {
                auto n = end - start; if (n > 0 && (*this)[n] == '\r') n--;
                result.push_back(std::string::substr(start, n));
                start = end + 1;
            }
            if (start < std::string::size())
                result.push_back(std::string::substr(start));
            return result;
        }

        ////////////////////////////////////////////////////////////////////////////

        bool starts_with (const range  r) const { return upto(r.length) == r.upto(r.offset+size()); }
        bool starts_with (const str  & s) const { return starts_with(s.whole()); }
        bool starts_with (const char * s) const
        {
            for (char c : *this) {
                if (*s == '\0') return true;
                if (*s++ != c) return false;
            }
            return *s == '\0';
        }
        bool ends_with (const range  r) const { return from(size()-r.length) == r.upto(r.offset+size()); }
        bool ends_with (const str  & s) const { return ends_with(s.whole()); }

        ////////////////////////////////////////////////////////////////////////////

        template<class I>
        auto insert(const_iterator i, I f, I l) { return base::insert(i, f, l); }
        auto insert(int i, const str   & s    ) { return base::insert(i, s); }
        auto insert(int i, const range & r    ) { return base::insert(i, r); }

        void triml (const str & chars = " "){
            auto r = find(one_not_of{chars});
            if (!r) clear(); else upto(r.offset).erase();
        }
        void trimr (const str & chars = " "){
            auto r = find(one_not_of{chars}, start_from_end());
            if (!r) clear(); else from(r.offset+1).erase();
        }
        void strip (const str & chars = " "){
             trimr(chars);
             triml(chars);
        }

        void align_left (int n, char padding = ' ') {
            if (size() < n) *this += str(padding, n - size());
        }
        void align_right (int n, char padding = ' ') {
            if (size() < n) *this = str(padding, n - size()) + *this;
        }
        str left_aligned  (int n, char padding = ' ') { str s = *this; s.align_left (n, padding); return s; }
        str right_aligned (int n, char padding = ' ') { str s = *this; s.align_right(n, padding); return s; }

        int replace_all (str from, str to) {
            int pos = 0, nn = 0;
            while (true) {
                auto range = find(from, start_from(pos)); if (range.empty()) break;
                pos = range.offset; range.replace_by(to);
                pos += to.size ();
                nn++;
            };
            return nn;
        }
        int replace_all (char c, int ccc, str to) // c repeated exactly ccc times
        {
            int pos = 0, nn = 0;
            while (true) {
                auto range = find(str(c, ccc), start_from(pos)); if (range.empty()) break;
                pos = range.offset;
                if (pos+ccc >= size() || at(pos+ccc) != c) {
                    range.replace_by(to);
                    pos += to.size ();
                    nn++;
                }
                else {
                    range = find(one_not_of{str(c, 1)}, start_from(pos)); if (range.empty()) break;
                    pos = range.offset;
                }
            }
            return nn;
        }
    
        void canonicalize ()
        {
            for (char & c : *this) if( c == '\t' || c  == '\r' || c == '\n' )  c = ' ';
            base::erase(std::unique(begin(), end(), [](char c1, char c2){ return c1 == ' ' && c2 == ' '; }), end());
            strip();
        }

        ////////////////////////////////////////////////////////////////////////////

        bool ascii_isalnum () const {
            for (char c : *this)
                if((c < '0') || ('9' < c &&
                    c < 'A') || ('Z' < c &&
                    c < 'a') || ('z' < c))
                    return false;
            return true;
        }

        static char ascii_tolower (char c) { return 'A' <= c && c <= 'Z' ? c - 'A' + 'a' : c; }
        static char ascii_toupper (char c) { return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c; }

        str ascii_lowercased () const { str s = *this; std::transform(s.begin(), s.end(), s.begin(), ascii_tolower); return s; }
        str ascii_uppercased () const { str s = *this; std::transform(s.begin(), s.end(), s.begin(), ascii_toupper); return s; }
    };
}
namespace std
{
    template <> struct hash<aux::str>
    {
        std::size_t operator()(const aux::str & s) const
        {
            using std::size_t;
            using std::hash;
            using std::string;
            return hash<string>()(s);
        }
    };
}
