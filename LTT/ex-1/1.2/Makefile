build:
	g++ window1.cpp -o window1 `pkg-config --cflags --libs gtk+-3.0`
	g++ window2.cpp -o window2 `pkg-config --cflags --libs gtk+-3.0`
	g++ window3.cpp -o window3 `pkg-config --cflags --libs gtk+-3.0`
	g++ main.cpp -o main
	./main
clean:
	rm window1 window2 window3 main
