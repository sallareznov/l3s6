(**
   Evaluateur de phrases pour mini CAML
   Evaluation par valeurs. Liaison dynamique.

   Necessite les modules [Lexical] et [Syntaxe].

   Compilation
   - [ocamlc -c lexical.cmo syntaxe.cmo evaluation.ml{i}]

   @author FIL - IEEA - Univ. Lille1
*)


type valeur = Valeur of Syntaxe.expression | Liaison of (string * valeur) | Aucune
and environnement = (string * valeur) list

exception Variable_non_liee of string
exception Condition_incorrecte
exception Application_impossible
exception Typage_incorrect

(**
   [eval env p] renvoie un couple contenant un (nouvel) environnement, ainsi que la valeur de la phrase [p] evaluee dans l'environnement [env].

   Si la phrase [p] est une declaration de la forme
   [let x = e],
   l'environnement obtenu en retour est 
   [(x,v) :: env],
   dans lequel [v] est la valeur dans l'environnement [env] de l'expression [e].

   Si la phrase [p] est une expression, l'environnement obtenu en retour est [env].
*)
val eval :
  environnement -> Syntaxe.phrase -> environnement * valeur
