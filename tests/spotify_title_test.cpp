// Host-side test for the Spotify window-title logic (no Windows headers needed).
#include <cstdio>
#include <string>

#include "../src/spotify_title.h"

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("FAIL line %d: %s\n", __LINE__, #cond); failures++; } } while (0)

int main() {
    using namespace spotify;
    CHECK(NowPlayingFromTitle(L"Daft Punk - Get Lucky") == L"Daft Punk - Get Lucky");
    CHECK(NowPlayingFromTitle(L"Emicida, Rael - Mandume") == L"Emicida, Rael - Mandume");
    CHECK(NowPlayingFromTitle(L"Spotify").empty());
    CHECK(NowPlayingFromTitle(L"Spotify Premium").empty());
    CHECK(NowPlayingFromTitle(L"spotify free").empty());
    CHECK(NowPlayingFromTitle(L"Advertisement").empty());
    CHECK(NowPlayingFromTitle(L"").empty());
    CHECK(NowPlayingFromTitle(L"Default IME").empty());
    // A song that merely starts with "Spotify" is still a song.
    CHECK(NowPlayingFromTitle(L"Spotify Wrapped - Intro") == L"Spotify Wrapped - Intro");

    CHECK(Shorten(L"short", 10) == L"short");
    CHECK(Shorten(L"abcdefghij", 10) == L"abcdefghij");
    CHECK(Shorten(L"abcdefghijk", 10) == L"abcdefghi…");
    CHECK(Shorten(L"ab   cdefghijk", 6) == L"ab…");  // trailing spaces before the cut are dropped
    CHECK(Shorten(L"abc", 1) == L"abc");                   // too small to shorten sensibly

    if (failures) return 1;
    std::printf("spotify title tests passed\n");
    return 0;
}
