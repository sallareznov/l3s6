(* Question 1 *)

type int_etendu = Non_defini | Int of int

let quotient = fun x y ->
	match y with
		| 0 -> Non_defini
		| _ -> Int(x/y)

(* Question 2 *)

type liste_entiers = Vide | Cons of (int * liste_entiers)

let l = Cons(1,(Cons(2,(Cons (3,Cons(1,Vide))))))

(* Question 3 *)

type booleen = Vrai | Faux

let et = function
	| Vrai,Vrai -> Vrai
	| _ -> Faux

let negation = function
	| Vrai -> Faux
	| Faux -> Vrai

(* Question 4 *)

let plus = fun n1 n2 ->
	match n1,n2 with
		| Int n, Int p -> Int(n+p)
		| _ -> Non_defini

let quotient_etendu = fun n1 n2 ->
	match n1,n2 with
		| Int n, Int p -> quotient n p
		| _ -> Non_defini

(* Question 5 *)

let rec longueur = function l ->
	match l with
		| Vide -> 0
		| Cons (a,b) -> 1 + longueur b

(* Question 6 *)

let egal = fun n1 n2 ->
	match n1,n2 with
		| Int n, Int p when n = p -> Vrai
		| Non_defini, Non_defini -> Vrai
		| _ -> Faux

let rec rang = fun n l ->
	match l with
		| Vide -> Non_defini
		| Cons(a,_) when a = n -> Int 0
		| Cons(_,r) -> plus (Int 1) (rang n r) ;;


(* Question 11 *)

(* Voir fichier syntaxe.ml *)

(* Question 12 *)

(* Voir fichier tp4.txt *)