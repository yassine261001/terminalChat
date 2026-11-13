# terminalChat

A peer-to-peer chat over TCP in your terminal, written in C with POSIX sockets for fun.

Built so you and your friend can message each other quietly over the terminal, without using WhatsApp or another messenger. One person listens, the other connects, and both can type at any time.

´´´
> main menu: connect
> Enter an IP address: 127.0.0.1
> Chat started. Type /quit to leave.
Friend: hey, did you get the last slide?
You: ye, sending it after class
> _
´´´

### Features
- Direct peer-to-peer connection over TCP, no server or account needed
- Real-time two-way messaging: poll() watches the keyboard and the socket at the same time, so incoming messages show up while you're waiting to type
- A simple command menu (listen, connect, help, exit)
- Detects when your friend disconnects and returns you to the menu
- IPv4 and IPv6 support via getaddrinfo()

### Building
You need a C compiler on Linux, macOS or WSL
´´´
gcc main.c chat.c -o chat
´´´
### Usage
Both users run ./chat

#### Person A listens:
´´´
> main menu: listen
You are listening on port 8080...
´´´
#### Person B connects:
´´´
> main menu: connect
> Enter an IP address: 192.168.0.42
´´´
To find your IP address, run ipconfig on Windows or ifconfig on macOS
Try it on a single machine in two separate terminals and connect to 127.0.0.1

### How it works
Once the connection is established, both sides run the same ´´´chat_loop()´´´. It calls ´´´poll()´´´ on two file descriptors: standard input (the keyboard) and the connected socket. ´´´poll()´´´ sleeps until one of them has data:
- Keyboard ready: read the line with ´´´fgets()´´´ and ´´´send()´´´ it to the peer
- Socket ready: ´´´recv() the message and print it. A return value of ´´´0´´´ means the peer closed the connection

The chat is fully asynchronous on a single thread.

### Structure
´´´
main.c  command menu and input loop
chat.c  network: listen, connect, chat loop
chat.h  shared function declarations and command enum
´´´

### Limitations
- Currently no encryption. Messages are sent as plain text, so anyone on the same network could read them with a packet sniffer.
- One-to-one only, no group chat
- Some university or public Wi-Fi networks may block direct connections between devices ("client isolation"). Try a phone hotspot.
- Small quirk: A message arriving while you're typing will clear your unfinished text from the screen, but still gets sent correctly after pressing Enter.

### Roadmap / Possible features in the future
- Choose port instead of fixed "8080"
- Individual usernames
- Colored input
- TLS encryption with OpenSSL
- Group chat with a central server
---
