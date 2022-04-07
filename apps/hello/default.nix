{
  stdenv,
  cmake,
  cli,
}:
stdenv.mkDerivation {
  name = "hello";
  src = ./.;
  nativeBuildInputs = [cmake];
  buildInputs = [cli];
}
