#ifndef TESTRUN_H
#define TESTRUN_H

#include "utils.h"
#include <string>
#include <json/json.h>

using namespace std;

// ---------------------------------------------------------------------------
// TestRunBase
//
// Shared test-run metadata for all tools.  Each top-level tool (IID/non-IID/etc)
// stores one TestRunBase and then adds tool-specific data (test cases, results).
//
// Common fields:
//   - timestamp: record when the tool was run
//   - sha256: hash of input file for traceability
//   - filename: tested file path
//   - errorLevel/errorMsg: execution status
//   - type: tool type or purpose
//   - commandline: complete run command for audit
//
// GetBaseJson() builds a minimal JSON object shared by all child objects.
// ---------------------------------------------------------------------------
class TestRunBase {
public:
    string timestamp;
    string sha256;
    string filename;
    int errorLevel = 0;
    string errorMsg;
    string type;
    string commandline;

protected:
    // -----------------------------------------------------------------------
    // GetBaseJson
    //
    // Create a JSON object with shared fields (timestamp, command line, errors,
    // tool version, and file identity).  Child test run classes append more data.
    // -----------------------------------------------------------------------
    Json::Value GetBaseJson() {
        Json::Value baseJson;
        baseJson["dateTimeStamp"] = timestamp;
        baseJson["commandline"] = commandline;
        baseJson["errorLevel"] = errorLevel;
        baseJson["type"] = type;
        baseJson["toolVersion"] = VERSION;

        if (errorLevel != 0){
            baseJson["errorMessage"] = errorMsg;
        }
        if(!filename.empty()) {
            baseJson["filename"] = filename;
        }
        if(!sha256.empty()) {
            baseJson["sha256"] = sha256;
        }
        return baseJson;
    }
};
#endif /* TESTRUN_H */
