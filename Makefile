all: 
	g++ src/main_cli.cpp -o icon_player_cli.exe -w -s
	g++ src/main_gui.cpp -o icon_player.exe -w -s -lfltk
