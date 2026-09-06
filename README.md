# PlaneSpotter

PlaneSpotter is a small ESP32-S3 project for the **LILYGO T-Display-S3 (1.9\" 170×320 display)**. It connects to Wi-Fi, retrieves nearby live ADS-B aircraft data, and will present a text-only list of interesting aircraft around a configured observation point.

The primary use case is spotting aircraft approaching or departing **Zurich Airport (ZRH)**, with arrivals intended to receive priority in the final UI.

## Current status

Early proof of concept:

- T-Display-S3 boots and displays status text
- Wi-Fi connection works
- HTTPS request code is present
- ADSB.lol radius endpoint is being tested
- raw API response is printed to Serial
- refresh interval is 15 seconds

JSON parsing and the final aircraft list UI are not implemented yet.

## Hardware

- LILYGO T-Display-S3 ESP32-S3
- 1.9-inch ST7789V IPS display, 170×320
- USB power
- 2.4 GHz Wi-Fi connection

## Development environment

Preferred environment: **Arduino IDE**.

### Arduino libraries

Install the following libraries before compiling:

- ESP32 Arduino core
- LilyGo display library (`LilyGo-display-library`)
- LovyanGFX
- ArduinoJson

The sketch also uses the ESP32 core libraries:

- `WiFi`
- `WiFiClientSecure`
- `HTTPClient`

## Configuration

The configured observation point is:

- Latitude: `47.39724126370273`
- Longitude: `8.59600761481239`
- Desired radius: `25 km`
- Refresh interval: `15 seconds`

Project-level settings are in `config.h`.

### Wi-Fi credentials

Copy the example secrets file:

```bash
cp secrets.example.h secrets.h
```

Then edit `secrets.h`:

```cpp
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"
```

`secrets.h` is ignored by Git and should never be committed.

## ADSB.lol User-Agent

ADSB.lol rejects overly generic User-Agent strings and requests valid contact/project identification.

After creating this repository on GitHub, edit `config.h` and replace:

```text
YOUR_GITHUB_USERNAME
```

with your GitHub username so that the header points back to the real public repository, for example:

```cpp
#define API_USER_AGENT \
  "PlaneSpotter/0.1 (+https://github.com/example/PlaneSpotter)"
```

The current public API documentation states that the ADSB.lol API is available to everyone: <https://www.adsb.lol/docs/open-data/api/>

## Planned aircraft output

The final screen is intended to remain **text only**. No map, radar, aircraft icons, dots, or geographic visualization.

For each aircraft, the desired information is:

- callsign / airline
- aircraft type
- departure airport code
- destination airport code
- altitude
- vertical rate
- distance from observer
- bearing / direction
- approaching or receding state

Priority order is expected to be:

1. aircraft arriving at ZRH
2. aircraft departing ZRH
3. other nearby aircraft

A future example might look like:

```text
ARRIVALS TO ZRH

SWR1077 A320
FRA > ZRH
6.2 km  4300 ft
SW  -1180 fpm
```

## Planned implementation steps

1. Confirm a successful ADSB.lol API request.
2. Parse the live aircraft JSON with ArduinoJson.
3. Normalize the response into an internal `Aircraft` structure.
4. Calculate observer-to-aircraft distance and bearing locally.
5. Track previous aircraft state to detect approaching/receding traffic.
6. Add origin/destination route enrichment.
7. Prioritize ZRH arrivals and departures.
8. Build the compact 170×320 text UI.

## Security note

The current prototype uses:

```cpp
client.setInsecure();
```

This disables TLS certificate verification and is acceptable only as a development shortcut. It should be replaced with proper certificate validation before considering the firmware production-ready.
