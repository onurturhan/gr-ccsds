# gr-ccsds: CCSDS Telemetry and Telecommand Transceivers

The `gr-ccsds` GNU Radio out-of-tree (OOT) module provides a set
of transceivers implementing the CCSDS recommendations for telemetry
and telecommand synchronization and channel coding.

The implementation follows the specifications desscribed in the CCSDS blue books
[CCSDS 131.0-B-3 ](https://public.ccsds.org/Pubs/131x0b3e1.pdf), 
[CCSDS 231.0-B-3](https://public.ccsds.org/Pubs/231x0b3.pdf) 
and [CCSDS 401.0-B-28](https://public.ccsds.org/Pubs/401x0b29.pdf)

## Installation

### Requirements
* GNU Radio ( > 3.7.7 )
* CMake ( > 3.1)
* G++ (with C++11 support)
* VOLK
* git


### Installation from source

1. `git clone https://gitlab.com/librespacefoundation/gr-ccsds.git`
2. `cd gr-satnogs`
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make`
7. `sudo make install`

If this is the first time you are building the gr-satnogs module run
`sudo ldconfig`

#### Advanced
By default, the **gr-ccsds** module will use the default installation prefix.
This highly depends on the Linux distribution. You can use the `CMAKE_INSTALL_PREFIX`
variable to alter the default installation path.
E.g:

`cmake -DCMAKE_INSTALL_PREFIX=/usr ..`

Another common control option is the library suffix of the Linux distribution.
There are distributions like Fedora, openSUSE, e.t.c that their 64-bit version variant
use the `lib64` folder to store the 64-bit libraries.
On the other hand, Debain based distributions do the exact opposite. They use
`lib` directory for the libraries of the native architecture and place the 32-bit versions
on the `lib32` directory. In any case the correct library directory suffix
can be specified with the `LIB_SUFFIX` variable. For example:

`cmake -DLIB_SUFFIX=64 -DCMAKE_INSTALL_PREFIX=/usr ..`

will install the libraries at the `/usr/lib64` directory.

## Website
This module is part of the SDR Makerspace project of the European Space Agency (ESA),
implemented by Libre Space Foundation (LSF).
For more indormation please visit our [site](https://libre.space).

## License

![ESA SDR Makerspace Activity](docs/assets/ESASDRMakerspace_ICONS_8-300x78.png) 
![ESA](docs/assets/img_colorlogo_darkblue-300x131.gif)
![Libre Space Foundation](docs/assets/LSF_HD_Horizontal_Color1-300x66.png)   

&copy; 2018 [Libre Space Foundation](http://librespacefoundation.org).

Licensed under the [GPLv3](LICENSE).
