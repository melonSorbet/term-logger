{
  description = "Terminal Logger - devShell with LLVM, CLI11, FTXUI, fixed headers";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ flake-parts, nixpkgs, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      # Platforms we support
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      # Shells and configs per system
      perSystem =
        { pkgs, ... }:
        {
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [
              cmake
              cli11
              ftxui
              boost
              ninja
              ripgrep
            ];

            # Make CMake aware of CLI11, FTXUI, Boost
            shellHook = ''
              export CMAKE_PREFIX_PATH=${pkgs.cli11}:${pkgs.ftxui}:${pkgs.boost}
            '';
          };
        };
    };
}
