template<class type> struct array : public std::vector<type>
{
    using base = std::vector<type>;
    using base::vector;

    int size () const { return (int) base::size (); }

    void operator += (const type  & e) { base::push_back (e); }
    void operator += (const array & a) { base::insert (base::end(), a.cbegin(), a.cend()); }

//    void operator += (type e) { base::push_back (std::move(e)); }
//    void operator += (const array & a) { if (base::empty()) *this = a; else base::insert (base::end(), a.cbegin(), a.cend()); }
//    void operator += (     array && a) { if (base::empty()) *this = std::move(a); else base::insert (base::end(), 
//        std::make_move_iterator(std::begin(a)),
//        std::make_move_iterator(std::end(a)));
//    }

    friend array operator + (const array & a, const type  & b) { array tt; tt += a; tt+= b; return tt; }
    friend array operator + (const type  & a, const array & b) { array tt; tt += a; tt+= b; return tt; }

    friend std::ostream & operator << (std::ostream & out, const array & a) { for (const auto & e : a) out << e << std::endl; return out; }

    bool found (const type e) const { return std::find (base::begin(), base::end(), e) != base::end(); }
    bool binary_found (const type e) const { return std::binary_search (base::begin(), base::end(), e); }
};

struct str : public std::string
{
    using base = std::string;

    str (               ) : base ( ) {}
    str (const base & s ) : base (s) {}
    str (      base &&s ) : base (std::move(s)) {}
    str (const char * s ) : base (s) {}
    str (char c, int n=1) { if (n > 0) *this = base (n,c); }
    str (const str &  s ) = default;
    str (      str && s ) = default;

    str (base::const_iterator f, base::const_iterator l) : base (f, l) {}

    int size () const { return (int) base::size (); }

    str & operator  = (const str &  s){ base::operator  = (s); return *this; }
    str & operator  = (      str && s){ base::operator  = (std::move(s)); return *this; }
    str & operator += (const str &  s){ base::operator += (s); return *this; }
    str & operator += (      str && s){ base::operator += (std::move(s)); return *this; }

    static const int max = std::numeric_limits<int>::max ();
    static const int nope = -1;

    str substr (int pos, int num) const { if (pos < 0) num += pos, pos = 0; num = std::min (num, size () - pos); return num > 0 ? base::substr (pos, num) : base (); }

    str from (int pos) const { return substr (pos, max); }
    str head (int num) const { return substr (0, num); }
    str tail (int num) const { return substr (size () - num, max); }

    struct one_of     { base chars; };
    struct one_not_of { base chars; };

    typedef std::variant<str, one_of, one_not_of> pattern;

    struct start { int offset; bool from_end; };
    static start start_from     (int offset = 0){ return start {offset, false}; }
    static start start_from_end (int offset = 0){ return start {offset, true }; }

    auto find  (pattern pattern, start start = start_from(0)) const
    {
        const_iterator i;

        if (std::holds_alternative<str>(pattern))
        {
            if (start.from_end) i = std::next(
                std::search(rbegin() + start.offset, rend(),
                std::boyer_moore_horspool_searcher(
                std::get<str>(pattern).rbegin(),
                std::get<str>(pattern).rend ())
            )).base();

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

        return i == end() ? nope : int(i - begin());
    }

    bool found (pattern pattern, start start = start_from(0)) const { return find (pattern, start) != nope; }

    enum class delimiter { exclude, to_the_left, to_the_right };

    bool split_by (pattern pattern, start start, str& s1, str& s2, delimiter delimiter = delimiter::exclude)
    {
        int n = find (pattern, start);
        int m = std::holds_alternative<str>(pattern) ? std::get<str>(pattern).size () : 1;
        s1 = n == nope ? *this : head (n     + (delimiter == delimiter::to_the_left  ? m : 0));
        s2 = n == nope ? str() : from (n + m - (delimiter == delimiter::to_the_right ? m : 0));
        return n != nope;
    }

    bool split_by (pattern pattern, str& s1, str& s2, delimiter delimiter = delimiter::exclude)
    {
        return split_by (pattern, start_from(0), s1, s2, delimiter);
    }

    array<str> split_by (pattern pattern)
    {
        array<str> a;
        str s, ss = *this;
        while (ss != ""){ ss.split_by(pattern, s, ss); a.push_back(s); };
        return a;
    }

    bool starts_with (str s) const { int n = s.size(); if (size() < n) return false; auto i =  begin(); auto j = s. begin(); while (n--) if (*i++ != *j++) return false; return true; }
    bool ends_with   (str s) const { int n = s.size(); if (size() < n) return false; auto i = rbegin(); auto j = s.rbegin(); while (n--) if (*i++ != *j++) return false; return true; }

    str lowercased () const { str s = *this; std::transform(s.begin(), s.end(), s.begin(), ::tolower); return s; }
    str uppercased () const { str s = *this; std::transform(s.begin(), s.end(), s.begin(), ::toupper); return s; }

    void append    (                  str s){ *this += s; }
    void insert    (int pos,          str s){ *this = head (pos) + s + from (pos); }
    void overwrite (int pos,          str s){ erase (pos, s.size ()); insert (pos, s); }
    void replace   (int pos, int num, str s){ erase (pos, num      ); insert (pos, s); }
    void erase     (int pos, int num = 1   ){ if (pos < 0) num += pos, pos = 0; num = std::min (num, size () - pos); if (num > 0) base::erase (pos, num); }
    void truncate  (int pos                ){ erase (pos, max); }
    void truncate  (                       ){ erase (size () - 1); }
    void strip     (const str & chars = " "){ trimr (chars); triml (chars); }
    void triml     (const str & chars = " "){ int n = find (one_not_of {chars});                    if (n == nope) clear(); else erase (0, n); }
    void trimr     (const str & chars = " "){ int n = find (one_not_of {chars}, start_from_end ()); if (n == nope) clear(); else truncate (n+1); }

    int replace_all (str from, str to){ int pos = 0, nn = 0; while ((pos = find (from, start_from(pos))) != nope){ replace (pos, from.size (), to); pos += to.size (); nn++; }; return nn; }

    void canonicalize ()
    {
        for (char & c : *this) if( c == '\t' || c  == '\r' || c == '\n' )  c = ' ';
        base::erase(std::unique(begin(), end(), [](char c1, char c2){ return c1 == ' ' && c2 == ' '; }), end());
        strip();
    }
};

namespace std
{
    template <> struct hash<str>
    {
        std::size_t operator()(const str& s) const
        {
            using std::size_t;
            using std::hash;
            using std::string;
            return hash<string>()(s);
        }
    };
}