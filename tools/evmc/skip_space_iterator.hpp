// EVMC: Ethereum Client-VM Connector API.
// Copyright 2022 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <iterator>

namespace evmc
{
/// The constexpr variant of std::isspace().
inline constexpr bool isspace(char ch) noexcept
{
    // Implementation taken from LLVM's libc.
    return ch == ' ' || (static_cast<unsigned>(ch) - '\t') < 5;
}

/// The input filter iterator which skips whitespace characters from the base input iterator.
template <typename BaseIterator>
struct skip_space_iterator
{
    using difference_type = typename std::iterator_traits<BaseIterator>::difference_type;
    using value_type = typename std::iterator_traits<BaseIterator>::value_type;
    using pointer = typename std::iterator_traits<BaseIterator>::pointer;
    using reference = typename std::iterator_traits<BaseIterator>::reference;
    using iterator_category = std::input_iterator_tag;

private:
    BaseIterator base;
    BaseIterator base_end;
    value_type value;

    constexpr void forward_to_next_value()
    {
        for (; base != base_end; ++base)
        {
            value = *base;
            if (isspace(value) == 0)
                break;
        }
    }

public:
    constexpr skip_space_iterator(BaseIterator it, BaseIterator end) noexcept
      : base{it}, base_end{end}
    {
        forward_to_next_value();
    }

    constexpr auto operator*() { return value; }

    constexpr void operator++()
    {
        ++base;
        forward_to_next_value();
    }

    constexpr bool operator!=(const skip_space_iterator& o) { return base != o.base; }
    constexpr bool operator==(const skip_space_iterator& o) { return base == o.base; }
};
}  // namespace evmc
