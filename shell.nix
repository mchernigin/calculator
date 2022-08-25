{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
    nativeBuildInputs = [
        pkgs.flex
        pkgs.bison
        pkgs.gnumake

        pkgs.python39
        pkgs.python39Packages.matplotlib
        pkgs.python39Packages.seaborn
    ];
}

