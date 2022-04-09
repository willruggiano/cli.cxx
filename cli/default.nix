{
  stdenv,
  cmake,
  doctest,
  ...
}:
stdenv.mkDerivation {
  name = "cli";
  src = ./.;
  nativeBuildInputs = [cmake];
  buildInputs = [doctest];
  doCheck = true;
}
