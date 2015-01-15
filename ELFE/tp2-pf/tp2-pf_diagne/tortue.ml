(* 3.2 La tortue graphique *)

open Lsysteme ;;

open Graphics ;;

type etat = { x : float ; y : float ; cap : float }

let pi = 4.*.atan(1.) ;;

(* Question 5 *)

let avance = fun e d ->
	let x' = e.x +. (d *. (cos e.cap)) and y' = e.y +. (d *. (sin e.cap))
	in
	lineto (int_of_float x') (int_of_float y'); { x = x' ; y = y' ; cap = e.cap }

(* Question 6 *)

let vole = fun e d ->
	let x' = e.x +. (d *. (cos e.cap)) and y' = e.y -. (d *. (sin e.cap))
	in
	moveto (int_of_float x') (int_of_float y'); { x = x' ; y = y' ; cap = e.cap }

(* Question 7 *)

let tourne_gauche = fun e a -> { x = e.x ; y = e.y ; cap = e.cap +. a }

let tourne_droite = fun e a -> { x = e.x ; y = e.y ; cap = e.cap -. a }

(* Question 8 *)

let memoire = Stack.create();;

let memorise = function e -> Stack.push e memoire; e

let rappelle = function () -> Stack.pop memoire

let interprete_symbole = fun d a e s ->
	match s with
		| A -> avance e d
		| G -> tourne_gauche e a
		| D -> tourne_droite e a
		| M -> memorise e
		| R -> rappelle ()
		| _ -> e

(* Question 9 *)

let rec interprete_mot = fun d a e m ->
	match m with
		| x::r -> interprete_mot d a (interprete_symbole d a e x) r
		| [] -> ()

(* 3.3 Paramètres graphiques associés aux L-systèmes *)

type param_graphiques = { pas : float ; facteur : float ; angle : float ; etat : etat }

let liste_parametres = [(vonKoch1,{ pas = 540. ; facteur = (1./.3.) ; angle = (pi/.3.) ; etat = { x = 50. ; y = 50. ; cap = 0. } });
						(vonKoch2,{ pas = 360. ; facteur = (1./.3.) ; angle = (pi/.3.) ; etat = { x = 100. ; y = 360. ; cap = 0. } });
						(vonKoch3,{ pas = 360. ; facteur = (1./.3.) ; angle = (pi/.3.) ; etat = { x = 100. ; y = 100. ; cap = 0. } });
						(hilbert,{ pas = 400. ; facteur = (1./.2.) ; angle = (pi/.2.) ; etat = { x = 150. ; y = 400. ; cap = 0. } });
						(peano,{ pas = 400. ; facteur = (1./.3.) ; angle = (pi/.2.) ; etat = { x = 100. ; y = 240. ; cap = 0. } });
						(ileDeKoch,{ pas = 300. ; facteur = (1./.4.) ; angle = (pi/.2.) ; etat = { x = 150. ; y = 100. ; cap = 0. } });
						(brindille,{ pas = 390. ; facteur = (1./.3.) ; angle = ((25.*.pi)/.180.) ; etat = { x = 320. ; y = 50. ; cap = (pi/.2.) } });
						(broussaille,{ pas = 280. ; facteur = (2./.5.) ; angle = ((25.*.pi)/.180.) ; etat = { x = 320. ; y = 50. ; cap = (pi/.2.) } })
						] ;;

(* Question 10 *)

(* liste_parametres est une liste de couples (l_systeme * param_graphiques) *)

(* Question 11 *)

let rec param_graph = fun ls -> List.assq ls liste_parametres

(* Question 12 *)

let longueur_segment = fun ls n -> (param_graph ls).pas *. ((param_graph ls).facteur ** (float_of_int n))

(* 3.4 La procédure dessine *)

let dessine = fun ls n -> clear_graph(); moveto (int_of_float (param_graph ls).etat.x) (int_of_float (param_graph ls).etat.y);
						interprete_mot (longueur_segment ls n) (param_graph ls).angle (param_graph ls).etat (derive_nieme ls n)


(******************************************** production d'un exécutable ****************************************************)

let string_to_ls = function s ->
	match s with
		| "vonKoch1" -> vonKoch1
		| "vonKoch2" -> vonKoch2
		| "vonKoch3" -> vonKoch3
		| "hilbert" -> hilbert
		| "peano" -> peano
		| "ileDeKoch" -> ileDeKoch
		| "brindille" -> brindille
		| "broussaille" -> broussaille
		| _ -> failwith "string_to_ls : pas un l_systeme"

let usage = fun () ->
	Printf.printf "USAGE : ./mlsysteme ls n\n";
	Printf.printf "		Ce programme dessine la dérivée n-ième du l-systeme ls\n";
	Printf.printf "ARGUMENTS :\n";
	Printf.printf "	- ls peut prendre l'une de ces valeurs :\n";
	Printf.printf "		vonKoch1 (n entre 0 et 9)\n";
	Printf.printf "		vonKoch2 (n entre 0 et 8)\n";
	Printf.printf "		vonKoch3 (n entre 0 et 8)\n";
	Printf.printf "		hilbert (n entre 0 et 8)\n";
	Printf.printf "		peano (n entre 0 et 6)\n";
	Printf.printf "		ileDeKoch (n entre 0 et 5)\n";
	Printf.printf "		brindille (n entre 0 et 8)\n";
	Printf.printf "		broussaille (n entre 0 et 6)\n";
	Printf.printf "	- n un entier (la valeur absolue de n sera considérée)\n"


let _ =
	if ((Array.length Sys.argv < 3) || (Sys.argv.(1) = "-h")) then
		usage()
	else
		try
			begin
				let ls = string_to_ls Sys.argv.(1) and n = abs (int_of_string Sys.argv.(2))
				in
				open_graph " 640x480";
				dessine ls n;
				ignore(read_key())
			end
		with
			| Failure("string_to_ls : pas un l_systeme") -> usage();
			| Stack_overflow -> usage()
