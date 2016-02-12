with import <nixpkgs> {}; {
  mdoowEnv = stdenv.mkDerivation {
    name = "mdoow";
    buildInputs = [
      rPackages.devtools
    ];
    R_LIBS_SITE = "${rPackages.devtools}/library";
    R_LIBS_USER="~/R/x86_64-pc-linux-gnu-library/3.2/";
  };
}
