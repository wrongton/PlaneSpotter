#pragma once

// Observer location
constexpr double OBSERVER_LAT = 47.39724126370273;
constexpr double OBSERVER_LON = 8.59600761481239;

// Desired local display/search radius in kilometres.
constexpr float RADIUS_KM = 25.0f;

// Current ADSB.lol proof-of-concept endpoint distance parameter.
// Kept separate until the API distance semantics are confirmed.
constexpr int API_RADIUS = 20;

// Poll live aircraft data every 15 seconds.
constexpr uint32_t REFRESH_INTERVAL_MS = 15000;

// ADSB.lol rejects generic User-Agent strings. After creating the GitHub repo,
// replace YOUR_GITHUB_USERNAME with the real repository owner.
#define API_USER_AGENT \
  "PlaneSpotter/0.1 (+https://github.com/wrongton/PlaneSpotter)"
