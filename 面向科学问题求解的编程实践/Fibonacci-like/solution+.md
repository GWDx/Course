

$$
\left(\begin{array}{c}
 \bf{a} \\ \bf{m} \\ \bf{p} \\
\end{array} \right) = \left(
\begin{array}{cccccccccccccccccc}
 0 & 0 & 0 & 7 & 7 & 14 & 2 & 1 & 13 & 11 & 4 & 14 & 10 & 23 & 23 & 31 & 22 & 43 \\
 3 & 4 & 5 & 9 & 10 & 15 & 8 & 18 & 20 & 30 & 45 & 24 & 36 & 40 & 60 & 90 & 72 & 120 \\
 2 & 3 & 5 & 17 & 11 & 61 & 7 & 19 & 41 & 31 & 109441 & 23 & 107 & 2161 & 2521 & 181 & 103681 & 241 \\
\end{array} \right)
$$


$$
A_0=114890854840046588673257358847750200\\
A_1=17045109889786284033752421414514643
$$

$$
\begin{align}
&1\leq a_k\leq m_k\\
\forall n,\exist k,&\ n\equiv a_k\pmod{m_k}\\
&p_k\in P,\ p_k|F_{m_k}\\
\forall k& \neq l,\ p_k\neq p_l
\end{align}
$$


解释 $A_n$ 可被 $p_k$ 整除但不等于 $p_k$ 的原因

### $A_n$ 可被 $p_k$ 整除

$$
\begin{align}
\forall k,\ \ F_{m_k-a_k}&\equiv A_0\pmod{p_k}\\
\forall k,\ \ F_{m_k-a_k+1}&\equiv A_1\pmod{p_k}\\
\end{align}
$$

$$
A_{m+n}=A_mF_{n-1}+A_{m+1}F_n
$$



$\forall n,\exist k,\ n\equiv a_k\pmod{m_k}\\$

1. 若 $n<m_k$，则 $n=a_k$
   $$
   \begin{align}
   A_n&=A_{a_k}=A_0F_{a_k-1}+A_1F_{a_k}\\
   &\equiv F_{m_k-a_k}F_{a_k-1}+F_{m_k-a_k+1}F_{a_k}=\ F_{m_k}\\
   &\equiv\ 0\pmod{p_k}
   \end{align}
   $$

2. 若 $A_n\equiv 0\pmod{p_k}$，则
   $$
   \begin{align}
   A_{n+m_k}&=A_{n}F_{m_k-1}+A_{n+1}F_{m_k}\\
   &\equiv\ 0\pmod{p_k}
   \end{align}
   $$

因此 $\forall n,\exist k,\ A_n\equiv0\pmod{p_k}$

### $A_n\neq p_k$

对于任意 $k$

$A_0>p_k>0$，$A_1>p_k>0$

对于 $n\geq 2$

$A_{n}=A_{n-1}+A_{n-2}>A_1>p_k$



