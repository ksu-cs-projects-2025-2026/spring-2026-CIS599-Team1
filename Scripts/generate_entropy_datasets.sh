#!/bin/bash

OUTPUT_DIR="min_entropy_datasets"
BLOCKS=977
FILES_PER_LEVEL=100
SECONDS=0 #timer becauses why not

# path to your binary (relative to this script location)
DJEN="../../djenrandom/djenrandom"

mkdir -p "$OUTPUT_DIR"

# H_min → bias mapping
declare -A bias_map=(
    [0.1]=0.933
    [0.2]=0.8706
    [0.3]=0.812
    [0.4]=0.758
    [0.5]=0.7071
    [0.6]=0.66
    [0.7]=0.616
    [0.8]=0.5743
    [0.9]=0.5359
    [1.0]=0.5
)

for H in "${!bias_map[@]}"
do
    BIAS="${bias_map[$H]}"
    LEVEL_DIR="${OUTPUT_DIR}/hmin_${H}"
    mkdir -p "$LEVEL_DIR"

    echo "=== H_min=$H (bias=$BIAS) ==="

    for i in $(seq 1 "$FILES_PER_LEVEL")
    do
        SEED="h${H}_run${i}"
        OUTPUT_FILE="${LEVEL_DIR}/data_${i}.bin"

        "$DJEN" \
            -D "$SEED" \
            -m biased \
            --bias "$BIAS" \
            -k "$BLOCKS" \
            -b \
            -o "$OUTPUT_FILE"
    done
done

printf "Total elapsed time: %02d:%02d:%02d\n" \
    $((SECONDS/3600)) $((SECONDS%3600/60)) $((SECONDS%60))