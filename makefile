srcs = 	fs_ext2.cpp \
	fs_ext4.cpp \
	ext2p.cpp \
	mp5.cpp
objs = ${srcs:.cpp=.o}
target = extx

all : ${objs}
	${CC} -o ${target} ${objs} -lstdc++ -lm

clean :
	rm -f ${objs} ${target} *~
