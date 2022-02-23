require './symmetric_group.rb'

#test

a=Permutation.new [2,5,4,3,1]
b=Permutation.new [2,5,3,4,1]
c=Permutation.new [1,2,4,3,5]

raise "Permutation.new didn't work." unless a.instance_of? Permutation
raise "Permutation.new didn't work." unless a == [2,5,4,3,1]

raise "is_cyclic? didn't work." if a.is_cyclic?
raise "is_cyclic? didn't work." unless b.is_cyclic?
raise "is_cyclic? didn't work." unless c.is_cyclic?

raise "is_transposition? didn't work." if a.is_transposition?
raise "is_transposition? didn't work." if b.is_transposition?
raise "is_transposition? didn't work." unless c.is_transposition?

d=a.to_cycles
raise "to_cycles didn't work." unless d == [b,c]

raise "mapsto didn't work." unless a.mapsto(4) == 3

raise "multiplication didn't work." unless a == b*c

raise "to_s_c didn't work." unless b.to_s_c == "[1,2,5]"
raise "to_s_c didn't work." unless c.to_s_c == "[3,4]"
raise "to_s_c didn't work." unless a.to_s_c == "[1,2,5][3,4]"

p=PSet.new [a,b]
q=PSet.new [c]
raise "multiplication on PSet didn't work." unless p*q == PSet.new([Permutation.new([2,5,3,4,1]),Permutation.new([2,5,4,3,1])])
raise "multiplication on Permutation with PSet didn't work." unless a*p == PSet.new([Permutation.new([5,1,3,4,2]),Permutation.new([5,1,4,3,2])])
raise "multiplication on PSet with Permutation didn't work." unless p*a == PSet.new([Permutation.new([5,1,3,4,2]),Permutation.new([5,1,4,3,2])])

raise "Group generation didn't work." unless PGroup.new(PSet.new([Permutation.new([2,1,3,4,5])])) == PSet.new([Permutation.new([2,1,3,4,5]),Permutation.new([1,2,3,4,5])])

print "The test has been completed.\n"
print "All the methods has worked.\n"

