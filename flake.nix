{
  description = "Terminal Logger";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        llvm = pkgs.llvmPackages_latest;
        lib = nixpkgs.lib;
      in
      {
        devShell = pkgs.mkShell {
          nativeBuildInputs = [
            pkgs.cmake
            llvm.lldb
            pkgs.clang-tools
            llvm.clang
            pkgs.cli11
            pkgs.ftxui
          ];

          buildInputs = [
            llvm.libcxx
          ];

          CPATH = builtins.concatStringsSep ":" [
            (lib.makeSearchPathOutput "dev" "include" [ llvm.libcxx ])
            (lib.makeSearchPath "resource-root/include" [ llvm.clang ])
          ];
          CMAKE_PREFIX_PATH = "${pkgs.cli11}";
        };
      }
    );
}
