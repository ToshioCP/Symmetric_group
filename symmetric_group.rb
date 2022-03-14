# Ruby program for Symmetric groups
#
# This program supports symmetric groups, which elements are permutations (the same as bijections on finite sets).

# 対称群のrubyプログラム
# 対称群の要素は置換、または有限集合の全単射である。

require 'set'

# Permutation belongs to the n-degree symmetric group. 
# Ruby array has zero based indices on its arrays.
# For example, a permutation [2,1,3] is implememted as an array p[0]=2, p[1]=1, p[2]=3.
# The permutation maps 1=>2, 2=>1, 3=>3.
# Be careful that there is difference 1 between the indices and the numbers in the domain of the permutation.
# Instances of the class Permutation are immutable.

# n個の置換（n>=0）
# index 0 から始まるので、 p=[2,1,3]は p[0]=2, p[1]=1, p[2]=3 で、1=>2, 2=>1, 3=>3という置換。
# インデックスと置換前の数字の間に差が1だけあるのに注意。
# 置換は変更不可能（immutable）である。
class Permutation < Array
  # Permutation.new ([2,4,3,1]) => [2,4,3,1] (1=>2, 2=>4, 3=>3, 4=>1)
  def initialize a
    raise "Illegal argument" unless a.instance_of? Array
    n=a.size
    # check the numbers(contents) are [1..n]
    raise "Illegal argument" unless n >= 1
    c=[false]*n
    0.upto(n-1) do |i|
      raise "Illegal argument" unless a[i].instance_of? Integer
      raise "Illegal argument" unless a[i]>=1 and a[i]<=n
      c[a[i]-1]=true
    end
    raise "Illegal argument" unless c == [true]*n

    super(a)

    # instance variables for cycles
    # @group=[1,1,0,0,2,2,2] for the permutation [2,1,3,4,7,5,6]
    #  0: the element doesn't move by the permutation. 3=>3, 4=>4
    #  1: the first cycle. 1=>2=>1
    #  2: the second cycle. 5=>7=>6=>5
    # @n: the number of cycles in the permutation
    # @c_ent: Entries to cycles. @c_ent=[2,0,4] for [2,1,3,4,7,5,6] (@group [1,1,0,0,2,2,2])
    #  @c_ent[0]=2: points the first index of the element whiich doesn't move with the permutation.
    #  @c_ent[1]=0: points the first index of the first cycle.
    #  @c_ent[2]=4: points the first index of the second cycle.
    # @cycle: true if the permutation is a cycle.
    # @transposition: true if the permutation is a transposition.
 
    # 巡回置換のための諸変数
    # @group=[1,1,0,0,2,2,2] 置換が[2,1,3,4,7,5,6]だった場合
    #  0: 置換で動かない要素 3=>3, 4=>4
    #  1: 最初の巡回置換 1=>2=>1
    #  2: 2番めの巡回置換 5=>7=>6=>5
    # @n: 置換の含む巡回置換の数
    # @c_ent: 各巡回置換へのエントリー @c_ent=[2,0,4]は置換[2,1,3,4,7,5,6](@group [1,1,0,0,2,2,2])に対するエントリー
    #  @c_ent[0]=2: 置換で動かない最初の要素を指す
    #  @c_ent[1]=0: 最初の巡回置換の中の最初の要素を指す
    #  @c_ent[2]=4: 2番めの巡回置換の最初の要素を指す
    # @cycle: 置換全体が純愛置換のときに真
    # @transposition: 置換が互換のときに真

    @group=[0]*self.size
    @c_ent=[]
    l=1
    self.each_index do |i|
      next if @group[i] >= 1 # already done
      if self[i]-1 == i
        @c_ent[0]=i if @c_ent[0] == nil
        @group[i]=0
      else
        if @c_ent[l] == nil
          @c_ent[l]=i
        end
        j=i
        @group[j]=l
        until self[j]-1 == i
          j=self[j]-1
          @group[j]=l
        end
        l+=1
      end
    end
    @n=l-1

    @cycle=true
    @group.each do |l|
      if l >= 2
        @cycle=false
        break
      end
    end
    @cycle=false if @n == 0
    @transposition = ((@group.sum == 2 && @n == 1) ? true : false)
    self.freeze
    @group.freeze
    @n.freeze
    @c_ent.freeze
    @cycle.freeze
    @transposition.freeze
  end

  def is_transposition?
    @transposition
  end

  def is_cyclic?
    @cycle
  end

  # to_cycles method creates an array of cycles of self (permutation).
  # [2,3,1,5,4,6] => [[1,2,3],[4,5]]
  # Note that [6] (6 moves to 6 itself) isn't included to the array.
  # If self is an identity permutation, to_cycles returns an empty array.
 
  # 巡回置換に分解し、それらを配列で返す
  # [2,3,1,5,4,6] => [[1,2,3],[4,5]]
  # このとき、自分自身に写る6は巡回置換にならないことに注意
  # 注意：元の置換が恒等置換の場合、巡回置換が現れないので、要素が空の配列が返される => []
  def to_cycles
    return [] if @n == 0
    box=[]
    1.upto(@n) do |l|
      c=[]
      self.each_index do |i|
        if @group[i] == l
          c[i]=self[i]
        else
          c[i]=i+1
        end
      end
      box << Permutation.new(c)
    end
    box
  end

  # mapping
  # mapsto method maps an integer with the permutation.
  # If self is [2,3,1], then mapsto maps 1 to 2, 2 to 3 and 3 to 1

  # 写像
  # 置換によって整数を写す。
  # 置換が[2,3,1]ならば、1=>2, 2=>3, 3=>1のように写す。
  def mapsto x
    raise "Illegal argument" unless x.instance_of? Integer
    raise "Illegal argument" unless x>=1 and x<=self.size
    self[x-1]    
  end

  # multiplication
  # It is a composition of functions, so the second permutation performs first.
  # This multiplication also supports a set (p*Q={pq|q in Q})
  # Pq and PQ are supported by the class PSet.

  # 置換の積
  # 積は右から計算する。先にotherの置換をしてからselfの置換をする。
  # 置換と置換の集合の積もサポート => pQ={pq|q in Q}
  # PqとPQはPSetで定義されている。
  def *(other)
    raise "Illegal argument" unless other.instance_of?(Permutation) || other.instance_of?(PSet)
    if other.instance_of? Permutation
      a=[]
      0.upto(self.size-1) do |i|
        a[i]=self[other[i]-1]
      end
      Permutation.new a
    else
      pq=PSet.new []
      other.each do |q|
        pq << self*q
      end
      pq
    end
  end

  # creates a string which shows cycles of the permutation

  # 巡回置換の積の形式の文字列を作る
  def to_s_c
    return "id" if @n == 0
    c=""
    1.upto(@n) do |l|
      i=start=@c_ent[l]
      c << "[#{i+1}"
      until self[i]-1 == start
        i=self[i]-1
        c << ",#{i+1}"
      end
      c << "]"
    end
    c
  end

  # creates a tex-formated string

  # texフォーマットの文字列を返す 
  def to_tex
    n=self.size
    h=(n+1)/2-1
    s="\\begin{bmatrix}"
    0.upto(n-1) do |i|
      if i == n-1
        s << "#{self[i]}& &#{i+1}"
      elsif i == h
        s << "#{self[i]}&\\leftarrow&#{i+1}\\\\"
      else
        s << "#{self[i]}& &#{i+1}\\\\"
      end
    end
    s <<"\\end{bmatrix}"
  end

