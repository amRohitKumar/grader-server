CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
OBJ = client.o tcpClient.o fileTransfer.o 

tcpClient_DIR = modules/tcpClient
fileTransfer_DIR = modules/fileTransfer

client: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

client.o: client.cpp $(tcpClient_DIR)/tcpClient.h $(fileTransfer_DIR)/fileTransfer.h
	$(CC) $(CFLAGS) -c $< -o $@

tcpClient.o: $(tcpClient_DIR)/tcpClient.cpp $(tcpClient_DIR)/tcpClient.h
	$(CC) $(CFLAGS) -c $< -o $@

fileTransfer.o: $(fileTransfer_DIR)/fileTransfer.cpp $(fileTransfer_DIR)/fileTransfer.h $(tcpClient_DIR)/tcpClient.h
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) client

