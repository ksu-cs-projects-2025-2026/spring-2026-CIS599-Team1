#pragma once

#include "../shared/utils.h"
#include "paq6v2_library.h"

#include <vector>

// PAQ6v2 predictor-style non-IID test (bitwise).
double paq6v2_predictor_test(uint8_t *data, long len, int alph_size, int verbose, const char *label) {
    const uint8_t *bits = data;
    long bit_len = len;
    std::vector<uint8_t> bitbuf;

    // Convert to bitstream if input is not already binary.
    if (alph_size != 2) {
        int bits_per_symbol = 0;
        while ((1 << bits_per_symbol) < alph_size) bits_per_symbol++;
        if (bits_per_symbol < 1) bits_per_symbol = 1;

        bitbuf.resize(len * bits_per_symbol);
        uint8_t *pOut = &bitbuf[0];

        for (long i = 0; i < len; ++i) {
            uint8_t sym = data[i];
            for (int b = bits_per_symbol - 1; b >= 0; --b) {
                *pOut++ = (sym >> b) & 1;
            }
        }
        bits = bitbuf.data();
        bit_len = bitbuf.size();
    }

    paq6v2::Predictor predictor;

    long C = 0;
    long N = 0;
    long run_len = 0;
    long max_run_len = 0;

    for (long i = 0; i < bit_len; ++i) {
        const uint8_t y = bits[i] & 0x1;
        const int p_scaled = predictor.p();
        const int pred = (p_scaled >= (paq6v2::PSCALE / 2)) ? 1 : 0;
        ++N;
        if (pred == y) {
            ++C;
            if (++run_len > max_run_len) max_run_len = run_len;
        } else {
            run_len = 0;
        }
        predictor.update(y);
    }
    return predictionEstimate(C, N, max_run_len, 2, "PAQ6v2Predictor", verbose, label);
}