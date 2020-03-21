#include <iostream>

#include "pn_sequence.h"

int main()
{
    PNSequenceState *state;

    // Retrieve state size and allocate memory
    int32_t pBufSize = 0;
    PNSequenceGetStateSize(&pBufSize);
    uint8_t *pBuf = (uint8_t*)malloc(pBufSize);

    // Get polynomial for x^3 + x^2 + 1
    uint32_t m3 = PNSequenceGetPoly(3, 2, 0);

    // Initialize generator, no shift, initial state of 0x0001
    PNSequenceInit(&state, m3, 0, 1, pBuf);

    // Generate the full sequence
    uint8_t bits[7];
    PNSequenceGenerate(state, bits, 7);

    // Clean up
    free(pBuf);

    return 0;
}
