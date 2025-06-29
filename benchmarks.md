# Сценарии бенчмарков

## Подготовка окружения

Настройки для измерение времени:

```bash
MEASURER=timer
OUTPUT_DIRECTORY=output/time
```

Настройки для измерения энергопотребления CPU:

```bash
MEASURER=cpu-power-meter
OUTPUT_DIRECTORY=output/power
```

Общие настройки:

```bash
THREADS=(1 2 4 8 12 16)
GENERATOR=random
RUNS=10
alias FFTBenchmark="sudo cmake-build-debug/FFTBenchmark"
mkdir -p "${OUTPUT_DIRECTORY}"
```

## Влияние Radix на ускорение

Recursive Cooley Tukey (Radix 2) vs Recursive Cooley Tukey (Radix 4):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-R-2 CT-R-4 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/radix_r_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Iterative Cooley Tukey (Radix 2) vs Iterative Cooley Tukey (Radix 4):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-I+BRP+S-2 CT-I+DRP+S-4 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/radix_i_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Recursive Stockham (Radix 2) vs Recursive Stockham (Radix 4):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms S-R-2 S-R-4 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/radix_r_stockham_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Iterative Stockham (Radix 2) vs Iterative Stockham (Radix 4):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms S-I-2 S-I-4 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/radix_i_stockham_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Bluestein + Stockham (Radix 2) vs Bluestein + Stockham (Radix 4):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms B-I+S-2 B-I+S-4 \
    --sizes 255 1023 4095 16383 65535 262143 1048575 4194303 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/radix_bluestein-stockham_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

## Влияние природы алгоритма на ускорение

Recursive Cooley Tukey + BRP vs Iterative Cooley Tukey + BRP:

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-R+BRP-2 CT-I+BRP+S-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/form_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Recursive Stockham vs Iterative Stockham:

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms S-R-2 S-I-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/form_stockham_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

## Использование перестановки

Recursive Cooley Tukey (Radix 2):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-R-2 CT-R+BRP-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/permutation_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

## Использование барьера

Recursive Cooley Tukey (Radix 2):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-R+BRP-2 CT-R+B+BRP-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/barrier_r_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Iterative Cooley Tukey (Radix 2):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms CT-I+BRP+S-2 CT-I+B+BRP+S-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/barrier_i_cooley-tukey_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

## Использование пула потоков

Iterative Stockham (Radix 2):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms S-I-2 S-I+TP-2 \
    --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 16777216 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/pool_stockham_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

## Сравнение сверточных методов

Good Thomas + DFT vs Good Thomas + Bluestein + Iterative Stockham (Radix 2):

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms GT-I+DFT GT-I+B+S-2 \
    --sizes 100 500 1000 5000 10000 50000 100000 200000 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/good-thomas_inconvenient_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms GT-I+DFT GT-I+B+S-2 \
    --sizes 99 506 1023 5183 9999 50175 100806 200703 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/good-thomas_convenient_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

Сравнение GoodThomas + Bluestein и Bluestein:

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms GT-I+B+S-2 B-I+S-2 \
    --sizes 254 1018 4078 16382 65522 262142 1048574 4194286 16777186 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/good-thomas-bluestein-vs-bluestein_bluestein-convenient_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms GT-I+B+S-2 B-I+S-2 \
    --sizes 255 1023 4095 16383 65535 262143 1048575 4194303 16777215 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/good-thomas-bluestein-vs-bluestein_both-convenient_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```

```bash
for t in "${THREADS[@]}"; do
  FFTBenchmark \
    --algorithms GT-I+B+S-2 B-I+S-2 \
    --sizes 272 1056 4160 16512 65792 262656 1049600 4196352 16781312 \
    --measurer "${MEASURER}" \
    --generator "${GENERATOR}" \
    --output "${OUTPUT_DIRECTORY}/good-thomas-bluestein-vs-bluestein_good-thomas-convenient_${t}.csv" \
    --runs "${RUNS}" \
    --threads "${t}"
done
```
