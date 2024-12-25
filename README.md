# Utility-Pair
Source code for the Utility Pair cards for the [Music Thing Modular Workshop System Computer](https://www.musicthing.co.uk/workshopsystem/).

For documentation and the UF2 files that can be put on a Computer card, see the [Utility Pair website](https://www.chris-j.co.uk/utility_pair/).


## Compiling
This is a [Pico SDK](https://github.com/raspberrypi/pico-sdk) cmake project. In the `Utility-Pair` directory, compile with:
```
mkdir build
cd build
cmake ..
make
```

*Warning!* The default build includes all 625 utility pair targets, and takes a few hours to compile (producing a >5GB build directory in the process). See the `CMakeLists.txt` file for how to reduce this to a subset of the pairs for a much smaller/faster build.


## Licences

`ComputerCard.h` is part of the [ComputerCard](https://github.com/TomWhitwell/Workshop_Computer/tree/main/Demonstrations%2BHelloWorlds/PicoSDK/ComputerCard) library.

Source code in the `loops/` directory is not shared under this license, and has been derived from the following, with license information linked:
- [Suggested audio for the Radio Music](https://github.com/TomWhitwell/RadioMusic/wiki/Audio-for-the-SD-Card)
- ['Rasputin' on looperman.com](https://www.looperman.com/loops/detail/382008/spector-christmas-groove-free-150bpm-pop-drum-loopnot)
- [Ashley Hannson on samplefocus.com](https://samplefocus.com/samples/sleigh-bells-c)

