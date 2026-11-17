#pragma once
// "Now playing" from the Spotify desktop app (Windows). Reads the title of Spotify's window, which shows
// "Artist - Song" while a track plays. Nothing is sent anywhere and Spotify is not touched.
#include <string>

namespace spotify {

// "Artist - Song" while Spotify is playing, "" when it is paused, stopped, playing an ad or not running.
// Cheap (one window enumeration); safe to call every couple of seconds from the UI thread.
std::wstring NowPlaying();

}  // namespace spotify
