1.upto(16) do|i|
  puts "mpiexec -n #{i} a.out"
end
