type symbole =
	| A
	| G
	| D
	| X
	| Y
	| M
	| R

type mot = symbole list

type l_systeme = { axiome : mot ; regles : symbole -> mot }

let vonKoch1 = { axiome = [A] ; regles = function | A -> [A;G;A;D;D;A;G;A] | x -> [x] } ;;

let vonKoch2 = { axiome = [A;D;D;A;D;D;A;D;D] ; regles = function | A -> [A;G;A;D;D;A;G;A] | x -> [x] } ;;

let vonKoch3 = { axiome = [A;G;G;A;G;G;A;G;G] ; regles = function | A -> [A;G;A;D;D;A;G;A] | x -> [x] } ;;

let hilbert = { axiome = [X] ; regles = function | X -> [D;Y;A;G;X;A;X;G;A;Y;D] | Y -> [G;X;A;D;Y;A;Y;D;A;X;G] | x -> [x] } ;;

let peano = { axiome = [A] ; regles = function | A -> [A;A;G;A;G;A;G;A;A;G;A;G;A;D;A] | x -> [x] } ;;

let ileDeKoch = { axiome = [A;G;A;G;A;G;A] ; regles = function | A -> [A;G;A;D;A;D;A;A;G;A;G;A;D;A] | x -> [x] } ;;

let brindille = { axiome = [A] ; regles = function | A -> [A;M;G;A;R;A;M;D;A;R;A] | x -> [x] }

let broussaille = { axiome = [A] ; regles = function | A -> [A;A;G;M;G;A;D;A;D;A;R;D;M;D;A;G;A;G;A;R] | x -> [x] }

(* Question 1 *)

let symbole_2_mot = fun ls s -> ls.regles(s)

(* Question 2 *)

let rec transforme_mot = fun ls m ->
	match m with
		| x::r -> (symbole_2_mot ls x)@(transforme_mot ls r)
		| [] -> []

(* Question 3 *)

let rec itere = fun f n e ->
	match n with
		| 0 -> e
		| _ -> itere f (n - 1) (f e)

let derive_nieme = fun ls n -> itere (function m -> transforme_mot ls m) n ls.axiome ;;

(* Question 4 *)

derive_nieme vonKoch1 3 ;;
derive_nieme hilbert 4 ;;
derive_nieme ileDeKoch 2 ;;
derive_nieme peano 5 ;;
