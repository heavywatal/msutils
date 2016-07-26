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
% tar xzf ~/Downloads/ms.tar.gz
% make
% make install DESTDIR=${HOME}/local/bin
```

Test:
```
% ms
% ms 4 2 -t 5.0
% ms 30 4 -t 3.0 | sample_stats
```

### `sample_stats++`

`sample_stats++` processes the output of `ms` and calculates some summary statistics as the original `sample_stats` does. But the output is more simple and can be saved as a pure TSV file.
```
% ms 30 4 -t 3.0 | sample_stats++
pi      S       D       tH
5.328736        16      1.084781        4.740230
2.471264        12      -0.600858       0.632184
11.857471       28      2.435626        10.901149
8.510345        22      1.873420        7.213793
```

If the samples are generated under a model with population structure, `ms ... -I npop n1 n2 ...`, summary statistics are calculated for each subpopulation.
```
% ms 30 4 -t 3.0 -I 2 20 10 -ej 0.5 2 1 | sample_stats++
pi_1    S_1     D_1     tH_1    pi_2    S_2     D_2     tH_2    Fst     Kst
2.610526        18      -1.826448       7.915789        2.755556        9       -0.586084       2.5777780.732498        0.577907
4.363158        13      0.691813        2.689474        3.044444        7       0.975784        2.0666670.571815        0.400379
5.673684        15      1.261762        3.800000        0.400000        2       -1.400851       0.0444440.828427        0.707107
4.510526        12      1.197215        2.963158        2.933333        10      -0.754261       5.9555560.382765        0.236678
```

The program is writen in C++ using [libsequence](https://github.com/molpopgen/libsequence).
```
% brew install homebrew/science/libsequence
% make sample_stats++
% cp sample_stats++ /somewhere/in/$PATH
```
