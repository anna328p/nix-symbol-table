{ syms, hello, ... }:

let
	hello2 = syms.toSymbol "hello";
in
	hello == hello2
