program main
  real::a(1000)
  integer::omp_get_thread_num
!$OMP PARALLEL PRIVATE(myid)
  myid = omp_get_thread_num()

!$OMP DO SCHEDULE(STATIC,2)
  do i=1,10
  a(i)=i
  if(myid.eq.0)then
    write(6,*) i
  endif
  enddo
!$OMP END PARALLEL
end

