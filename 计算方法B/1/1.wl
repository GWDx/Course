(* ::Package:: *)

(* ::Input:: *)
(*Clear["`*"] *)


(* ::Section:: *)
(*2*)


(* ::Subsection:: *)
(*1*)


(* ::Input:: *)
(*raw=({*)
(* {1920, 1930, 1940, 1950, 1960, 1970},*)
(* {105711, 123203, 131669, 150697, 179323, 203212}*)
(*});*)


(* ::Input:: *)
(*data=Transpose@raw;*)


(* ::Input:: *)
(*L[data_]:=Module[{n=Length@data-1,l,*)
(*allX=Transpose[data][[1]],allF=Transpose[data][[2]]},*)
(*l[k_]:=Product[(x-allX[[i]])/(allX[[k]]-allX[[i]]),{i,Drop[Range[n+1],{k}]}];*)
(*Sum[l[i]allF[[i]],{i,n+1}]]*)


(* ::Input:: *)
(*lagrange=L[data]//Simplify*)


(* ::Input:: *)
(*lagrange/.x->{1910,1965,2002}//N*)


(* ::Subsubsection:: *)
(*check*)


(* ::Input:: *)
(*in=Interpolation[data,InterpolationOrder->5];*)
(*predict=in/@{1910,1965,2002,2012}//N*)


(* ::Subsection:: *)
(*2*)


(* ::Input:: *)
(*data2=Reverse@Prepend[data,{1910,91772}]*)


(* ::Input:: *)
(*allX=Transpose[data2][[1]];*)


(* ::Input:: *)
(*lagrange1=L[Most@data2]//Simplify;*)
(*lagrange2=L[Rest@data2]//Simplify;*)


(* ::Input:: *)
(*r[x_]=(x-allX[[1]])/(allX[[1]]-allX[[-1]]) (lagrange1-lagrange2);*)


(* ::Input:: *)
(*r/@{1965,2002}//N*)


(* ::Section:: *)
(*3*)


(* ::Input:: *)
(*Clear[f]*)


(* ::Input:: *)
(*f[x__]:=(f@@Rest@{x}-f@@Most@{x})/(Last@{x}-First@{x})/;Length@{x}>=2*)


(* ::Input:: *)
(*Set[f[#1],#2]&@@@data;*)


(* ::Input:: *)
(*allX=Transpose[data][[1]];*)
(*allF=Transpose[data][[2]];*)
(*n=Length@data-1;*)


(* ::Input:: *)
(*newton[x_]=f@allX[[1]]+Sum[f@@Take[allX,k+1]Product[x-allX[[i]],{i,k}],{k,n}];*)


(* ::Input:: *)
(*newton/@{1965,2012}//N*)
