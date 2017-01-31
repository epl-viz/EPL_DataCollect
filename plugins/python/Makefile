
api:
	python3 setup.py build_ext --inplace
	python3 TestAPI.py	#WITH TEST

plugin:
	python3 setupPlugin.py build_ext --inplace
	python3 TestPlugin.py
	@echo C++ Interaction
	g++ main.cpp Plugin.cpp -I/usr/include/python3.5 -lpython3.5m
	./a.out

all: api plugin
	@echo complete
	
