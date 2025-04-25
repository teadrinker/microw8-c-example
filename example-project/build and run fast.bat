
clang -O2 -Wno-incompatible-library-redeclaration --no-standard-libraries -ffast-math -ffinite-math-only -Xclang -target-feature -Xclang +nontrapping-fptoint -mno-exception-handling -Wl,--no-entry,--export-all,--import-memory,--initial-memory=262144,--global-base=81920,-zstack-size=4096 -o cart.wasm cart.c --target=wasm32

"%~dp0../uw8.exe" filter-exports cart.wasm cart.wasm

wasm-opt -Oz --fast-math --strip-producers -o cart.wasm cart.wasm

"%~dp0../uw8.exe" pack -l 0 cart.wasm cart.uw8

"%~dp0../uw8.exe" run cart.uw8 --fps

pause
