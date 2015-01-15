(** 
    Boucle d'interaction du langage mini CAML

    Necessite les modules [Lexical], [Syntaxe] et [Evaluation].

    @author FIL - IEEA - Univ. Lille1
*)



let imprimer = function
  | Evaluation.Valeur e -> Printf.printf "-> %s\n" (Syntaxe.en_chaine (Syntaxe.Expr e)) ; flush stdout
  | Evaluation.Liaison (x, Evaluation.Valeur e) -> Printf.printf "val %s = %s\n" x (Syntaxe.en_chaine (Syntaxe.Expr e)) ; flush stdout
  | Evaluation.Aucune -> Printf.printf "\n" ; flush stdout
  | _ -> () (* pour l'exhaustivite du filtrage *)

let rec interprete env () =
  try
    let _ =  Printf.printf "?- " ; flush stdout in
    let phrase = Syntaxe.analyse (input_line stdin) in
    let env', result =  Evaluation.eval env phrase in
    let _ =  imprimer result in
    interprete env' ()
  with
    | End_of_file -> exit 0
    | Syntaxe.Erreur _ -> 
      interprete env (Printf.printf "Erreur de syntaxe.\n")
    | Evaluation.Variable_non_liee x -> 
      interprete env (Printf.printf "Variable %s non liee.\n" x)
    | Evaluation.Typage_incorrect -> 
      interprete env (Printf.printf "Types incorrects.\n")
    | Evaluation.Application_impossible ->
      interprete env (Printf.printf "Application impossible.\n")

let _ = interprete [] ()
