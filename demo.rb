# demo
require './symmetric_group.rb'

print "D4 is a dihedral group. D4 includes rotations and refletions of a square.\n"
print "It is expressed by a permutation of vertices and it is generated from two cycles [1,2,3,4] and [1,3],\n"
print "Let s be [1,2,3,4] and t be [1,3].\n"
print "s = Permutation.new([2,3,4,1])\n"
print "t = Permutation.new([3,2,1,4])\n"
print "\n"
sg = Symmetric_group.new(4)
i = sg.index([2,3,4,1])
j = sg.index([3,2,1,4])
print "D4 = #{sg.to_s_c(sg.gen_group([i,j]))}\n"
print "\n\n"

print "If you generate a group from [1,2,3,4] and [1,2] instead of the cycles above, the symmetric group S4 will be generated.\n"
print "s = Permutation.new([2,3,4,1])\n"
print "t = Permutation.new([2,1,3,4])\n"
print "\n"
j = sg.index([2,1,3,4])
print "S4 = #{sg.to_s_c(sg.gen_group([i,j]))}\n"
print "\n\n"

