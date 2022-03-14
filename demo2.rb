# demo2.rb
# It shows all the subgroups of Sn
# It takes about 3 seconds on my laptop (Core-i7)

# n次対称群のすべての部分群を表示するプログラム
# 計算に約3秒（Core-i7のノートPCで）

require 'benchmark'
include Benchmark
 
require './symmetric_group.rb'

def find_supergroup(group, elements)
  return if group.size*2 >= @sg_indecies.size
  diff = elements - group
  diff.each do |q|
    pset = group.union([q])
    supergroup = @sg.gen_group(pset)
    next if @subgroups.include?(supergroup)
    @subgroups << supergroup
    elements = elements - [q]
    find_supergroup(supergroup, elements)
  end
end

def find_subgroups n
  @sg = Symmetric_group.new(n)
  elements = @sg_indecies = (0 .. @sg.size-1).to_a
  @subgroups = []
  @sg_indecies.each do |i|
    next if i == 0
    pset = [i]
    group = @sg.gen_group(pset)
    @subgroups << group
    elements = elements - pset
    find_supergroup group, elements
  end
  @subgroups << [0]
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

