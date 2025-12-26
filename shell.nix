{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell.override { stdenv = pkgs.clang19Stdenv; } {
  buildInputs = [
    pkgs.gcc        # compiler + std
    pkgs.cmake
    pkgs.gdb
    pkgs.cli11
  ];
  shellHook = ''
    rm -rf build compile_commands.json
    cmake -S . -B build
  '';
}
