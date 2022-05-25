// EVMC: Ethereum Client-VM Connector API.
// Copyright 2022 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

#include <tools/evmc/skip_space_iterator.hpp>
#include <gtest/gtest.h>

using evmc::skip_space_iterator;

namespace
{
std::string remove_space(std::string_view in)
{
    // Copy input to additional buffer. This helps with out-of-buffer reads detection by sanitizers.
    const auto in_buffer = std::make_unique<char[]>(in.size());
    const auto begin = in_buffer.get();
    const auto end = begin + in.size();
    std::copy(in.begin(), in.end(), begin);

    // Filter the input.
    std::string out;
    std::copy(skip_space_iterator{begin, end}, skip_space_iterator{end, end},
              std::back_inserter(out));
    return out;
}
}  // namespace

TEST(skip_space_iterator, empty)
{
    EXPECT_EQ(remove_space(""), "");
    EXPECT_EQ(remove_space(" "), "");
    EXPECT_EQ(remove_space("  "), "");
}

TEST(skip_space_iterator, filter_middle)
{
    EXPECT_EQ(remove_space("x y"), "xy");
    EXPECT_EQ(remove_space("x  y"), "xy");
}

TEST(skip_space_iterator, filter_front)
{
    EXPECT_EQ(remove_space(" x"), "x");
    EXPECT_EQ(remove_space("  x"), "x");
}

TEST(skip_space_iterator, filter_back)
{
    EXPECT_EQ(remove_space("x "), "x");
    EXPECT_EQ(remove_space("x  "), "x");
}

TEST(skip_space_iterator, filter_mixed)
{
    EXPECT_EQ(remove_space(" x y z "), "xyz");
    EXPECT_EQ(remove_space("  x  y  z  "), "xyz");
}

TEST(skip_space_iterator, isspace)
{
    // Test internal isspace() compliance with std::isspace().
    // The https://en.cppreference.com/w/cpp/string/byte/isspace has the list of "space" characters.

    for (int i = int{std::numeric_limits<char>::min()}; i <= std::numeric_limits<char>::max(); ++i)
    {
        const auto c = static_cast<char>(i);
        EXPECT_EQ(evmc::isspace(c), (std::isspace(c) != 0));
        switch (c)
        {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            EXPECT_TRUE(evmc::isspace(c));
            break;
        default:
            EXPECT_FALSE(evmc::isspace(c));
            break;
        }
    }
}
