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

raise "to_s_c didn't work." unless a.to_s_c == "[1,2,5][3,4]"
raise "to_s_c didn't work." unless b.to_s_c == "[1,2,5]"
raise "to_s_c didn't work." unless c.to_s_c == "[3,4]"

sg = Symmetric_group.new(5)
raise "method degree on Symmetric_group didn't work." unless sg.degree == 5
i=sg.index(a)
j=sg.index(b)
k=sg.index(c)
raise "method mul on Symmetric_group didn't work." unless sg.mul([i,j],[k]) == [sg.index([2,5,3,4,1]),sg.index([2,5,4,3,1])].sort
raise "method mul on Symmetric_group didn't work." unless sg.mul(i,[i,j]) == [sg.index([5,1,3,4,2]),sg.index([5,1,4,3,2])].sort
raise "method mul on Symmetric_group didn't work." unless sg.mul([i,j],i) == [sg.index([5,1,3,4,2]),sg.index([5,1,4,3,2])].sort
raise "Group generation didn't work." unless sg.gen_group([sg.index([2,1,3,4,5])]) == [sg.index([2,1,3,4,5]),sg.index([1,2,3,4,5])].sort
raise "method to_s_c on Symmetric_group didn't work." unless sg.to_s_c(i) == "[1,2,5][3,4]"
raise "method to_s_c on Symmetric_group didn't work." unless sg.to_s_c([j,k]) == "[[1,2,5],[3,4]]"


print "The test has been completed.\n"
print "All the methods has worked.\n"

