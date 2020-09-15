compile:
	g++ main.cpp filesapi.cpp pathutilities.cpp shell.cpp X11Engine.cpp -lX11 -lstdc++fs -o fsexplorer.out

debug:
	g++ main.cpp filesapi.cpp pathutilities.cpp shell.cpp X11Engine.cpp -lX11 -g -lstdc++fs

clean:
	rm a.out fsexplorer.out

run:
	./fsexplorer.out