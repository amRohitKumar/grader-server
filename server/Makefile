CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
OBJ = grader.o connection.o threadPool.o tcpServer.o server.o

graders_DIR = modules/grader
connection_DIR = modules/connection
threadPool_DIR = modules/threadPool
tcpServer_DIR = modules/tcpServer

server: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@	

server.o: server.cpp $(graders_DIR)/grader.h $(tcpServer_DIR)/tcpServer.h $(threadPool_DIR)/threadPool.h
	$(CC) $(CFLAGS) -c $< -o $@

connection.o: $(connection_DIR)/connection.cpp $(connection_DIR)/connection.h
	$(CC) $(CFLAGS) -c $< -o $@

grader.o: $(graders_DIR)/grader.cpp $(graders_DIR)/grader.h $(connection_DIR)/connection.h
	$(CC) $(CFLAGS) -c $< -o $@

threadPool.o: $(threadPool_DIR)/threadPool.cpp $(threadPool_DIR)/threadPool.h
	$(CC) $(CFLAGS) -c $< -o $@

tcpServer.o: $(tcpServer_DIR)/tcpServer.cpp $(tcpServer_DIR)/tcpServer.h $(threadPool_DIR)/threadPool.h $(graders_DIR)/grader.h $(connection_DIR)/connection.h
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) server