(**
   Analyse syntaxique du langage mini CAML.

   Necessite le module [Lexical].

   Compilation
   - [ocamlc -c lexical.cmo syntaxe.ml{i}]

   @author FIL - IEEA - Univ. Lille1
*)


type op_bin = Add | Sub | Mult | Div | And | Or | Eq | Inf
and op_un = Moins | Not
and phrase = Decl of declaration | Expr of expression | Vide
and declaration = Decla of string * expression
and expression =
    Expr_int of int
  | Expr_bool of bool
  | Expr_var of string
  | Expr_binaire of op_bin * expression * expression
  | Expr_unaire of op_un * expression
  | Expr_cond of expression * expression * expression
  | Expr_fonc of string * expression
  | Expr_appl of expression * expression


exception Erreur of int

(**
   [analyse p] = analyse la phrase [p] et renvoie la phrase en representation abstraite.

   Declenche l'exception [Syntaxe.Erreur n] si [p] syntaxiquement incorrect. [n] est le numero d'ordre de l'element lexical sur lequel l'erreur de syntaxe a ete decouverte. 
*)
val analyse : string -> phrase

(**
   [en_chaine p] = chaine contenant une representation dans le langage mini CAML de la phrase [p].
*)
val en_chaine : phrase -> string
