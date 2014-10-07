HIP on HLL: Approximate Distinct Counter
========================================

[![Build Status](https://travis-ci.org/iwiwi/hip-hyperloglog.svg?branch=master)](https://travis-ci.org/iwiwi/hip-hyperloglog)

This is a C++ implementation of the modern approximate distinct counter which adopts the *Historic Inverse Probability estimator (HIP)* on the *HyperLogLog counter* (*k*-partition Minwise Hashing with base-2 ranks).

Currently under development.

## References
* E. Cohen. **All-distances sketches, revisited: HIP estimators for massive graphs analysis.** In *PODS'14.*
* P. Flajolet, É. Fusy, O. Gandouet, and F. Meunier. **HyperLogLog: The analysis of a near-optimal cardinality estimation algorithm.** In *AOFA'07.*

## License
The MIT License (MIT)

Copyright (c) 2014 Takuya Akiba

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
