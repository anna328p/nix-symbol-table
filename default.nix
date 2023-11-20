{ stdenv
, pkg-config
, boost
, nix }:

stdenv.mkDerivation {
	pname = "nix-symbol-table";
	version = "0.1";

	src = ./.;

	nativeBuildInputs = [ pkg-config ];
	buildInputs = [ boost nix ];

	dontConfigure = true;

	installPhase = ''
		mkdir -p "$out/lib"
		cp -t "$out/lib" nix-symbol-table.so
	'';
}
