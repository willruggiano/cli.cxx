{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    utils.url = "github:gytis-ivaskevicius/flake-utils-plus";
    pre-commit.url = "github:cachix/pre-commit-hooks.nix";
  };

  outputs = {
    self,
    nixpkgs,
    utils,
    ...
  } @ inputs:
    utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages."${system}";
      cli-cxx = pkgs.callPackage ./cli {};
      aws = pkgs.callPackage ./apps/aws {inherit cli-cxx;};
      hello = pkgs.callPackage ./apps/hello {inherit cli-cxx;};
    in rec {
      apps = {
        aws = utils.lib.mkApp {
          drv = aws;
        };
        hello = utils.lib.mkApp {
          drv = hello;
        };
      };

      checks = {
        pre-commit = inputs.pre-commit.lib."${system}".run {
          src = ./.;
          hooks = let
            pre-commit-hooks = "${pkgs.python3Packages.pre-commit-hooks}/bin";
          in {
            alejandra.enable = true;
            check-executables-have-shebangs = {
              entry = "${pre-commit-hooks}/check-executables-have-shebangs";
              types = ["text" "executable"];
            };
            check-json = {
              enable = true;
              entry = "${pre-commit-hooks}/check-json";
              types = ["json"];
            };
            check-merge-conflict = {
              enable = true;
              entry = "${pre-commit-hooks}/check-merge-conflict";
              types = ["text"];
            };
            check-toml = {
              enable = true;
              entry = "${pre-commit-hooks}/check-toml";
              types = ["toml"];
            };
            check-yaml = {
              enable = true;
              entry = "${pre-commit-hooks}/check-yaml";
              types = ["yaml"];
            };
            clang-format = {
              enable = true;
              types_or = ["c" "c++"];
            };
            end-of-file-fixer = {
              enable = true;
              entry = "${pre-commit-hooks}/end-of-file-fixer";
              types = ["text"];
            };
            trailing-whitespace = {
              enable = true;
              entry = "${pre-commit-hooks}/trailing-whitespace-fixer";
              types = ["text"];
            };
          };
        };

        tests = cli-cxx;
      };

      devShells.default = pkgs.mkShell {
        name = "cli.cxx";
        nativeBuildInputs = with pkgs; [
          clang-tools
          cmake
          lldb_12 # for debugging
        ];
        buildInputs = with pkgs; [
          doctest
        ];
        inherit (self.checks."${system}".pre-commit) shellHook;
      };

      overlays.default = final: prev: {
        inherit cli-cxx;
      };

      packages = {
        inherit cli-cxx aws hello;
      };
    });
}
