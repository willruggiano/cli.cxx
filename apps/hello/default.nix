{
  stdenv,
  cmake,
  cli-cxx,
}:
stdenv.mkDerivation {
  name = "hello";
  src = ./.;
  nativeBuildInputs = [cmake];
  buildInputs = [cli-cxx];
}
