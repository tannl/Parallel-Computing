# Parallel-Computing
【自用】并行计算的资料以及优化的一些代码
## 说明
使用MPI、openmp进行代码优化。
## 运行环境
  gcc、mipcc/mpirun
## MPI+OpenMP编译
  ```
  mpicc/mpic++ -o test omp.c/omp.cpp -fopenmp