end

# n-degree symmetric group, which is a group consists of all the n-permutations.
# It is a sorted array of the permutations. 
# The elements (permutations) of the array are referenced by the index.
#   sg: an instance of Symmetric_group class, p: an instance of Permutation class
#   a permutation to an index => sg.index(p)
#   an index to a permutation => sg[i]
# So, a premutation and its index can be seen as the same thing. 
# An instance of the Symmetric_group class makes a Ceyley table in it.
# Multiplication of permutations can be calculated fast with the table.



# n次対称群。長さnの置換全体を配列の順にしたがって整列したもの。
# この群に含まれる置換は、配列の添字と1対1に対応する。
#   sg: Symmetric_groupクラスのインスタンス, p: Permutationクラスのインスタンス
#   a permutation to an index => sg.index(p)
#   an index to a permutation => sg[i]
# インスタンスは生成されるときに演算表を内部に作る。
# 演算表を用いてることにより、高速に置換の積を計算することができる。
class Symmetric_group < Array
  # Symmetric_group.new(2) => [[1,2],[2,1]] 
  def initialize n
    raise "Illegal argument." unless n.instance_of? Integer
    raise "Illegal argument." unless n >= 1
    super (1..n).to_a.permutation.sort.map {|p| Permutation.new(p)}
    @degree = n

    # Create a Cayley table (a table arranging all the produts of the group's elements)
    # 乗算表の作成
    n = self.size
    @table = Array.new(n){Array.new(n)}
    (0 .. n-1).each do |i|
      (0 .. n-1).each do |j|
        @table[i][j] = self.index self[i]*self[j]
      end
    end
  self.freeze
  @table.freeze
  end

  def degree
    @degree
  end

  def cayley_table
    @table
  end

  # Multiplication. The two arguments are indecies of permutations, or subsets of the symmetric group.
  # 乗算。2つの引数はインデックス（正の整数）で表された置換、または対称群の部分集合。
  def mul a,b
    n = self.size
    raise "Illegal argument." unless a.instance_of?(Integer) || a.instance_of?(Array)
    raise "Illegal argument." unless b.instance_of?(Integer) || b.instance_of?(Array)
    if a.instance_of? Integer
      raise "Illegal argument." unless (0 .. n-1).include? a
    end
    if b.instance_of? Integer
      raise "Illegal argument." unless (0 .. n-1).include? b
    end
    if a.instance_of? Array
      a.each {|i| raise "Illegal argument." unless (0 .. n-1).include? i}
    end
    if b.instance_of? Array
      b.each {|j| raise "Illegal argument." unless (0 .. n-1).include? j}
    end
    if a.instance_of? Integer
      if b.instance_of? Integer
        @table[a][b]
      else
        b.map{|j| @table[a][j]}.uniq.sort
      end
    else
      if b.instance_of? Integer
        a.map{|i| @table[i][b]}.uniq.sort
      else
        m = []
        a.each do |i|
          m += b.map{|j| @table[i][j]}.uniq.sort
        end
        m
      end
    end
  end

  # Generate a group from a set

  # 集合から群を生成。
  def gen_group subset
    n = self.size
    raise "Illegal argument." unless subset.instance_of? Array
    raise "Illegal argument." if subset == []
    subset.each do |i|
      raise "Illegal argument." unless i.instance_of? Integer
      raise "Illegal argument." unless (0 .. n-1).include? i
    end
    group = subset.sort
    s = []
    until s == group
      s = (s+group).uniq.sort
      group = self.mul(s,s).uniq.sort
    end
    group
  end

  # Create a string which expresses the permutation as a product of cycles.

  # 巡回置換の積の形で表現した文字列を生成
  def to_s_c a
    n = self.size
    raise "Illegal argument." unless a.instance_of?(Integer) || a.instance_of?(Array)
    if a.instance_of? Integer
      raise "Illegal argument." unless (0 .. n-1).include? a
    else
      a.each {|i| raise "Illegal argument." unless (0 .. n-1).include? i}
    end
    if a.instance_of? Integer
      self[a].to_s_c
    else
      s = a.map{|i| self[i].to_s_c}
      "["+s.join(",")+"]"
    end
  end
end

