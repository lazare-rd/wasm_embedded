clean:
	rm -r build
	mkdir build

build:
	cd build
	cmake ..
	cmake --build .
	cd ..

run:
	./build/wasm3_in_c

export_cube:
	mv $(file) /Users/lazarericour-dumas/STM32CubeIDE/workspace_1.14.0/wasm3_embedded_stm32h7/Core/Src/wasm3/extra/$(file)

rebuild:
	make clean
	make build