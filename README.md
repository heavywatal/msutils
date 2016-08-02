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
% ms 30 4 -t 3.0 -I 2 24 6 -ej 0.5 2 1 | sample_stats++
pi_1    S_1     D_1     tH_1    pi_2    S_2     D_2     tH_2    Fst     Kst
2.717391        12      -0.529610       3.369565        2.266667        5       0.196505        1.733333        0.525328        0.356234
5.217391        13      1.730140        4.347826        5.266667        14      -0.866008       4.733333        0.172311        0.094278
4.326087        14      0.539218        2.630435        1.266667        3       -0.185445       0.733333        0.286031        0.166883
3.644928        14      -0.097285       1.920290        1.600000        4       -0.469825       0.800000        0.523188        0.354269
```

The program is written in C++ using [libsequence](https://github.com/molpopgen/libsequence).
```
% brew install homebrew/science/libsequence
% make sample_stats++
% cp sample_stats++ /somewhere/in/$PATH
```
