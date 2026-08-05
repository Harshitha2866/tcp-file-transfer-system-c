# TCP File Manager with Web Dashboard

A full-stack file transfer application that combines a TCP client-server system built in C using Winsock with a modern web dashboard developed using Node.js, Express.js, HTML, CSS and JavaScript.

The project demonstrates TCP socket programming, REST API development and responsive frontend design through an intuitive interface for managing file transfers.

---

## Features

### TCP Client-Server (C + Winsock)

- TCP-based client-server communication
- List files available on the server
- Upload files from client to server
- Download files from server to client
- Command-based console interface
- Reliable file transfer over TCP sockets

### Web Dashboard

- Responsive user interface
- Upload files through the browser
- Download files with one click
- Delete files from the server
- Search files instantly
- Dashboard displaying:
  - Total Files
  - Connection Protocol
  - Server Status
- File-type icons using Font Awesome
- Modern card-based interface with gradient styling

---

## Tech Stack

### Backend
- C
- Winsock (Windows Socket API)
- Node.js
- Express.js

### Frontend
- HTML5
- CSS3
- JavaScript
- Font Awesome

### Tools
- Visual Studio Code
- MinGW GCC
- Git
- GitHub

---

## Project Architecture

```text
                   +----------------------+
                   |   Web Dashboard      |
                   | HTML • CSS • JS      |
                   +----------+-----------+
                              |
                         REST APIs
                              |
                   +----------v-----------+
                   |  Express.js Server   |
                   +----------+-----------+
                              |
                     File Management
                              |
                   +----------v-----------+
                   |    uploads Folder    |
                   +----------+-----------+
                              ^
                              |
                TCP Socket Communication
                              |
         +--------------------+--------------------+
         |                                         |
+--------v--------+                       +--------v--------+
| TCP Client (C)  | <------ TCP -------> | TCP Server (C)  |
|    Winsock      |                      |    Winsock      |
+-----------------+                      +-----------------+
```

---

## Folder Structure

```text
TCP-File-Transfer-System/
│
├── client/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── server/
│   ├── server.js
│   ├── package.json
│   └── node_modules/
│
├── uploads/
│
├── client.c
├── server.c
│
├── README.md
└── screenshots/
    ├── dashboard.png
    ├── upload.png
    └── file-manager.png
```

---

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/Harshitha2866/TCP-File-Transfer-System.git

cd TCP-File-Transfer-System
```

---

## Running the TCP Application

### Compile the Server

```bash
gcc server.c -o server -lws2_32
```

### Compile the Client

```bash
gcc client.c -o client -lws2_32
```

### Start the Server

```bash
server.exe
```

### Start the Client

```bash
client.exe
```

### Available Commands

```text
LIST
UPLOAD
DOWNLOAD
EXIT
```

---

## Running the Web Dashboard

### Navigate to the Server Folder

```bash
cd server
```

### Install Dependencies

```bash
npm install
```

### Start the Express Server

```bash
node server.js
```

The backend runs at:

```text
http://localhost:3000
```

### Launch the Frontend

Open `client/index.html` using the **Live Server** extension in Visual Studio Code.

The frontend runs at:

```text
http://127.0.0.1:5500/client/index.html
```

---

## REST API Endpoints

| Method | Endpoint | Description |
|---------|----------|-------------|
| GET | `/` | Backend status |
| GET | `/files` | Retrieve all uploaded files |
| POST | `/upload` | Upload a file |
| GET | `/download/:filename` | Download a file |
| DELETE | `/delete/:filename` | Delete a file |

---

## Dashboard Features

- File upload
- File download
- File deletion
- File search
- Dashboard statistics
- Server status indicator
- Responsive design
- Font Awesome icons
- Modern UI

---

## Sample Console Output

### LIST

```text
=========================================
Files Available
=========================================
beach.png
logo.jpg
test.txt
=========================================
```

### UPLOAD

```text
Enter Command : UPLOAD

Enter Filename : test.txt

UPLOAD SUCCESS
```

### DOWNLOAD

```text
Enter Command : DOWNLOAD

Enter Filename : logo.jpg

Download Complete.
```

---

## Screenshots

### Web Dashboard

![Dashboard](screenshots/dashboard.png)

```text
screenshots/dashboard.png
```
---

## Concepts Demonstrated

- TCP Socket Programming
- Client-Server Architecture
- Winsock Networking
- REST API Development
- File Handling
- Express.js
- Frontend Development
- Asynchronous JavaScript
- HTTP Methods (GET, POST and DELETE)

---

## Future Enhancements

- Multi-client support using multithreading
- Drag-and-drop file uploads
- Upload progress bar
- User authentication
- Secure file transfer
- File size and transfer speed indicators
- Transfer history dashboard
- Docker deployment

---

## Author

**Harshitha Minnikanti**

B.Tech – Artificial Intelligence and Data Science

Shri Vishnu Engineering College for Women

- **GitHub:** https://github.com/Harshitha2866
- **LinkedIn:** https://www.linkedin.com/in/harshitha-minnikanti-24a682354/
- **Portfolio:** https://harshitha-portfolio-beige.vercel.app/