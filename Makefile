CC=g++
CPPFLAGS= -g -Wall -D_REENTRANT -Wno-deprecated -std=c++11

TARGET=main
OBJS=main.o command.o data_mapper.o json_manager.o mysql_conn.o server.o util.o query.o
SRC=$(OBJS:.o=.c)

LIBS= -lmysqlclient -L/usr/lib64/mysql -I/usr/include/mysql
PTHREAD=-lpthread

CLIENT_TARGET=dummy_client
CLIENT_OBJS=dummy_client.o
CLIENT_SRC=$(CLIENT_OBJS:.o=.c)

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${PTHREAD} ${LIBS} ${CPPFLAGS}

$(CLIENT): $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_TAREGT) $(CLIENT_OBJS)

clean:
	rm -f $(OBJS) $(TARGET) $(CLIENT_TARGET)
