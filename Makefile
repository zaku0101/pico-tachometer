clean:
	rm -rf build
build:
	mkdir build
	cmake . -Bbuild -DPICO_BOARD=pico_w
	make -C build

flash:build/pico-tachometer.uf2
	picotool load -f build/pico-tachometer.uf2