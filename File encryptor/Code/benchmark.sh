#!/usr/bin/env bash
#
# benchmark.sh
# Compare throughput of cli_file_encryptor.py vs. OpenSSL
#

# Always run from the script’s own directory
cd "$(dirname "$0")"

FILES_DIR="Files_for_benchmark"
PASSPHRASE="Benchmark123!"
SIZES=(1 10 100)

for MB in "${SIZES[@]}"; do
  INPUT="${FILES_DIR}/${MB}MB.bin"
  OUT_CLI="${FILES_DIR}/cli_${MB}MB.enc"
  OUT_OPENSSL="${FILES_DIR}/openssl_${MB}MB.enc"

  echo "==== ${MB} MB File ===="

  # 1) Our Python tool
  start=$(date +%s.%N)
  python3 cli_file_encryptor.py encrypt \
    -i "$INPUT" -o "$OUT_CLI" -p "$PASSPHRASE"
  dur=$(echo "$(date +%s.%N) - $start" | bc)
  thr1=$(echo "$MB / $dur" | bc -l)
  printf "CLI Tool : %.2f MB/s\n" "$thr1"

  # 2) OpenSSL
  start=$(date +%s.%N)
  openssl enc -aes-256-cbc -pbkdf2 -iter 200000 \
    -salt -in "$INPUT" -out "$OUT_OPENSSL" \
    -pass pass:"$PASSPHRASE"
  dur=$(echo "$(date +%s.%N) - $start" | bc)
  thr2=$(echo "$MB / $dur" | bc -l)
  printf "OpenSSL  : %.2f MB/s\n" "$thr2"

  # 3) Percentage difference
  diff=$(echo "($thr1 - $thr2)/$thr2 * 100" | bc -l)
  printf "Delta    : %+0.1f%%\n\n" "$diff"
done