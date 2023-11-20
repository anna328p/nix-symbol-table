{
	description = "Nix symbol table plugin";

	nixConfig = {
		allow-unsafe-native-code-during-evaluation = true;
	};

	outputs = { self
		, nixpkgs
	}: let
		inherit (nixpkgs) lib;

		forEachSystem' = env: body:
			lib.genAttrs lib.systems.flakeExposed (system: body (env system));

		forEachSystem = forEachSystem' (system: {
			inherit system;

			pkgs = nixpkgs.legacyPackages.${system};
		});
	in {
		packages = forEachSystem (env: let
			inherit (env) pkgs;
		in rec {
			nix-symbol-table = pkgs.callPackage ./. {};

			lib = import ./lib.nix { inherit nix-symbol-table; };

			default = nix-symbol-table;
		});
	};
}
