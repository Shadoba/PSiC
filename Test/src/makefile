server:
	g++ -pthread -o testServer testServer.cpp $(HOME)/ZAJECIA/PSiC/PSiC/zeroInstall/lib/libzmq.a

run:
	./testServer

script:
	python3 TestProxy.py


.PHONY: clean

clean:
	rm testServer