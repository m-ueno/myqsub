[1,2,4,8,16].each do |i|
  10.times do
    puts "mpiexec -n #{i} a.out"
  end
  puts
end
