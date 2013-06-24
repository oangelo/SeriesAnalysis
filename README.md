SeriesAnalysis
==============

Various methods to analyse time series.

Entropy:
------
```bash
./time_series --bins 10000 --entropy <min> <max> -ts -f <file name> 

```
Recurrence Plot:
------

```gnuplot
set palette grey
unset colorbox
plot [0:] [0:] "< time_series -ts -ts_to_att -tau <int> -d <int> -th_std <float> -rp -p -f <file name>" matrix with image notitle

```
