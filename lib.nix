{ nix-symbol-table }:

builtins.importNative
	"${nix-symbol-table}/lib/nix-symbol-table.so"
	"initialize"
