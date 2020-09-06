compile:
	g++ main.cpp filesapi.cpp pathutilities.cpp shell.cpp -o fsexplorer

debug:
	g++ main.cpp filesapi.cpp pathutilities.cpp shell.cpp -g

clean:
	rm a.out fsexplorer