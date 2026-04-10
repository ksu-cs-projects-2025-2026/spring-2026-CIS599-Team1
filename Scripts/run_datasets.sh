#!/bin/bash

DATASET_DIR="../Scripts/min_entropy_datasets"
SP800="../cpp/ea_non_iid"
OUTPUT_CSV="results.csv"
FILES_PER_LEVEL=100
SECONDS=0 #timer becauses why not

# Get sorted list of hmin directories
hmins=($(ls "$DATASET_DIR" | sort -V))

# Write header
(IFS=,; echo "${hmins[*]}") > "$OUTPUT_CSV"

# Loop over dataset index
for i in $(seq 1 "$FILES_PER_LEVEL")
do
    row=()

    for h in "${hmins[@]}"
    do
        file="$DATASET_DIR/$h/data_${i}.bin"

        echo "Processing $file"

        # Run SP800 and extract PAQ value directly
        paq=$("$SP800" -i -a -v "$file" 8 2>/dev/null \
              | grep -m1 "PAQ6v2 Predictor Test Estimate" \
              | sed -E 's/.*= ([0-9.]+) \/.*/\1/')

        # Handle missing values
        [ -z "$paq" ] && paq="NA"

        row+=("$paq")
    done

    # Write row to CSV
    (IFS=,; echo "${row[*]}") >> "$OUTPUT_CSV"
done

printf "Total elapsed time: %02d:%02d:%02d\n" \
    $((SECONDS/3600)) $((SECONDS%3600/60)) $((SECONDS%60))