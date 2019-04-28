all: dist
	g++ src/main_cli.cpp -o dist/icon_player_cli.exe -w -s
	g++ src/main_gui.cpp -o dist/icon_player.exe -w -s -lfltk
	cp dlls/*.dll dist
	cp -r utils dist/utils
clean:
	rm -r dist

dist:
	mkdir dist
