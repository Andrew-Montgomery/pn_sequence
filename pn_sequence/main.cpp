#include <iostream>

#include "pn_sequence.h"

int main()
{
    PNSequenceState *state;
    int32_t pBufSize = 0;
    PNSequenceGetStateSize(&pBufSize);

    uint8_t *pBuf = (uint8_t*)malloc(pBufSize);

    PNSequenceInit(&state, 13, 0, 1, pBuf);

    uint8_t bits[7];
    PNSequenceGenerate(state, bits, 7);

    free(pBuf);

    return 0;
}
