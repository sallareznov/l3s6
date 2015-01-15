(** 
    Boucle d'interaction du langage mini CAML

    Necessite les modules [Lexical], [Syntaxe] et [Evaluation].
    
    Compilation
    - [ocamlc -o minicaml lexical.cmo syntaxe.cmo evaluation.cmo minicaml.ml]
    @author FIL - IEEA - Univ. Lille1
*)




(**
   [imprimer v] imprime la valeur [v].
*)
val imprimer : Evaluation.valeur -> unit

(**
   [interprete env ()] lance une boucle d'interaction du type read/eval/print.

   Les phrases du langage mini CAML {b doivent} etre redigees sur une seule ligne.
*)
val interprete : (string * Evaluation.valeur) list -> unit -> 'a
