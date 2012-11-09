srcs = extX.cpp	
objs = ${srcs:.cpp=.o}
target = extx

all : ${objs}
	${CC} -o ${target} ${objs} -lstdc++

clean :
	rm -f ${objs} ${target} *~
