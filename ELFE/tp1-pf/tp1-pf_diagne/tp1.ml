(* Exercice 1 : Fonctions du module List *)

let rec mnth = fun (l : int list) i ->
	match l,i with
		| x::_,0 -> x
		| _::r,i -> mnth r (i - 1)
		| [],_ -> failwith "nth"


let rec mappend = fun (l1 : int list) (l2 : int list) ->
	match l1 with
		| x::r -> x::(mappend r l2)
		| _ -> l2


let rec mflatten = fun l ->
	match l with
		| x::r -> x@(mflatten r)
		| _ -> []


let rec mmap = fun f l ->
	match l with
		| x::r -> (f x)::(mmap f r)
		| _ -> []


let rec mfold_left = fun f n l ->
	match l with
		| x::r -> mfold_left f (f n x) r
		| _ -> n

(* Exercice 2 *)

(* Si l est une 'a list, la déclaration let x = nth l déclare une fonction x qui prend en paramètre un entier i et renvoie l'élément à l'indice i de la liste l *)

(* Exercice 3 *)

(* Question 1 *)
let somme = function l -> List.fold_left (+) 0 l

(* Question 2 *)
let mlength = function l ->
	match l with
		| [] -> 0
		| _ -> (somme (mmap (function x -> (somme l)) l) / (somme l))

(* Exercice 4 : Liste d'itérés *)

(* Question 1 *)
let rec miter = fun f x n ->
	match n with
		| 0 -> []
		| _ -> x::(miter f x (n-1))

(* Question 2 *)
let entiers_consecutifs = function n -> miter (function x -> x + 1) (-1) (n + 1)
