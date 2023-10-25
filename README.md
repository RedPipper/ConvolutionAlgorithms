# Convolution algorithms
## Runtime tests for java
Indexed algorithm runtime 
| Tip Matrice | Nr threads | Timp executie | |||
|--|--|--|--|--|--|
| M= (10,10)  K= (3,3) | secvential | indexed: 788 | lined: 730 |columned: 758|
| | 2 | indexed: 879 | lined: 836 | columned:  823|
| | 4 | indexed: 1099 | lined: 1102 | columned:  1162|
| | 8 | indexed: 1650 | lined: 1475 |  columned:  1620|
| | 16 | indexed: 2134 | lined: 2180 | columned: 2089|
| M=(1000,1000) K=(5,5) | secvential | 	indexed: 148721 |lined: 69121 | columned: 84033 |
| | 2 | indexed: 86084 |lined:49460 |columned: 52212|
| | 4 | indexed: 72966 |lined: 46897|columned: 48543|
| | 8 | indexed: 69970 |lined: 43929|columned: 41077|
| | 16 | indexed: 79116 |lined: 45812|columned: 55039|
| M=(10,10000) K= (5,5) | secvential | 	indexed: 26419| lined: 15593|columned: 1331 |
| | 2 | indexed: 28423 |lined: 17181|columned: 1591|
| | 4 | indexed: 33153 |lined: 18250|columned: 1819|
| | 8 | indexed: 49566 |lined: 23023|columned: 2895|
| | 16 | indexed: 76368 |lined: 24714|columned: 4382 |
| M=(10000,10) K= (5,5) | secvential | 	indexed: 29978 |lined:14506|columned: 1512|
| | 2 | indexed: 27462 |lined: 17301|columned: 2040|
| | 4 | indexed: 34186 |lined: 17389|columned: 2591|
| | 8 | indexed: 70579 |lined: 22626|columned: 3511|
| | 16 | indexed: 94891 |lined: 21197|columned: 4410|

## Runtime tests for C++, static

| Tip Matrice | Nr threads | indexed | lined | columned|
|--|--|--|--|--|
| M= (10,10)  K= (3,3) | 2| 174| 214 |194 |
| | 4|303 |354 |250 |
| | 8|682 | 526| 578|
| | 16|715 | 740|729 |
| M=(1000,1000) K=(5,5) | 2| 37271 | 44439|56745 |
| | 4|19408 |26417 | 31294|
| | 8|19162 | 28136|32342 |
| | 16|19108 |24062 |34049 |
| M=(10,10000) K= (5,5) | 2| 3670|4303 |4318 |
| | 4|2386 |2765 |2490 |
| | 8|2109 |2768 | 2610|
| | 16|2199 |3020 | 2617|
| M=(10000,10) K= (5,5) |2 | 13895|30238 |61182 |
| | 4| 9776|20603 |51844 |
| | 8|7502 |18780 |69617 |
| | 16|7550 | 17834|68967 |

## Runtime for C++ tests, dynamic

| Tip Matrice | Nr threads | indexed | lined | columned|
|--|--|--|--|--|
| M= (10,10)  K= (3,3) | 2| 177| 177 |154 |
| | 4|307 |307 |293 |
| | 8|491 | 479| 437|
| | 16|571 | 787|600 |
| M=(1000,1000) K=(5,5) | 2| 28057 | 46340|51136 |
| | 4|13732 |26572 | 27559|
| | 8|11841 | 25602|25198 |
| | 16|12375 |23578 |27145 |
| M=(10,10000) K= (5,5) | 2| 3108|4774 |4716 |
| | 4|1921 |2842 |2536 |
| | 8|1609 |2818 | 2648|
| | 16|1674 |3158| 2694|
| M=(10000,10) K= (5,5) |2 | 2757|4708 |6373 |
| | 4| 2012|2751 |3496 |
| | 8|1616 |2510 |2987 |
| | 16|1783 | 2686|3619 |

Am observat ca, in general, algoritmul "indexed" este mai rapid decat celelalte doua. In schimb, exista o corelatie intre dimensiunile datelor si performanta algoritmului folosit. Pentru dimensiuni foarte mici, performanta este arbitrara intre toati cei 3 algoritmi.
Cum era de asteptat, variantele algoritmilor in java sunt cu mult mai lente fata de cele din C++.  
Am observat ca minimul timpului de executie este intotdeauna la rularile de 4, respectiv 8 threaduri. La 16 (si respectiv 32) timpul de rulare creste => suspectez ca aceasta anomalie este influentata de numarul de threaduri per procesor fizic.


