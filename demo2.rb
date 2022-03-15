# demo2.rb
# It shows all the subgroups of Sn
# It takes about 3 seconds on my laptop (Core-i7)

# n次対称群のすべての部分群を表示するプログラム
# 計算に約3秒（Core-i7のノートPCで）

require 'benchmark'
include Benchmark
 
require './symmetric_group.rb'

def find_supergroup(cgroups)
  n = cgroups.size
  return if cgroups == [@sg_indecies]
  groups = []
  cgroups.combination(2) do |s,t|
    u = (s+t).uniq
    gr = @sg.gen_group(u).sort
    groups << gr
  end
  groups = groups.sort.uniq
  @subgroups += groups
  find_supergroup groups
end

def find_subgroups n
  @sg = Symmetric_group.new(n)
  @sg_indecies = (0 .. @sg.size-1).to_a
  cgroups = @sg_indecies.map {|i| @sg.gen_cyclic_group(i).sort}
  cgroups = cgroups.sort.uniq
  @subgroups = cgroups.dup
  find_supergroup cgroups-[[0]]
  @subgroups = @subgroups.uniq.sort {|a,b| a.size <=> b.size}
  print "The number of subgroups is #{@subgroups.size}.\n"
  print "生成された全部分群（#{@subgroups.size}個）\n"
  @subgroups.each do |s|
    print "#{@sg.to_s_c(s)}\n"
  end
end

# Comment out the following two lines if you want to run a benchmark test.
# ベンチマーク測定をする場合は、下記2行をコメントアウトする
find_subgroups(5)
__END__

Benchmark.benchmark(CAPTION, 15, FORMAT) do |x|
  x.report("find_subgroups:") do
    stdout_old = $stdout.dup
    $stdout.reopen("/dev/null")
    find_subgroups(5)
    $stdout.reopen stdout_old
  end
end

