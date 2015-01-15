(**
   Evaluateur de phrases pour mini CAML
   Evaluation par valeurs. Liaison dynamique.

   Necessite les modules [Lexical] et [Syntaxe].

   Compilation
   - [ocamlc -c lexical.cmo syntaxe.cmo evaluation.ml{i}]

   @author Salla DIAGNE
*)

(* directive pour ne pas prefixer toutes les fonctions du module 
   Syntaxe
*)
open Syntaxe

type valeur = 
    Valeur of Syntaxe.expression 
  | Liaison of (string * valeur) 
  | Aucune

and environnement = (string * valeur) list

exception Variable_non_liee of string
exception Condition_incorrecte
exception Application_impossible
exception Typage_incorrect

let rec subs = fun x e1 e2 ->
  match e1 with
    | Expr_var y when x = y -> e2
    | Expr_binaire (op_bin,exp1,exp2) -> Expr_binaire (op_bin,subs x exp1 e2,subs x exp2 e2) 
    | Expr_unaire (op_un,exp) -> Expr_unaire (op_un,subs x exp e2)
    | Expr_cond (exp1,exp2,exp3) -> Expr_cond (subs x exp1 e2,subs x exp2 e2,subs x exp3 e2)
    | Expr_fonc (f,exp) when f <> x -> Expr_fonc (f,subs x exp e2)
    | Expr_appl (exp1,exp2) -> Expr_appl(subs x exp1 e2,subs x exp2 e2)
    | _ -> e1


let rec eval_expr = fun env -> function
  | Expr_int n -> Valeur (Expr_int n)
  | Expr_bool b -> Valeur (Expr_bool b)
  | Expr_var s -> (try List.assoc s env with | Not_found -> raise (Variable_non_liee s))
  | Expr_fonc (f,e) -> Valeur (Expr_fonc(f,e))
  | Expr_unaire (op_un,exp) ->
    (match op_un,eval_expr env exp with
        | Moins, Valeur (Expr_int n) -> Valeur (Expr_int (-n))
        | Not, Valeur (Expr_bool b) -> Valeur (Expr_bool (not(b)))
        | _ -> raise Typage_incorrect
    )
  | Expr_binaire (op_bin,exp1,exp2) ->
    (match op_bin,eval_expr env exp1,eval_expr env exp2 with
        | Add, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_int (n1 + n2))
        | Sub, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_int (n1 - n2))
        | Mult, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_int (n1 * n2))
        | Div, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_int (n1 / n2))
        | Eq, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_bool (n1 = n2))
        | Inf, Valeur (Expr_int n1), Valeur (Expr_int n2) -> Valeur(Expr_bool (n1 <= n2))

        | And, Valeur (Expr_bool b1), Valeur (Expr_bool b2) -> Valeur(Expr_bool (b1 && b2))
        | Or, Valeur (Expr_bool b1), Valeur (Expr_bool b2) -> Valeur(Expr_bool (b1 || b2))
        | Eq, Valeur (Expr_bool b1), Valeur (Expr_bool b2) -> Valeur(Expr_bool (b1 = b2))

        | _ -> raise Typage_incorrect
    )
  | Expr_cond (exp1,exp2,exp3) ->
      (match eval_expr env exp1 with
        | Valeur (Expr_bool true) -> eval_expr env exp2
        | Valeur (Expr_bool false) -> eval_expr env exp3
        | _ -> raise Condition_incorrecte
      )
  | Expr_appl (e1,e2) ->
    let v1 = eval_expr env e1 and v2 = eval_expr env e2
    in
    (match v1,v2 with
      | Valeur(Expr_fonc(x,e)),Valeur e3 -> eval_expr env (subs x e e3)
      | _ -> raise Application_impossible
    )


let eval = fun env p ->
  match p with
    | Decl (Decla (x,e)) -> 
      let v = eval_expr env e in
	((x, v)::env, Liaison (x, v))
    | Expr e -> (env, eval_expr env e)
    | Vide -> (env, Aucune)
