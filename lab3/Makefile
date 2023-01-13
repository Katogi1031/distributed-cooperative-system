.PHONY: run
all: predator prey run
predator:
	g++ -std=c++11 -c predator.cpp
prey:
	g++ -std=c++11 -c prey.cpp
run:
	gcc -w -o run predator.o prey.o pursuit.c -lpthread -lstdc++ -std=c99

各ブランチの制限
	feature_predator: 	predator.cのみ編集
	feature_prey:		prey.cのみ編集
