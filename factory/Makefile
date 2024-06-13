build:
	./wasi-sdk-22.0/bin/clang --sysroot wasi-sdk-22.0/share/wasi-sysroot/ -Wl,--export-all wasm_factory.c -o wasm_from_factory_bin.wasm;
	python3 utils.py wasm_from_factory_bin.wasm wasm_from_factory.h;
	mv wasm_from_factory.h ../source/extra/wasm_from_factory.h

compile:
	./wasi-sdk-22.0/bin/clang --sysroot wasi-sdk-22.0/share/wasi-sysroot/ wasm_factory.c -o wasm_from_factory_bin.wasm;

transform:
	python3 utils.py wasm_from_factory_bin.wasm wasm_from_factory.h;

mouv:
	mv wasm_from_factory.h ../source/extra/wasm_from_factory.h

clean:
	rm *.wasm


#clang --target=wasm32 -nostdlib -Wl,--export=add -Wl,--no-entry -o sum_c.wasm sum.c