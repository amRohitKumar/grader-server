CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
OBJ = client.o tcpClient.o itcp.o 

itcp_DIR = ../modules/tcp
tcpClient_DIR = modules/tcpClient

client: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

client.o: client.cpp $(tcpClient_DIR)/tcpClient.h
	$(CC) $(CFLAGS) -c $< -o $@

tcpClient.o: $(tcpClient_DIR)/tcpClient.cpp $(tcpClient_DIR)/tcpClient.h $(itcp_DIR)/tcp.h
	$(CC) $(CFLAGS) -c $< -o $@

itcp.o: $(itcp_DIR)/tcp.cpp $(itcp_DIR)/tcp.h
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) client

