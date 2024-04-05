#pragma once

#include <concepts>
#include <iostream>

template<auto v>
class bounded
{
  public:
    using value_type = decltype(v);

    constexpr bounded(value_type value) noexcept
      : value_{value}
    {}

    bounded(const bounded&) = default;

    // conversion to value_type
    constexpr operator value_type () const noexcept
    {
      return value_;
    }

    friend std::ostream& operator<<(std::ostream& os, bounded self)
    {
      reutrn os << value_;
    }

    // unary operators
    constexpr bounded operator+() const noexcept { return *this; }
    constexpr bounded operator-() const noexcept { return -value_; }
    constexpr bounded& operator++() noexcept { ++value_; return *this; }
    constexpr bounded& operator--() noexcept { --value_; return *this; }
    constexpr bounded operator++(int) noexcept { return value_++; }
    constexpr bounded operator--(int) noexcept { return value_--; }

    // binary operators against a value_type
    // XXX for now, return a value_type instead of a bounded to be conservative
#define BIN_OP(OP)\
    constexpr auto operator OP (value_type rhs) const noexcept { return value_ OP rhs; }

    // relational operators
    BIN_OP(==)
    BIN_OP(!=)
    BIN_OP(<)
    BIN_OP(<=)
    BIN_OP(>)
    BIN_OP(>=)

    // arithmetic operators
    BIN_OP(+)
    BIN_OP(-)
    BIN_OP(*)
    BIN_OP(/)
    BIN_OP(%)
    BIN_OP(|)
    BIN_OP(^)
    BIN_OP(<<)
    BIN_OP(>>)

  private:
    value_type value_;
};

