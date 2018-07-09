# DigitalFiltering

Get rid of those pesky other signals

## End Goal

Take a stream of 12 bit analog input, and filter for 1 kHz and 10 kHz (or any arbitrary frequency within reasonable bounds).

## Useful Links

- [choose FFT frequency](https://dsp.stackexchange.com/questions/41916/fft-frequency-bands-and-filtering)
- [FFT frequency filtering](https://www.ltu.se/cms_fs/1.36192!/file/E0005E_Lecture06_Transforms-version4.pdf)
- [more in depth filtering (I highly reccomend this one)](http://www.it.uom.gr/teaching/linearalgebra/NumericalRecipiesInC/c13-5.pdf)
- [less in depth but quicker example of above](https://www.dsprelated.com/freebooks/filters/Time_Domain_Digital_Filter.html)
- [aaand this is probably the most practical](https://batchloaf.wordpress.com/2013/12/07/simple-dft-in-c/)
- [goertzel optimization for stability (section 2.3)][1]
- [efficient streaming of goertzel](https://netwerkt.wordpress.com/2011/08/25/goertzel-filter/)
- [gist that ended up working](https://gist.github.com/sebpiq/4128537)

    [1]: http://www.math.uni.wroc.pl/~olech/metnum2/Podreczniki/(eBook)%20Introduction%20to%20Numerical%20Analysis%20-%20J.Stoer,R.Bulirsch.pdf