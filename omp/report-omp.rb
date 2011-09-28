[1,2,4,8,16].each do |i|
  10.times do
    puts "OMP_NUM_THREADS=#{i} ./a.out"
  end
end
