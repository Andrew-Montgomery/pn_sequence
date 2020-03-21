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

#include "pn_sequence.h"

// LFSR taps for up to 31 degree polynomials
// For maximal length PN sequences
static const uint32_t maximalLengthTaps[] =
{
    0,
    0,
    0,
    PNSequenceGetPoly(3,2,0),
    PNSequenceGetPoly(4,3,0),
    PNSequenceGetPoly(5,3,0),
    PNSequenceGetPoly(6,5,0),
    PNSequenceGetPoly(7,6,0),
    PNSequenceGetPoly(8,6,5,4,0),
    PNSequenceGetPoly(9,5,0),
    PNSequenceGetPoly(10,7,0),
    PNSequenceGetPoly(11,9,0),
    PNSequenceGetPoly(12,6,4,1,0),
    PNSequenceGetPoly(13,4,3,1,0),
    PNSequenceGetPoly(14,5,3,1,0),
    PNSequenceGetPoly(15,14,0),
    PNSequenceGetPoly(16,15,13,4,0),
    PNSequenceGetPoly(17,14,0),
    PNSequenceGetPoly(18,11,0),
    PNSequenceGetPoly(19,6,2,1,0),
    PNSequenceGetPoly(20,17,0),
    PNSequenceGetPoly(21,19,0),
    PNSequenceGetPoly(22,21,0),
    PNSequenceGetPoly(23,18,0),
    PNSequenceGetPoly(24,23,22,17,0),
    PNSequenceGetPoly(25,22,0),
    PNSequenceGetPoly(26,6,2,1,0),
    PNSequenceGetPoly(27,5,2,1,0),
    PNSequenceGetPoly(28,25,0),
    PNSequenceGetPoly(29,27,0),
    PNSequenceGetPoly(30,6,4,1,0),
    PNSequenceGetPoly(31,28,0)
};
static_assert(sizeof(maximalLengthTaps) == sizeof(uint32_t) * 32, "Invalid size");

struct PNSequenceState {
    uint32_t poly;
    uint32_t state;
    uint32_t initialState;
    uint32_t order;
};

#if _WIN32
#include <nmmintrin.h>
#endif

static inline int32_t Popcount(uint32_t n)
{
#if _WIN32
    return _mm_popcnt_u32(n);
#else
    int32_t i = 0;
    for(; n != 0; n &= n - 1) {
        i++;
    }
    return i;
#endif
}

static inline uint32_t Parity(uint32_t n)
{
    return Popcount(n) & 1;
}

static inline uint32_t HighestBitSet(uint32_t n)
{
    uint32_t b = 0;

    while(n >>= 1) {
        b++;
    }

    return b;
}

uint32_t PNSequenceGetPoly(uint32_t a)
{
    return 1 << a;
}

uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b)
{
    return (1 << a) + (1 << b);
}

uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c)
{
    return (1 << a) + (1 << b) + (1 << c);
}

uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
    return (1 << a) + (1 << b) + (1 << c) + (1 << d);
}

uint32_t PNSequenceGetPoly(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
    return (1 << a) + (1 << b) + (1 << c) + (1 << d) + (1 << e);
}

uint32_t PNSequenceGetMLPoly(uint32_t M)
{
    if(M < 0 || M >= 32) {
        return 0;
    }

    return maximalLengthTaps[M];
}

int PNSequenceGetStateSize(int32_t *size)
{
    if(!size) {
        return -1;
    }

    *size = sizeof(PNSequenceState);
    return 0;
}

// Advances the generator 1 bit
static inline uint8_t PNSequenceAdvance(PNSequenceState *state)
{
    uint8_t nextBit = state->state & 1;
    uint8_t feedBack = Parity(state->state & state->poly);
    state->state = ((state->state >> 1) | (feedBack << (state->order - 1)));
    return nextBit;
}

int PNSequenceInit(PNSequenceState **state,
                   uint32_t polynomial,
                   uint32_t shift,
                   uint32_t initialState,
                   uint8_t *buffer)
{
    if(!state || !buffer || !polynomial || !initialState) {
        return -1;
    }

    PNSequenceState *statePtr = (PNSequenceState*)buffer;
    *state = statePtr;

    statePtr->poly = polynomial;
    statePtr->order = HighestBitSet(statePtr->poly);

    // Clamp seed to possible range of values
    initialState &= ((1 << statePtr->order) - 1);

    // Advance the state 'shift' steps
    statePtr->state = initialState;
    for(uint32_t i = 0; i < shift; i++) {
        PNSequenceAdvance(statePtr);
    }

    // Cache the shifted stated
    statePtr->initialState = statePtr->state;
    return 0;
}

int PNSequenceReset(PNSequenceState *state)
{
    if(!state) {
        return -1;
    }

    state->state = state->initialState;

    return 0;
}

int PNSequenceGenerate(PNSequenceState *state, uint8_t *dst, int n)
{
    if(!state || !dst || n < 0) {
        return -1;
    }

    for(int i = 0; i < n; i++) {
        dst[i] = PNSequenceAdvance(state);
    }

    return 0;
}


