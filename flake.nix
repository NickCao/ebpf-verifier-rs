{
  inputs = {
    nixpkgs.url = "github:NickCao/nixpkgs/nixos-unstable-small";
    flake-utils.url = "github:numtide/flake-utils";
    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
    ebpf-verifier = {
      url = "github:NickCao/ebpf-verifier/rust";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };
  outputs = { self, nixpkgs, flake-utils, rust-overlay, ebpf-verifier }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ rust-overlay.overlay ];
          };
        in
        {
          devShell = pkgs.mkShell {
            LIBCLANG_PATH = "${pkgs.llvmPackages.libclang.lib}/lib";
            buildInputs = with pkgs; [
              (rust-bin.selectLatestNightlyWith (toolchain:
                toolchain.default.override {
                  extensions = [ "rust-analyzer-preview" ];
                }))
            ];
          };
        }
      );
}
