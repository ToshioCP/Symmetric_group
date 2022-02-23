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

# PSetUtils has methdos for PSet and PGroup classes

# PSetとPGroupクラスのためのモジュール
module PSetUtils
  def to_s_c
    return "[]" if self.empty?
    a=self.to_a
    n=a.size
    c="[#{a[0].to_s_c}"
    1.upto(n-1) do |i|
      c << ",#{a[i].to_s_c}"
    end
    c << "]"
    c
  end
end

# PSet is a set of permutations.

# 置換の集合
class PSet < Set
  include PSetUtils
  # PSet.new([p1,p2,p3]) => [p1,p2,p3] p1,vp2, p3 are Permutation instances.
  # PSet.new([]) => []

  # PSet.new([p1,p2,p3]) => [p1,p2,p3] p1,vp2, p3 は置換のインスタンス
  # PSet.new([]) => []
  def initialize a
    raise "Illegal argument" unless a.instance_of? Array
    a.each do |p|
      raise "Illegal argument" unless p.instance_of? Permutation
    end
    if a == []
      super
    else
      super(a)
    end
  end

  # PQ={pq|p in P, q in Q}
  # Pq={pq| p in P}
  # pQ is defined in the Permutation class.

  # PQ={pq|p in P, q in Q}
  # Pq={pq| p in P}
  # pQはPermutationの演算として定義する。
  def *(other)
    raise "Illegal argument" unless other.instance_of?(PSet) || other.instance_of?(Permutation)
    pq=PSet.new []
    self.each do |p|
      if other.instance_of? PSet
        other.each do |q|
          pq << p*q
        end
      else
        pq << p*other
      end
    end
    pq
  end
end

# PGroup is a subgroup of S_n (n-degree symmetric group).

# PGroupはn次対称群の部分群
class PGroup < Set
  include PSetUtils
  # PGroup.new(pset): pset is an instance of PSet. PGroup.new creates <pset>, which is the minimum group contains pset.
  # Note: Suppose A is a finite subset of a group G. If AA=A, then A is a subgroup of G.

  # PGroup.new(pset): psetはPSetインスタンス。PGroup.newはpsetによって生成される部分群<pset>（psetを含む最小の部分群）を生成する。
  # 注：Aを群Gの有限部分集合とする。AA=Aが成り立つならば、AはGの部分群である。
  def initialize pset
    raise "Illegal argument" unless pset.instance_of? PSet
    if pset == []
      super
    else
      until pset == (sp = pset*pset)
        pset += sp
      end
      super(pset)
    end
  end
end

