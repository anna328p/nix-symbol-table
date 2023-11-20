let
	syms = builtins.importNative ./symbol-table.so "initialize";

	inherit (syms) toSymbol;

	hello = toSymbol "hello";
in
	import ./test2.nix { inherit syms hello; }
