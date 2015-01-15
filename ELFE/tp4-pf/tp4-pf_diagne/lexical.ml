(**
   Analyseur lexical pour mini CAML.

   @author FIL - IEEA - Univ. Lille1
*)


let keywords = 
  ["("; ")"; "let"; "fun"; "->"; "if"; "then"; "else"; 
   "+"; "-"; "*"; "/"; "true"; "false"; "&&"; 
   "||"; "not"; "="; "<="]

let lexer s = 
  Genlex.make_lexer keywords (Stream.of_string s)


let analyse s =
  lexer s
