// Copyright (c) 2020 Andrew Montgomery

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PN_SEQUENCE_H
#define PN_SEQUENCE_H

#include <cstdint>

// References
// https://en.wikipedia.org/wiki/Linear-feedback_shift_register
// https://www.xilinx.com/support/documentation/application_notes/xapp052.pdf

uint32_t PNSequenceGetPoly(uint32_t a);
uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b);
uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c);
uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);

// Returns a maximal length polynomial with a given order 'm'
// m can be chosen between [3,31]
static uint32_t PNSequenceGetMLPoly(uint32_t m);

// Generates maximal length (ML) pseudo-noise (PN) sequence
// Can accept up to a 32-bit polynomial (31st order)
// Uses a fibonacci LFSR
// ML Sequences repeat in 2^M - 1 bits
// Characteristics of maximal length sequence
//   Number of zeros in sequence is 2^(M-1) -1
//   Number of ones in sequence is 2^(M-1)
// Sequence returned is exactly the reverse of the MATLAB generated sequence
// Ideally, a PN sequence should have an autocorrelation similar to white noise

struct PNSequenceState;

// Inidicates the required buffer size necessary for Init
int PNSequenceGetStateSize(int32_t *size);

// Create PN generator state
// polynomial - Polynomial which determines the LFSR, represented as an unsigned int
//   Example, polynomial x^3 + x^2 + 1 = binary 1101 = unsigned int 13. 13 would be provided.
//   Assumes the polynomial supplied is a maximal length PN-sequence polynomial.
//   Order is assumed to be the highest bit set
// shift - Number of states/steps the LFSR is advanced during initialization.
// initialState - Initial state of the LFSR. This is the state used prior to any shift.
int PNSequenceInit(PNSequenceState **state, uint32_t polynomial, uint32_t shift, uint32_t initialState, uint8_t *buffer);

// Returns LFSR to initial state, if a non zero shift was provided during
//   initialization the initial state will include the shift.
int PNSequenceReset(PNSequenceState *state);

// Generate n bits in the PN sequence
// dst - Must be an array of n bytes. Each byte will be set to 0 or 1
// n - Number of bits to generate
int PNSequenceGenerate(PNSequenceState *state, uint8_t *dst, int n);

#endif // PN_SEQUENCE_H
