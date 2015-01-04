putin - command line ROM corruption utility
=====

**putin** is a small command-line utility that randomly modifies bytes in a binary file. Such a procedure is usually called "corruption" and can be used on video game ROM files for amusing effects.

The utility accepts several keys and parameters to determine the way in which a file is to be corrupted:

- **-i** (input) specifies the input file to be corrupted.

- **-O** (output) accepts the name for the output file to be created. This argument is optional; if not specified, the output name will be the input file name, with an underscore (\_) added at the beginning.

- **-o** (offset) specifies the offset (in bytes) at which the data should be corrupted. If not specified, the offset will be set to zero.

- **-s** (seed) sets the RNG seed that the program should use. When run with the same seed and parameters, the program will always corrupt the file in the same way. If not specified, the seed will be based on the current time. 

- **-e** (emulator / execute) specifies the name of the program to launch after the file is corrupted. This allows to run the emulator on the corrupted ROM without manually opening it. If not specified, no program will be launched.

- **-m** (method) allows to choose one of three methods in which bytes will be modified. 0 (default) replaces bytes with random values, 1 shifts them by a specific value, 2 replaces them with the specific value. This value can be specified with the **-v** (value) key.

- **-c** (count) accepts the size of area to corrupt. For example, if -o is 16384 and -s is 2048, then bytes 16384 ~ 18431 will be corrupted.

- **-p** (probability) is the probability that a byte in the area will be corrupted. The value ranges from 1 to 256, where 1 means that there's a 1/256 chance of modification, and 256 means that every byte will be modified.

- **-v** (value) specifies the specific shift or byte value to use if the method chosen by **-m** is not "replace with random values".

- **-h** and **-?** (help) show the help message, while **-V** shows the version info.

Parameters that accept a number (-o, -s, -m, -c, -p, -v) accept hexadecimal and octal values starting with "0x" and "0" respectively - therefore, **0x8**000 = 32768 and **0**4000 equals 2048.

Example command:

    ./putin -i "Contra (U) [!].nes" -o 0x9010 -c 0x1000 -p 64 -e mednafen

Corrupts bytes from 0x9010 (36880) to 0xA00F (40975) with a probability of 64/256 (1/4), replacing them with random values, then executes [Mednafen](http://mednafen.sourceforge.net/) to run the resulting ROM file.

(If this exact command is performed correctly on a corresponding NES ROM file for Contra, the game's title screen should become corrupted.)

Disclaimer
=====

This is a small worthless tool I coded in a day or two quite a long time ago. You may also find it worthless, buggy or badly written. This tool doesn't overwrite input files, but either way, like the LICENSE file says, the author isn't legally responsible for the results of this tool's use.

The author also acknowledges that video game ROM files are still property of their copyright owners. You can either venture into the questionably legal territory of obtaining ROM files for commercial video games or, instead, download some legal free ROMs to use from [PDRoms](http://pdroms.de).
