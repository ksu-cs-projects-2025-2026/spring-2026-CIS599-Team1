#!/bin/bash

DATASET_DIR="../Scripts/min_entropy_datasets"
SP800="../cpp/ea_non_iid"
FILES_PER_LEVEL=100
SECONDS=0

hmins=($(ls "$DATASET_DIR" | sort -V))

# Output files per test
PAQ_CSV="paq(bitwiseCharModel).csv"
MCW_CSV="multimcw.csv"
LAG_CSV="lag.csv"
MMC_CSV="multimmc.csv"
LZ_CSV="lz78y.csv"

# Write headers
write_header() {
    local file=$1
    local header=()

    for h in "${hmins[@]}"; do
        header+=("hmin_$h")
    done

    (IFS=,; echo "${header[*]}") > "$file"
}

write_header "$PAQ_CSV"
write_header "$MCW_CSV"
write_header "$LAG_CSV"
write_header "$MMC_CSV"
write_header "$LZ_CSV"

# Main loop
for i in $(seq 1 "$FILES_PER_LEVEL")
do
    paq_row=()
    mcw_row=()
    lag_row=()
    mmc_row=()
    lz_row=()

    for h in "${hmins[@]}"
    do
        file="$DATASET_DIR/$h/data_${i}.bin"
        echo "Processing $file"

        output=$("$SP800" -i -a -v "$file" 8 2>/dev/null)

        # PAQ
        paq=$(echo "$output" | grep "PAQ6v2 Predictor Test Estimate" \
        | grep -oE '[0-9]+\.[0-9]+' | tail -1)

        # MultiMCW (bitstring only)
        mcw=$(echo "$output" | grep "Multi Most Common in Window (MultiMCW)" | grep "bit string" | sed -E 's/.*= ([0-9.]+) \/.*/\1/' | tail -1)

        # Lag (bitstring only)
        lag=$(echo "$output" | grep "Lag Prediction Test Estimate (bit string)" \
        | grep -oE '[0-9]+\.[0-9]+' | tail -1)

        # MultiMMC (bitstring only)
        mmc=$(echo "$output" | grep "Multi Markov Model with Counting" \
        | grep "bit string" \
        | grep "Prediction Test Estimate" \
        | grep -oE '[0-9]+\.[0-9]+' | tail -1)

        # LZ78Y (bitstring only)
        lz=$(echo "$output" | grep "LZ78Y Prediction Test Estimate (bit string)" \
        | grep -oE '[0-9]+\.[0-9]+' | tail -1)

        # fallback
        [ -z "$paq" ] && paq="NA"
        [ -z "$mcw" ] && mcw="NA"
        [ -z "$lag" ] && lag="NA"
        [ -z "$mmc" ] && mmc="NA"
        [ -z "$lz" ] && lz="NA"

        paq_row+=("$paq")
        mcw_row+=("$mcw")
        lag_row+=("$lag")
        mmc_row+=("$mmc")
        lz_row+=("$lz")
    done

    # write rows
    (IFS=,; echo "${paq_row[*]}") >> "$PAQ_CSV"
    (IFS=,; echo "${mcw_row[*]}") >> "$MCW_CSV"
    (IFS=,; echo "${lag_row[*]}") >> "$LAG_CSV"
    (IFS=,; echo "${mmc_row[*]}") >> "$MMC_CSV"
    (IFS=,; echo "${lz_row[*]}") >> "$LZ_CSV"
done

# timing
printf "Total elapsed time: %02d:%02d:%02d\n" \
    $((SECONDS/3600)) $((SECONDS%3600/60)) $((SECONDS%60))