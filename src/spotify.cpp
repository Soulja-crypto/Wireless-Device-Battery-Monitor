#include "spotify.h"

#include <windows.h>

#include <cwchar>

#include "spotify_title.h"

namespace spotify {
namespace {

struct Search {
    std::wstring found;
};

bool IsSpotifyProcess(DWORD pid) {
    HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!p) return false;
    wchar_t path[MAX_PATH * 2];
    DWORD n = (DWORD)(sizeof(path) / sizeof(path[0]));
    bool ok = QueryFullProcessImageNameW(p, 0, path, &n) != 0;
    CloseHandle(p);
    if (!ok) return false;
    std::wstring s(path, n);
    size_t slash = s.find_last_of(L"\\/");
    std::wstring file = slash == std::wstring::npos ? s : s.substr(slash + 1);
    return EqualsNoCase(file, L"Spotify.exe");
}

BOOL CALLBACK EnumProc(HWND w, LPARAM lp) {
    auto* s = reinterpret_cast<Search*>(lp);
    // Spotify (an Electron/CEF app) has several windows; only the main one carries the track title.
    wchar_t cls[64] = {};
    GetClassNameW(w, cls, 64);
    if (wcscmp(cls, L"Chrome_WidgetWin_1") != 0) return TRUE;
    int len = GetWindowTextLengthW(w);
    if (len <= 0 || len > 512) return TRUE;
    std::wstring title(len + 1, L'\0');
    int got = GetWindowTextW(w, title.data(), len + 1);
    title.resize(got > 0 ? got : 0);
    std::wstring now = NowPlayingFromTitle(title);
    if (now.empty()) return TRUE;
    DWORD pid = 0;
    GetWindowThreadProcessId(w, &pid);
    if (!pid || !IsSpotifyProcess(pid)) return TRUE;
    s->found = now;
    return FALSE;  // stop at the first match
}

}  // namespace

std::wstring NowPlaying() {
    Search s;
    EnumWindows(EnumProc, reinterpret_cast<LPARAM>(&s));
    return s.found;
}

}  // namespace spotify
