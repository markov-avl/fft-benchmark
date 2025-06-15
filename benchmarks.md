```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E CT-I+BRP-2 CT-R+B+BRP-2 CT-R+BRP-2 CT-R+S-2 CT-R-2 S-I-2 S-I-4 S-R-2 \
  --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 \
  --measurer timer \
  --generator random \
  --output output/1.csv \
  --runs 5 \
  --threads 1
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E FFT-R-2 FFT-R+T-2 FFT-RB+T-2 \
  --sizes 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 \
  --measurer intel-cpu-power \
  --generator random \
  --output output/1.csv \
  --runs 1 \
  --threads 2
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E S-I-2 \
  --sizes 4194304 8388608 16777216 33554432 67108864 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 1 \
  --threads 16
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E CT-I+BRP-2 CT-R+BRP-2 \
  --sizes 4194304 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 5 \
  --threads 2
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E CT-I+BRP-2 S-I-2 S-I-4 \
  --sizes 4194304 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 1 \
  --threads 2
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E GT-I \
  --sizes 1045506 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 1 \
  --threads 16
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E CT-I+BRP+S-2 CT-I+B+BRP+S-2 CT-R+S-2 \
  --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 5 \
  --threads 2
```

```bash
sudo cmake-build-debug/Main \
  --algorithms FFTW-E \
    CT-I+B+BRP+S-2 CT-I+B+DRP+S-4 \
    CT-I+BRP+S-2 CT-I+DRP+S-4 \
    CT-R+B+BRP-2 CT-R+B+DRP-4 \
    CT-R+BRP-2 CT-R+DRP-4 \
    CT-R-2 CT-R-4 \
    S-I-2 S-I-4 \
    S-R-2 S-R-4 \
  --sizes 256 1024 4096 16384 65536 262144 1048576 4194304 \
  --measurer timer \
  --generator random \
  --output output/2.csv \
  --runs 5 \
  --threads 1
```