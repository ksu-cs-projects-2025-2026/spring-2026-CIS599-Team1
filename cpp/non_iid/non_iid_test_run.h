#ifndef NONIIDTESTRUN_H
#define NONIIDTESTRUN_H

#include <string>
#include <vector>
#include <json/json.h>

#include "../shared/test_run_base.h"
#include "non_iid_test_case.h"

using namespace std;

// ----------------------------------------------------------------------------
// NonIidTestRun
//
// Represents a full evaluation pass (one invocation of ea_non_iid).
// It wraps common metadata from TestRunBase (timestamp, commandline, file hash)
// and the list of NonIidTestCase results from each individual statistical test.
//
// The GetAsJson() method marshals everything to a complete JSON document.
// ----------------------------------------------------------------------------
class NonIidTestRun : public TestRunBase {
public:
    string GetAsJson() {
        Json::Value json = TestRunBase::GetBaseJson();
        json["IID"] = IID;  // non-IID tool always sets this false

        Json::Value testCasesJson;
        for (int i = 0; i < (int)testCases.size(); i++){
            // In-place convert each test case (e.g., collision, markov, etc.)
            testCasesJson[i] = testCases[i].GetAsJson();
        }

        json["testCases"] = testCasesJson;

        Json::StyledWriter styled;
        return styled.write(json);
    }

    // Identifier for JSON output.  Non-IID engine should always be false.
    const bool IID = false;

    // Set of per-test-case results.  Each element holds one of the SP800-90B
    // entropy estimators and the associated output value.
    vector<NonIidTestCase> testCases;
};
#endif /* NONIIDTESTRUN_H */
