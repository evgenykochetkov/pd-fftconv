This is a partitioned convolution external for [Pure Data](https://github.com/pure-data/pure-data/).
Under the hood, it uses https://github.com/thirtythreeforty/FFTConvolver, which is a fork of https://github.com/grawlinson/FFTConvolver, which is in turn forked from the original https://github.com/HiFi-LoFi/FFTConvolver.

Why make this, when [\[convolve~\]](https://github.com/wbrent/convolve_tilde) and [\[else/conv~\]](https://github.com/porres/pd-else/blob/master/Source/Audio/conv~.c) already exist? Because those are GPL-licensed, and I needed something that I could use in a closed-source libpd-based project.

For build instructions, see [pd.cmake docs](https://github.com/pure-data/pd.cmake?tab=readme-ov-file#compilation).

IRs in the `./demo` folder were made by Kristoffer Ekstrand (a.k.a. Adventure Kid) and are licensed under the [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/) (see https://www.adventurekid.se/akrt/free-reverb-impulse-responses/).
