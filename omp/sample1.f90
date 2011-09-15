program sample
!$  use omp_lib
!  integer::OMP_GET_THREAD_NUM
!$OMP PARALLEL
  write(*,*) "My thread id is ", OMP_GET_THREAD_NUM(), " Hello"
!$OMP END PARALLEL
end program sample

