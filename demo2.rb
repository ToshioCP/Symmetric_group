# demo2.rb
# It shows all the subgroups of S4
# It takes about 47 seconds on my laptop (Core-i7)

# 4次対称群のすべての部分群を表示するプログラム
# 計算に約47秒（Core-i7のノートPCで）

require 'benchmark'
include Benchmark
 
require './symmetric_group.rb'

def find_supergroup(group)
  return if group == @perms
  diff = @perms - group
  diff.each do |q|
    pset = PSet.new(group.to_a) + PSet.new([q])
    supergroup = PGroup.new(pset)
    @subgroups << supergroup
    find_supergroup(supergroup)
  end
end

def find_subgroups
  id = Permutation.new([1,2,3,4])
  @perms = PSet.new([1,2,3,4].permutation.to_a.map {|a| Permutation.new(a)})
  @subgroups = []
  @perms.each do |p|
    next if p == id
    pset = PSet.new([p])
    group = PGroup.new(pset)
    @subgroups << group
    find_supergroup group
  end
  @subgroups << PGroup.new(PSet.new([id]))
  @subgroups = @subgroups.uniq.sort {|a,b| a.size <=> b.size}
  print "The number of subgroups is #{@subgroups.size}.\n"
  print "生成された全部分群（#{@subgroups.size}個）\n"
  @subgroups.each do |s|
    print "#{s.to_s_c}\n"
  end
end

# Comment out the following two lines if you want to run a benchmark test.
# ベンチマーク測定をする場合は、下記2行をコメントアウトする
find_subgroups
__END__

Benchmark.benchmark(CAPTION, 15, FORMAT) do |x|
  x.report("find_subgroups:") do
    stdout_old = $stdout.dup
    $stdout.reopen("/dev/null")
    find_subgroups()
    $stdout.reopen stdout_old
  end
end

