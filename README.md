```bash
$ make bench
time g++ -std=c++2a N3000.cpp -o N3000 -ftemplate-depth=10000
40.63user 0.88system 0:41.70elapsed 99%CPU (0avgtext+0avgdata 1765712maxresident)k
3432inputs+656outputs (31major+447771minor)pagefaults 0swaps
$ g++ --version
g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```