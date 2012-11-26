srcs = mp5.cpp ext2analysis.cpp
objs = ${srcs:.cpp=.o}
target = extx

all : ${objs}
	${CC} -o ${target} ${objs} -lstdc++ -lm

clean :
	rm -f ${objs} ${target} *~
