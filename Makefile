all:
	g++ main.cpp -lwthttp -lwt
	./a.out --docroot . --http-address 0.0.0.0 --http-port 1337
