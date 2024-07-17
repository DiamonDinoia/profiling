# Profiling workshop instructions

### 1a. Flatiron machine modules
```bash
module load modules/2.3-20240529
module load cmake
module load gcc/11
module load perf-linux
module load gperftools/2.15
module load fftw
```
### 1b. Install dependencies on your local machine
```bash
pprof https://pypi.org/project/pprof/
cmake https://pypi.org/project/cmake/
perf https://www.ubuntupit.com/how-to-install-and-configure-perf-in-linux-distributions/
```
configure the kernel:

https://fishilico.github.io/generic-config/sysadmin/perf-linux.html  
https://www.kernel.org/doc/html/v5.7/admin-guide/perf-security.html  

(I recommend leaving this to scc)

### 2. Clone the repository
```bash
git clone https://github.com/DiamonDinoia/profiling.git
```

### 3. Build the project
```bash
mkdir buid && cd build
cmake ..  # ignore  note: gperftools' cmake support is incomplete and is best-effort only
make -j
```

### 4. Run the project
```bash
cd bin
./main
```
