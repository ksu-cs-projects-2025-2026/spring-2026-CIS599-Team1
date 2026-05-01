#ifndef NONIIDTESTCASE_H
#define NONIIDTESTCASE_H

#include <string>
#include <json/json.h>

#include "../shared/test_case_base.h"

using namespace std;

// ----------------------------------------------------------------------------
// NonIidTestCase
//
// A plain data holder that stores the results of each non-IID statistical test
// for one run of the engine (Section 6.3 of SP800-90B).  It inherits from
// TestCaseBase so common fields like name and status are included.
//
// Fields are initialized to -1.0 when not yet calculated.  The GetAsJson()
// method exports only filled values, to keep output compact and clear.
// ----------------------------------------------------------------------------
class NonIidTestCase : public TestCaseBase {
public:

    // Quantities from bit-string tests (t-tuple, LRS, etc).
    double bin_t_tuple_res = -1.0;
    double t_tuple_res = -1.0;
    double bin_lrs_res = -1.0;
    double lrs_res = -1.0;

    // Quantities from conditioning (Section 3.1.5.1.2).
    double h_r = -1.0;    // Estimated min-entropy of raw input
    double h_c = -1.0;    // Estimated min-entropy after conditioning
    double h_i = -1.0;    // Estimated min-entropy of input to conditioner

    // Conditioning parameters (n_in, n_out, nw, h_in, h_out).
    double n_in = -1.0;   // Input block size (bits)
    double n_out = -1.0;  // Output block size (bits)
    double nw = -1.0;     // Noise source output size (bits)
    double h_in = -1.0;   // Min-entropy per input block
    double h_out = -1.0;  // Min-entropy per output block

    double h_p = -1.0;    // Final chosen min-entropy used by this test case
    // ------------------------------------------------------------------------
    // Convert all values that have been computed into a clean JSON object.
    // Fields left at the default -1.0 are omitted because they are not yet
    // computed for this dataset.
    // ------------------------------------------------------------------------
    Json::Value GetAsJson() {
        Json::Value json = TestCaseBase::GetBaseJson();
        
        if(bin_t_tuple_res != -1)
            json["binTTupleRes"] = bin_t_tuple_res;
        if(t_tuple_res != -1)
            json["tTupleRes"] = t_tuple_res;
        if(bin_lrs_res != -1)
            json["binLrsRes"] = bin_lrs_res;
        if(lrs_res != -1)
            json["lrsRes"] = lrs_res;
        if(h_r != -1)
            json["h_r"] = h_r;
        if(h_c != -1)
            json["h_c"] = h_c;
        if(h_i != -1)
            json["h_i"] = h_i;
        if(n_in != -1)
            json["n_in"] = n_in;
        if(n_out != -1)
            json["n_out"] = n_out;
        if(nw != -1)
            json["nw"] = nw;
        if(h_in != -1)
            json["h_in"] = h_in;
        if(h_out != -1)
            json["h_out"] = h_out;
        if(h_p != -1)
            json["h_p"] = h_p;
        //if(vetted != -1)
        //    json["vetted"] = vetted;
        
        return json;
    }
};
#endif /* NONIIDTESTCASE_H */
