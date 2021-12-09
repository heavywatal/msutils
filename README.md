# msutils

This is an unofficial installer of the coalescent simulator [ms](http://home.uchicago.edu/rhudson1/source/mksamples.html).
It builds `ms` with a decent random number generator,
a variant of Mersenne Twister called [dSFMT](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html),
instead of the original `drand48()` and `rand()`.

Hudson, R. R. (2002)
"Generating samples under a Wright-Fisher neutral model of genetic variation"
*Bioinformatics* **18**, 2

## Usage

First, download `ms.tar.gz` manually from [the official website](http://home.uchicago.edu/rhudson1/source/mksamples.html).
Then,
```sh
git clone https://github.com/heavywatal/msutils.git
cd msutils/
tar xzf ~/Downloads/ms.tar.gz
make
make install DESTDIR=${HOME}/local/bin
```

Test:
```sh
ms
ms 4 2 -t 5.0
ms 30 4 -t 3.0 | sample_stats
```

### `sample_stats++`

`sample_stats++` processes the output of `ms` and calculates some summary statistics as the original `sample_stats` does. But the output is simpler and can be saved as a pure TSV file.
```sh
ms 30 4 -t 3.0 | sample_stats++
# pi      S       D       tH
# 5.328736        16      1.084781        4.740230
# 2.471264        12      -0.600858       0.632184
# 11.857471       28      2.435626        10.901149
# 8.510345        22      1.873420        7.213793
```

If the samples are generated under a model with population structure, `ms ... -I npop n1 n2 ...`, summary statistics are calculated for each subpopulation.
```sh
ms 30 4 -t 3.0 -I 2 24 6 -ej 0.5 1 2 | sample_stats++
# pi_1    S_1     D_1     tH_1    pi_2    S_2     D_2     tH_2    Fst
# 3.858696        14      0.102469        4.228261        2.066667        5       -0.314657    0.733333        0.579264
# 6.173913        18      1.013146        5.304348        2.266667        4       1.593189     1.733333        0.276522
# 1.735507        10      -1.174108       1.307971        3.000000        9       -1.422837    5.400000        0.488053
# 5.438406        18      0.462681        6.996377        6.866667        14      0.736375     7.933333        0.047349
```

Use `summstats` if you want summary statistics of whole population:
```sh
ms 30 4 -t 3.0 -I 2 24 6 -ej 0.5 1 2 | summstats
# pi      S       D       tH
# 3.627586        20      -0.980626       1.475862
# 4.696552        16      0.553175        3.924138
# 3.321839        16      -0.602825       2.540230
# 7.873563        20      1.949895        7.367816
```

The program is written in C++ using [molpopgen/libsequence](https://github.com/molpopgen/libsequence).
```sh
brew install brewsci/bio/libsequence
make libseq
make install-libseq
```

You may need to modify `LIBSEQUENCE` variable if you installed libsequence manually without Homebrew, e.g.,
```sh
make LIBSEQUENCE=${HOME}/local libseq
```
