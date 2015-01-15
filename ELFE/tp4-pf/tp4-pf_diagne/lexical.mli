(**
   Analyseur lexical pour mini CAML.

   Compilation
   - [ocamlc -c lexical.ml{i}]

   @author FIL - IEEA - Univ. Lille1
*)

(**
   [analyse s] = flot d'elements lexicaux extraits de la chaine [s].

   Ces elements lexicaux peuvent etre de type
   - [Genlex.Kwd of string] pour les mots-cles
   - [Genlex.Int of int] pour les entiers
   - [Genlex.Ident of string] pour les identificateurs de variables.

*)
val analyse : string -> Genlex.token Stream.t
