(* ::Package:: *)

primeFactor[x_]:=First/@FactorInteger[x,Automatic]
allRange=Flatten@Table[2^i 3^j 5^k,{i,0,20},{j,0,15},{k,0,10}];
range=DeleteCases[Select[Union@allRange,#<=400&],1|2];
primes=DeleteCases[primeFactor/@Fibonacci@range//Flatten//Union,1];
{m,p}=Transpose@SortBy[First]@
Table[{FirstCase[range,a_/;Divisible[Fibonacci@a,p]],p},{p,primes}];
factorLessThan[u_,v_,w_]:=Cases[{#,Lookup[Association[Rule@@@FactorInteger@#],{2,3,5},0]}&/@m,{i_,{a_,b_,c_}}:>i/;a<=u&&b<=v&&c<=w]
factorLessThan[4,1,1]
factorLessThan[3,2,1]


mm={3,4,5,8,9,10,15,18,20,24,30,36,40,45,60,72,90,120};
ansM=mm=Join[{3,4,5},SortBy[Drop[mm,3],PrimeOmega]];

remain[aa_]:=Block[{i,m,f,ans},
i=Length@aa;
m=Take[mm,i];
f[x_]=And@@(Mod[x,#2]!=#1&@@@Transpose@{aa,m});
Select[Range[LCM@@mm],f]]
len[aa_]:=Length@remain[aa]
comm[aa_]:=Commonest@Mod[remain[aa],mm[[Length@aa+1]]]

allAnsA={};
updateAllAnsA:=Block[{},Clear[AA];
AA[3]={{0,0,0}};
AA[i_]:=AA[i]=Block[{aa=AA[i-1]},
RandomChoice[Flatten[Table[Append[u,v],{u,aa},{v,comm[u]}],1],5]];allAnsA=Union[allAnsA,Select[AA[18],len@#==0&]];]
While[Length@allAnsA==0,updateAllAnsA]
unnormalizedAnsA=RandomChoice@allAnsA;
ansA=Mod[unnormalizedAnsA-1,ansM]+1


ansP=ansM/.Thread[m->p];
sumA1A2[ansM_,ansA_]:=Block[{},
ChineseRemainder[Fibonacci[ansM-ansA],ansP]+2ChineseRemainder[Fibonacci[ansM-ansA+1],ansP]]


Clear[A]
A[m_]:=A[m]=A[m-1]+A[m-2]
A[0]=ChineseRemainder[Fibonacci[ansM-ansA],ansP]
A[1]=ChineseRemainder[Fibonacci[ansM-ansA+1],ansP]
GCD[A[0],A[1]]


Select[Array[A,5000],PrimeQ]


MatrixForm@{ansP,ansM,ansA}
