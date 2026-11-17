#pragma once
// Pure text logic for the Spotify "now playing" feature (no Windows headers, so it can be unit tested).
//
// The Spotify desktop app puts "Artist - Song" in its window title while a track is playing and goes
// back to "Spotify", "Spotify Free" or "Spotify Premium" when paused or stopped. Ads show
// "Advertisement" (or just "Spotify").
#include <string>

namespace spotify {

inline bool EqualsNoCase(const std::wstring& a, const wchar_t* b) {
    size_t i = 0;
    for (; i < a.size() && b[i]; i++) {
        wchar_t x = a[i], y = b[i];
        if (x >= L'A' && x <= L'Z') x += 32;
        if (y >= L'A' && y <= L'Z') y += 32;
        if (x != y) return false;
    }
    return i == a.size() && b[i] == 0;
}

// True when the title is one of the idle titles (nothing playing / an ad / the app's own name).
inline bool IsIdleTitle(const std::wstring& title) {
    if (title.empty()) return true;
    static const wchar_t* const kIdle[] = { L"Spotify", L"Spotify Free", L"Spotify Premium", L"Spotify Premium Family",
                                            L"Advertisement", L"Spotify - Web Player", L"Default IME", L"MSCTFIME UI" };
    for (const wchar_t* idle : kIdle)
        if (EqualsNoCase(title, idle)) return true;
    return false;
}

// Shortens `s` to at most `maxChars` characters, ending with an ellipsis when it was cut.
inline std::wstring Shorten(const std::wstring& s, size_t maxChars) {
    if (maxChars < 2 || s.size() <= maxChars) return s;
    std::wstring t = s.substr(0, maxChars - 1);
    while (!t.empty() && t.back() == L' ') t.pop_back();
    return t + L"…";
}

// Returns the text to show for a Spotify window title, or "" when nothing is playing.
inline std::wstring NowPlayingFromTitle(const std::wstring& title) {
    if (IsIdleTitle(title)) return L"";
    return title;
}

}  // namespace spotify
