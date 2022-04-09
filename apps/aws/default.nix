{
  stdenv,
  cmake,
  cli-cxx,
  ...
}:
stdenv.mkDerivation {
  name = "aws";
  src = ./.;
  nativeBuildInputs = [cmake];
  buildInputs = [cli-cxx];
}
