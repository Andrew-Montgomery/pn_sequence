# pn_sequence

* Pseudo-noise (PN) generator reference implementation.
* Generates maximal length (ML) pseudo-noise sequence.
* Can accept up to a 32-bit polynomial (31st order).
* Uses a fibonacci LFSR.
* ML Sequences repeat in 2^M - 1 bits.
* Characteristics of maximal length sequence,
** Number of zeros in sequence is 2^(M-1) - 1
** Number of ones in sequence is 2^(M-1)
* Ideally, a PN sequence should have an autocorrelation similar to white noise

** References **
* https://en.wikipedia.org/wiki/Linear-feedback_shift_register
* https://www.xilinx.com/support/documentation/application_notes/xapp052.pdf