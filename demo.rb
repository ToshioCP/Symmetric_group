# demo
require './symmetric_group.rb'

print "D4 is a dihedral group. D4 includes rotations and refletions of a square.\n"
print "It is expressed with a permutation of vertices and it is generated from two cycles [1,2,3,4] and [1,3],\n"
print "Let s be [1,2,3,4] and t be [1,3].\n"
print "They are created with Permutation.new([2,3,4,1]) and Permutation.new([3,2,1,4]) respectively.\n"
print "st = PSet.new (s,t)\n"
print "D4 = PGroup.new (st)\n"
print "\n"
st=PSet.new ([Permutation.new([2,3,4,1]),Permutation.new([3,2,1,4])])
print "D4 = #{(PGroup.new(st)).to_s_c}\n"
print "\n\n"

print "If you generate a group from [1,2,3,4] and [1,2] instead of the cycles above, the symmetric group S4 will be generated.\n"
print "s = Permutation.new([2,3,4,1])\n"
print "t = Permutation.new([2,1,3,4])\n"
print "st = PSet.new (s,t)\n"
print "S4 = PGroup.new (st)\n"
print "\n"
st=PSet.new ([Permutation.new([2,3,4,1]),Permutation.new([2,1,3,4])])
print "S4 = #{(PGroup.new(st)).to_s_c}\n"
print "\n\n"

print "Print each element of S4.\n"
print "\n"
[1,2,3,4].permutation.each do |a|
  cycles = Permutation.new(a).to_cycles
  if cycles == []
    s = "id"
  else
    s=""
    cycles.each {|c| s << c.to_s_c}
  end
  print "#{s}\n"
end
