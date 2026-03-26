
#pragma once

#include "../shared/utils.h"
#include "paq6v2_library.h"

#include <vector>

// PAQ6v2 predictor-style non-IID test (bitwise).
double simple_paq_test(uint8_t* data, long len) {
    paq6v2::Predictor predictor;
    long correct = 0;
    long total = 0;

    // Loop through each byte
    for (long i = 0; i < len; ++i) {
        uint8_t byte = data[i];

        // Process each bit
        for (int b = 7; b >= 0; --b) {
            int actual = (byte >> b) & 1;

            // Get probability and convert to prediction
            int p = predictor.p();
            int pred = (p >= paq6v2::PSCALE / 2) ? 1 : 0;

            if (pred == actual)
                correct++;

            total++;
            // Update tables (Train predictor)
            predictor.update(actual);
        }
    }

    // Return simple accuracy
    return (double)correct / total;
}


