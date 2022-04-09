{
  stdenv,
  cmake,
  cli,
  ...
}:
stdenv.mkDerivation {
  name = "aws";
  src = ./.;
  nativeBuildInputs = [cmake];
  buildInputs = [cli];
}
