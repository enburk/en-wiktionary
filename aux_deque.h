#pragma once
#include <deque>
#include "aux_abc.h"
namespace aux
{
    template<class type> struct deque : public std::deque<type>
    {
        using base = std::deque<type>;

        int size () const { return (int)(base::size()); }

        deque (            ) = default;
        deque (deque const&) = default;
        deque (deque     &&) = default;
        deque (base const&c) : base(c) {}
        deque (base     &&c) : base(std::move(c)) {}

        explicit deque (array<type> const& a) : base(a.begin(), a.end()) {}
        explicit deque (array<type>     && a) : base(
            std::make_move_iterator(a.begin()),
            std::make_move_iterator(a.end())) {}

        auto& operator =  (deque const& a) { base::operator = (a); return *this; }
        auto& operator =  (deque     && a) { base::operator = (std::move(a)); return *this; }

        void  operator += (type  const& e) { base::push_back(e); }
        void  operator += (type      && e) { base::push_back(std::move(e)); }

        void  operator += (deque const& a) { base::insert(base::end(), a.begin(), a.end()); }
        void  operator += (deque     && a) { base::insert(base::end(),
            std::make_move_iterator(a.begin()),
            std::make_move_iterator(a.end())); }
    };
}
