(**
   Analyse syntaxique du langage mini CAML.

   @author FIL - IEEA - Univ. Lille1
*)

(**
   Les declarations de types 
*)
type op_bin = (* operateurs binaires *)
    Add | Sub | Mult | Div | And | Or | Eq | Inf
and op_un = (*operateurs unaires *)
    Moins | Not 
and phrase = (* phrases du langages *)
    Decl of declaration | Expr of expression | Vide 
and declaration = (* declarations de variables *)
    Decla of string*expression
and expression = (* expressions du langages *)
  (* les expressions atomiques *)
  | Expr_int of int
  | Expr_bool of bool
  | Expr_var of string
  (* les expressions composees *)
  | Expr_binaire of op_bin*expression*expression
  | Expr_unaire of op_un*expression 
  | Expr_cond of expression * expression * expression
  | Expr_fonc of string*expression
  | Expr_appl of expression*expression

(**
*)
exception Erreur of int

let rec expr_de_flot lex fl =
  match lex with
    | Genlex.Int n -> Expr_int n
    | Genlex.Ident s -> Expr_var s
    | Genlex.Kwd s -> 
      (match s with
	| "true" -> Expr_bool true
	| "false" -> Expr_bool false
	| "(" -> expr_composee fl
	| _ -> raise (Erreur (Stream.count fl))
      ) 
    | _ -> raise (Erreur (Stream.count fl))
      
and expr_composee fl = 
  try
    let t = Stream.next fl in
    match t with
      | Genlex.Kwd "fun" -> fin_expr_fonc fl
      | Genlex.Kwd "if" -> fin_expr_cond fl
      | Genlex.Kwd "-" -> fin_expr_unaire Moins fl
      | Genlex.Kwd "not" -> fin_expr_unaire Not fl
      | _ ->
	let e = expr_de_flot t fl in
	fin_autre_expr_comp e fl
  with
    | Stream.Failure -> raise (Erreur (Stream.count fl))

and fin_expr_fonc fl =
  match Stream.next fl with
    | Genlex.Ident x ->
      (match Stream.next fl with
	| Genlex.Kwd "->" ->
	  let e = expr_de_flot (Stream.next fl) fl in
	  (match Stream.next fl with
	    | Genlex.Kwd ")" -> Expr_fonc (x, e)
	    | _ -> raise (Erreur (Stream.count fl))
	  )
	| _ -> raise (Erreur (Stream.count fl))
      )
    | _ -> raise (Erreur (Stream.count fl))

and fin_expr_cond fl =
  let e1 = expr_de_flot (Stream.next fl) fl in
  match Stream.next fl with
    | Genlex.Kwd "then" ->
      let e2 = expr_de_flot (Stream.next fl) fl in
      (match Stream.next fl with
	| Genlex.Kwd "else" ->
	  let e3 = expr_de_flot (Stream.next fl) fl in
	  (match Stream.next fl with
	    | Genlex.Kwd ")" ->	Expr_cond (e1, e2, e3)
	    | _ -> raise (Erreur (Stream.count fl))
	  )
	| _ -> raise (Erreur (Stream.count fl))
      )
    | _ -> raise (Erreur (Stream.count fl))

and fin_expr_unaire op fl =
  let e = expr_de_flot (Stream.next fl) fl in
  match Stream.next fl with
    | Genlex.Kwd ")" -> Expr_unaire (op, e)
    | _ -> raise (Erreur (Stream.count fl))

and fin_autre_expr_comp e1 fl =
  match Stream.next fl with
    | Genlex.Kwd "+" -> fin_expr_binaire e1 Add fl
    | Genlex.Kwd "-" -> fin_expr_binaire e1 Sub fl
    | Genlex.Kwd "*" -> fin_expr_binaire e1 Mult fl
    | Genlex.Kwd "/" -> fin_expr_binaire e1 Div fl
    | Genlex.Kwd "&&" -> fin_expr_binaire e1 And fl
    | Genlex.Kwd "||" -> fin_expr_binaire e1 Or fl
    | Genlex.Kwd "=" -> fin_expr_binaire e1 Eq fl
    | Genlex.Kwd "<=" -> fin_expr_binaire e1 Inf fl
    | Genlex.Kwd "(" -> fin_application e1 (expr_de_flot (Genlex.Kwd "(") fl) fl
    | Genlex.Int n -> fin_application e1 (Expr_int n) fl
    | Genlex.Ident x -> fin_application e1 (Expr_var x) fl
    | Genlex.Kwd "true" -> fin_application e1 (Expr_bool true) fl
    | Genlex.Kwd "false" -> fin_application e1 (Expr_bool false) fl
    | _ -> raise (Erreur (Stream.count fl))

