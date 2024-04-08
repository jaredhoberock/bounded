#pragma once

#include "constant.hpp"
#include <concepts>
#include <iostream>
#include <limits>
#include <type_traits>

// XXX do we really require a non-negative bound?
template<std::integral auto b>
  requires (b >= 0)
class bounded
{
  public:
    using value_type = decltype(b);

    constexpr static value_type bound = b;

    constexpr bounded(value_type value) noexcept
      : value_{value}
    {}

    bounded(const bounded&) = default;

    constexpr value_type value() const noexcept
    {
      return value_;
    }

    // conversion to value_type
    constexpr operator value_type () const noexcept
    {
      return value();
    }

    friend std::ostream& operator<<(std::ostream& os, bounded self)
    {
      return os << self.value_;
    }

    // unary operators
    constexpr bounded operator+() const noexcept { return *this; }
    constexpr bounded operator-() const noexcept { return -value_; }
    constexpr bounded& operator++() noexcept { ++value_; return *this; }
    constexpr bounded& operator--() noexcept { --value_; return *this; }
    constexpr bounded operator++(int) noexcept { return value_++; }
    constexpr bounded operator--(int) noexcept { return value_--; }

    // binary operators against a value_type
#define BIN_OP_VALUE(OP)\
    constexpr auto operator OP (value_type rhs) const noexcept { return value_ OP rhs; }

    // relational operators
//    // XXX do we need these, or can we rely on conversion to value_type?
//    BIN_OP_VALUE(==)
//    BIN_OP_VALUE(!=)
//    BIN_OP_VALUE(<)
//    BIN_OP_VALUE(<=)
//    BIN_OP_VALUE(>)
//    BIN_OP_VALUE(>=)

    // arithmetic operators
    BIN_OP_VALUE(+)
    BIN_OP_VALUE(-)
    BIN_OP_VALUE(*)
    BIN_OP_VALUE(%)
    BIN_OP_VALUE(|)
    BIN_OP_VALUE(^)
    BIN_OP_VALUE(<<)
#undef BIN_OP_VALUE

    // division allows us to keep our bound
    constexpr bounded operator/(value_type rhs) const noexcept
    {
      return value_ / rhs;
    }

    // shift left allows us to keep our bound
    constexpr bounded operator>>(value_type rhs) const noexcept
    {
      return value_ >> rhs;
    }

    // modulus with a bounded yields a bounded
    friend constexpr bounded<bound-1> operator%(value_type lhs, bounded rhs) noexcept
    {
      return {lhs % rhs.value()};
    }

    // binary operators against a bounded
    template<std::integral auto other_bound>
      requires (other_bound >= 0)
    constexpr bounded<bound + other_bound> operator+(bounded<other_bound> rhs) const noexcept
    {
      return {value_ + rhs.value()};
    }

    template<std::integral auto other_bound>
      requires (other_bound >= 0)
    constexpr auto operator-(bounded<other_bound> rhs) const noexcept
    {
      if constexpr(other_bound <= bound)
      {
        return bounded<bound>(value_ - rhs.value());
      }
      else
      {
        return value_ - rhs.value();
      }
    }

    template<std::integral auto other_bound>
      requires (other_bound >= 0)
    constexpr bounded<bound * other_bound> operator*(bounded<other_bound> rhs) const noexcept
    {
      return {value_ * rhs.value()};
    }

    template<std::integral auto other_bound>
      requires (other_bound >= 0)
    constexpr bounded<other_bound-1> operator%(bounded<other_bound> rhs) const noexcept
    {
      return {value_ % rhs.value()};
    }

    // binary operators against a constant
    template<std::integral auto c>
      requires (bound + c >= 0)
    constexpr bounded<bound + c> operator+(constant<c>) const noexcept
    {
      return {value_ + c};
    }

    template<std::integral auto c>
      requires (bound + c >= 0)
    friend constexpr bounded<c + bound> operator+(constant<c>, bounded rhs) noexcept
    {
      return {c + rhs.value()};
    }

    template<std::integral auto c>
    constexpr auto operator-(constant<c>) const noexcept
    {
      if constexpr (0 <= c and c <= bound)
      {
        return bounded<bound - c>(value_ - c);
      }
      else
      {
        return value_ - c;
      }
    }

    template<std::integral auto c>
      requires (bound * c >= 0)
    constexpr bounded<bound * c> operator*(constant<c>) const noexcept
    {
      return {value_ * c};
    }

    template<std::integral auto c>
      requires (bound * c >= 0)
    friend constexpr bounded<c * bound> operator*(constant<c>, bounded rhs) noexcept
    {
      return {c * rhs.value()};
    }

    template<std::integral auto c>
      requires (c > 0)
    constexpr bounded<c-1> operator%(constant<c>) const noexcept
    {
      return {value_ % c};
    }

    template<std::integral auto c>
    friend constexpr bounded<bound - 1> operator%(constant<c>, bounded rhs) noexcept
    {
      return {c % rhs.value()};
    }

  private:
    value_type value_;
};

template<std::integral auto b>
class std::numeric_limits<bounded<b>> : public std::numeric_limits<decltype(b)>
{
  public:
    static constexpr auto max() noexcept
    {
      return b;
    }
};

