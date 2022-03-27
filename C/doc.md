日本語の解説は後半にあります。

#### How to use

demo.c calculates and displays subgroups of symmetric groups of order 1 to 5.
Compile and run demo.c

~~~
$ bash demo.sh
$ ./a.out
~~~

demo2.c calculates and displays a subgroup of the 6th order symmetric group.
It takes 5 hours 24 minutes 48 seconds on a core-i7 laptop.
Compile and run demo2.c

~~~
$ bash demo2.sh
$ ./a.out
~~~

Program testing.

~~~
$ bash test.sh
~~~

#### indeces of permutations

Take a permutation of length 3 as an example.
The permutation where 1 => 2,2 => 3,3 => 1 is written as p[2,3,1].
The symmetric group of degree 3 is denoted by S3.
The elements of S3 are:

- p[1,2,3]
- p[1,3,2]
- p[2,1,3]
- p[2,3,1]
- p[3,1,2]
- p[3,2,1]

The above six permutations are sorted by lexicographic order.
Similarly, the elements of the symmetric group of degree n can be arranged in lexicographic order.
And when a permutation is the i-th from the front, i-1 is called the index of that permutation.
The index is a number starting from 0.

By index, the elements of the symmetric group of degree n are represented by integers from 0 to n!-1.
When using an index, the degree of the group must be indicated in advance.
Both p[1,2] and p[1,2,3] have an index of 0 but they are different.
If the degree is not indicated, the permutation represented by index 0 cannot be determined.

#### Cayley table

A table of permutation multiplications is called a Cayley table.
If you use indece of premutations, the Cayley table is just a two-dimensional array of integers.
This makes it possible to perform permutation multiplication at high speed.
The Cayley table must be created separately for each degree.

#### A subset of symmetric groups.

A subset of symmetric groups can be represented on a computer as an array of index representations.
At this time, the degree and the size of the array (this is because the array itself does not have the size information of the array in C) are also required.

~~~
typedef struct _subset subset;
struct _subset {
  int degree;
  int n; / * size * /
  int * a; / * array of permutations (indeces) * /
};;
~~~

Sets are created by the `set_create_set` function.
If a set that has the same contents as a set already generated in the past are passed to the `set_create_set` function, a pointer to the set created in the past is returned.
Therefore, the sets created by the `set_create_set` function are singletons.
Therefore, do not release the memory acquired by the set in the middle of the program.
This is because other parts of the program may use the same set.
By making it a singleton, you can expect an effect in both memory efficiency　and speed.

#### The algorithm to find subgroups

The subgroup search algorithm is:

1. Find all the subgroups (cyclic groups) generated from one element.
2. Find all subgroups generated from the union of any two groups in them, except for subgroups generated in the precedure 1.
3. Repeat step 2.
4. End this operation when no new subgroups are newly generated.

In the search for the symmetric group of degree 5, 67 were found in the first search (cyclic group generated from each element) and 89 were found in the second search.
A total of 156 is the number of subgroups.
In the symmetric group of degree 6, the first search has 362, the second has 1008 and the third has 85.
A total of 1455 is the number of subgroups.

#### 使い方

demo.cは1から５次の対称群の部分群を計算して表示する。
demo.cのコンパイルと実行

~~~
$ bash demo.sh
$./a.out
~~~

demo2.cは6次の対称群の部分群を計算して表示する。
core-i7のノートパソコンで5時間24分48秒かかる。
demo2.cのコンパイルと実行

~~~
$ bash demo2.sh
$./a.out
~~~

プログラムのテスト。

~~~
$ bash test.sh
~~~

#### 置換のインデックス表示

長さ3の置換を例にとる。
1=>2,2=>3,3=>1となる置換をp[2,3,1]と書くことにする。
3次対称群は、

- p[1,2,3]
- p[1,3,2]
- p[2,1,3]
- p[2,3,1]
- p[3,1,2]
- p[3,2,1]

の6つである。
上の６つの置換は「辞書順」に並んでいる。
同様にn次対称群もその要素を辞書順に並べることができる。
そして、ある置換が前から数えてi番目であるとき、i-1をその置換のインデックスという。
インデックスは0から始まる番号である。

インデックスによってn次対称群の要素は0からn!-1までの整数で表される。
インデックス表示する場合は、その置換の次数をあらかじめ示しておかなければならない。
p[1,2]もp[1,2,3]もインデックスは0である。
次数が示されなければ、インデックス0が表す置換を決定することはできない。

#### ケーリー表

置換の乗算を表にしたものをケーリー表という。
インデックス表示を使えばPC内ではケーリー表は整数の2重配列で表される。
これにより置換の乗算を高速に行うことができる。
ケーリー表は次数ごとに別々に作成しなければならない。

#### 対称群の部分集合。

対称群の部分集合は、コンピュータ上ではインデックス表示の配列で表すことができる。
このとき、次数と配列の要素の個数（これはCでは配列の大きさ情報を配列自身が持たないため）も必要になる。

~~~
typedef struct _subset subset;
struct _subset {
  int degree;
  int n; /* size */
  int *a; /* array of permutations (indeces) */
};
~~~

集合は$set_create_set$関数で生成する。
このとき、過去にすでに生成された集合と同じ内容を$set_create_set$関数に渡すと、過去に生成済みの集合のポインタを返す。
したがって、$set_create_set$関数で生成された集合はシングルトンである。
そのため、プログラムの途中で集合で取得したメモリを解放してはいけない。
プログラムの他の部分で同じ集合を使っているかもしれないからである。
シングルトンにすることによって、メモリと速度の両方で効果が期待できる。

#### 部分群探索のアルゴリズム

部分群探索のアルゴリズムは

1. 1つの要素から生成される部分群（巡回群）をすべて求める。
2. それらの中の任意の2つの群の和集合から生成される部分群で1の部分群以外のものを全て求める。
3. この手続きで求められた群について2の操作を繰り返し、新たに得られた部分群を全て求める。
4. 新たな部分群が得られなくなったらこの操作を終了する。

である。
5次対称群の探索では、1回めの探索（各要素の巡回群）で67個、2回めの探索で89個を発見して終了した。
合計156個が部分群の総数である。
6次対称群の探索では、1回めの探索（各要素の巡回群）で362個、2回めの探索で1008個、3回めで85を発見して終了した。
合計1455個が部分群の総数である。