and fin_expr_binaire e1 op fl = 
  let e2 = expr_de_flot (Stream.next fl) fl in
  match Stream.next fl with
    | Genlex.Kwd ")" -> Expr_binaire (op, e1, e2)
    | _ -> raise (Erreur (Stream.count fl))

and fin_application e1 e2 fl = 
  match Stream.next fl with
    | Genlex.Kwd ")" -> Expr_appl (e1, e2)
    | _ -> raise (Erreur (Stream.count fl))

let analyse s = 
  let f = (Lexical.analyse s) in
  try
    match Stream.next f with
      | Genlex.Kwd "let" ->
	(match Stream.next f with
	  | Genlex.Ident x ->
	    (match Stream.next f with
	      | Genlex.Kwd "=" ->
		let expr = expr_de_flot (Stream.next f) f in
		(try
		  let _ = Stream.next f in
		  raise (Erreur (Stream.count f))
		with
		  | Stream.Failure -> Decl (Decla (x,expr))
		)
	      | _ -> raise (Erreur (Stream.count f))
	    )
	  | _ -> raise (Erreur (Stream.count f))
	)
      | lexeme -> 
	let expr = expr_de_flot lexeme f in
	try 
	  let _ = Stream.next f in
	  raise (Erreur (Stream.count f))
	with
	  | Stream.Failure -> Expr expr
  with
    | Stream.Failure -> Vide


(**
   Conversion Phrase en chaine
*)

let op_bin_en_chaine = function op ->
  Printf.sprintf "%s" 
    (match op with
      | Add -> " + "
      | Sub -> " - "
      | Mult -> " * "
      | Div -> " / "
      | And -> " && "
      | Or -> " || "
      | Eq -> " = "
      | Inf -> " <= "
    )

let op_un_en_chaine = function op ->
  Printf.sprintf "%s" 
    (match op with
      | Moins -> "- "
      | Not -> "not "
    )


let rec expr_en_chaine = function 
  | Expr_int n -> Printf.sprintf "%d" n
  | Expr_bool b -> Printf.sprintf "%B" b
  | Expr_var x -> x
  | Expr_binaire (op,e1,e2) ->
    (Printf.sprintf "(")^
      (expr_en_chaine e1)^
      (op_bin_en_chaine op)^ 
      (expr_en_chaine e2)^
      (Printf.sprintf ")")
  | Expr_unaire (op,e1) ->
    (Printf.sprintf "(")^
      (op_un_en_chaine op)^
      (expr_en_chaine e1)^
      (Printf.sprintf ")")
  | Expr_cond (e1, e2, e3) ->
    (Printf.sprintf "(if ")^
      (expr_en_chaine e1)^
      (Printf.sprintf " then ")^
      (expr_en_chaine e2)^
      (Printf.sprintf " else ")^
      (expr_en_chaine e3)^
      (Printf.sprintf ")")
  | Expr_fonc (x, e) -> 
    (Printf.sprintf "(fun %s -> " x)^
      (expr_en_chaine e)^
      (Printf.sprintf ")") 
  | Expr_appl (e1, e2) ->
    (Printf.sprintf "(")^
      (expr_en_chaine e1)^
      (Printf.sprintf " ")^
      (expr_en_chaine e2)^
      (Printf.sprintf ")")

let en_chaine = function
  | Decl (Decla (x,e)) -> 
    (Printf.sprintf "let %s = " x)^
      (expr_en_chaine e)
  | Expr e -> expr_en_chaine e
  | Vide -> "" ;;

(* Question 11 *)
analyse "12" ;;
analyse "true" ;;
analyse "x" ;;
analyse "(x + 1)" ;;
analyse "(y && (not z))" ;;
analyse "(0 <= x)" ;;
analyse "(- x)" ;;
analyse "(not y)" ;;
analyse "(if (0 <= x) then x else (- x))" ;;
analyse "(f 3)" ;;
analyse "let f = (fun x -> (if (0 <= x) then x else (- x)))" ;;

analyse "(12 -3)"
