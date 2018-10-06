# JSON-WEB-Api for San Andreas Multiplayer #
This plugin allows you to host a web api for your SA-MP Server.
It's still work in progress, not all rcon features are implemented yet.

## Features
- Very fast
- More informations than rcon provides
- More secure

## Various api functions
|url|params|return|
|-------|-------|:-----:|
|/player/list|-|A list of players|
|/player/kick/:name|-|-|
|/player/ban/:name|-|-|
|/misc/say|{color, name, message}|-|
|/misc/echo|{color, name, message}|-|
|/misc/ip/ban|{ip,time}|-|
|/misc/ip/unban|{ip}|-|
|/settings/list|-|A list of settings|
|/settings/gamemodetext|{value}|-|
|/settings/gravity|{value}|-|
|/settings/weather|{value}|-|

## Build
#### Requirements
- [Modified version of gclient](https://github.com/timniederhausen/gclient)
- CMake >=3.1
- C++11
  - debian package: libc6-dev-i386

Synchronize dependencies from [DEPS](DEPS) File.

    gclient.py sync -v -f

You can also download it manually and place it into the external directory.

    mkdir build
    cd build
    cmake ..\samp-log-spdlog
    make

## License
- [License](LICENSE)