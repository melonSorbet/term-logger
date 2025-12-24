{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell.override { stdenv = pkgs.clang19Stdenv; } {
  buildInputs = [
    pkgs.clang-tools
    pkgs.gcc        # compiler + std
    pkgs.cmake
    pkgs.gnumake
    pkgs.ninja
    pkgs.gdb        # optional but useful
    pkgs.pkg-config # optional
    pkgs.bear
    pkgs.libgcc
    pkgs.gdb
    pkgs.llvmPackages_latest.lldb
    pkgs.llvmPackages_latest.libllvm
    pkgs.llvmPackages_latest.libcxx
    pkgs.cli11
  ];
}
