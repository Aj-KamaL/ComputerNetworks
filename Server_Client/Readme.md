1. A client server program using sockets.
2. The server must be able to accept and maintain connection with multiple clients. Each client will have a unique identity
3. The client "c" will send a message "s" and the server will broadcast the message " received from c: s" to all the clients, including the sender.
4. The clients can come and go in between i.e. they can cancel their connection by informing the server and also abruptly.
5. The server can also go offline after informing the clients as well as abruptly.
6. The programming language is C and Pthread for multi-threading.



gcc -g -o Server -pthread 16076Server.c on any Machine.
gcc -g -o Client -pthread 16076Client.c on any Machine.

Run Server and Client on any machine.
Pass the IP of device on which Server object file is run as argument to the Client object file.
./Server
./Client <IP of device on which server is run>
