#pragma once

#include "../shared/utils.h"
#include "paq6v2_library.h"

/*
 PAQ6v2 predictor-style non-IID test (bitwise).
------------------------------------------------------------------------------
- Assumes the caller provides binary input.
- Feeds bits into the PAQ6v2 predictor and compares each prediction to the
  actual next bit.
- Tracks:
     C = correct predictions
     N = total predictions
     max_run_len = longest streak of correct predictions
- Returns a prediction-based entropy estimate using these stats.
*/
double paq6v2_predictor_test(uint8_t *data, long len, int verbose, const char *label) {
    // Predictor state is updated after each observed bit so the next call to
    // p() reflects the full history seen so far.
    paq6v2::Predictor predictor;

    long C = 0;               // correct predictions
    long N = 0;               // total predictions
    long run_len = 0;         // current run length of correct predictions
    long max_run_len = 0;     // maximum run length of correct predictions 

    for (long i = 0; i < len; ++i) {
        // Each input element is treated as a single binary symbol.
        const uint8_t y = data[i] & 0x1;
        // Scaled probability that the next bit will be 1. If the probability
        // is at least 0.5, predict 1; otherwise predict 0.
        // NOTE: 0 <= p_scaled <= PSCALE.
        const int p_scaled = predictor.p();
        const int pred = (p_scaled >= (paq6v2::PSCALE / 2)) ? 1 : 0;
        ++N;
        // Count correct predictions and track the longest streak of
        // consecutive correct predictions.
        if (pred == y) {
            ++C;
            if (++run_len > max_run_len) max_run_len = run_len;
        } else {
            // Reset the streak when the predictor misses.
            run_len = 0;
        }
        // Train the predictor on the actual observed bit.
        predictor.update(y);
    }

    // Convert the raw prediction statistics into the SP800-90B style
    // min-entropy estimate for this predictor test.
    return predictionEstimate(C, N, max_run_len, 2, "PAQ6v2Predictor", verbose, label);
}
