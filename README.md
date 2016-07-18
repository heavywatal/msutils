# msutils

This is an unofficial installer of the coalescent simulator [ms](http://home.uchicago.edu/rhudson1/source/mksamples.html).
It builds ``ms`` with
[dSFMT](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html),
a new variant of Mersenne Twister (MT),
instead of the original ``drand48()`` and ``rand()``.

Hudson, R. R. (2002)
"Generating samples under a Wright-Fisher neutral model of genetic variation"
*Bioinformatics* **18**, 2

## Usage

First, download `ms.tar.gz` manually from [the official website](http://home.uchicago.edu/rhudson1/source/mksamples.html).
Then,
```
% git clone https://github.com/heavywatal/msutils.git
% cd msutils/
% mv ~/Downloads/ms.tar.gz ./
% make
% make install DESTDIR=${HOME}/local/bin
```
