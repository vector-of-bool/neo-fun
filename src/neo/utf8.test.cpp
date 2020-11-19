#include <neo/utf8.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Decode some bytes") {
    std::string str = "I am a string";
    auto        res = neo::next_utf8_codepoint(str.cbegin(), str.cend());
    CHECK(res.codepoint == 'I');
    str = "😉";
    res = neo::next_utf8_codepoint(str.begin(), str.end());
    CHECK_FALSE(res.error());
    CHECK(res.codepoint == 0x01'f6'09);
    CHECK(res.codepoint == U'😉');
    CHECK(res.size == 4);

    auto wink = neo::next_utf8_codepoint(str.cbegin(), str.cend());
    CHECK(wink.codepoint == U'😉');

    str = str.substr(0, 2);
    res = neo::next_utf8_codepoint(str.cbegin(), str.cend());
    CHECK(res.error() == neo::utf8_errc::need_more);
    CHECK(res.size == 2);
    str = "😉";
    // Corrupt continuation byte:
    str[2] = 0b0101'1000;
    res    = neo::next_utf8_codepoint(str.cbegin(), str.cend());
    // CHECK(res.codepoint == 0);
    CHECK(res.size == 2);
    CHECK(res.error() == neo::utf8_errc::invalid_continuation_byte);

    str = "(hello)";
    res = neo::next_utf8_codepoint(str.cbegin(), str.cend());
    CHECK_FALSE(res.error());
}

TEST_CASE("View a range of codepoints") {
    std::string     str = "I am a string";
    neo::utf8_range rng{str};
    auto            it = rng.begin();
    CHECK(it->codepoint == U'I');
    ++it;
    CHECK(it->codepoint == U' ');
    ++it;
    CHECK(it->codepoint == U'a');
    ++it;
    CHECK(it->codepoint == U'm');
    std::advance(it, 6);
    CHECK(it->codepoint == U'r');
    ++it;
    CHECK(it->codepoint == U'i');
    ++it;
    CHECK(it->codepoint == U'n');
    ++it;
    CHECK(it->codepoint == U'g');
    ++it;
    CHECK(it == rng.end());
    --it;
    CHECK(it->codepoint == 'g');
    CHECK_FALSE(it->error());
}