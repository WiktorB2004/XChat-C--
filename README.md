# Overview

This project is a playground that, through prototyping, allows me to learn the functions of the libraries/languages used, and then iteratively build new functionalities. This way of development is based on learning from mistakes, which manifests itself in the implementation of the next iteration, if done correctly, it means that the method used to create a feature is the result of trying many solutions, which ensures that it is properly chosen for the project. In addition it also allows for a quick process of learning the attribues of the languages or libraries that I have never used before.

# XChat: Real-time Chat Application

XChat is a real-time message sending application built in C++ using the Qt framework. It allows users to exchange messages in real-time via a graphical user interface. The project consists of two main modules: the server and the client.

App Documentation: [Link](https://wiktorb2004.github.io/XChat-Cpp/)

## Features

- Real-time message exchange
- Creating Group chats
- Adding contacts
- Graphical user interface built with Qt
- Server-client architecture
- Cross-platform support
- User Authentication

## Requirements

- C++ compiler (supporting C++11 or higher).
- CMake (3.10 or higher).
- Qt framework (5.0 or higher).
- Docker (if running with Docker).

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/WiktorB2004/XChat-Cpp.git
   ```

2. Navigate to the project directory:

   ```bash
   cd XChat-Cpp
   ```

3. Build the server and client modules:

   ```bash
   cmake -Bbuild -H.
   cmake --build build
   ```

## Usage

### Running Locally

1. Start the server (If you want to be a host):

   ```bash
   ./server/bin/server/
   ```

2. Start the client (If you want to just chat):

   ```bash
   ./client/bin/client
   ```

3. Use the graphical user interface to exchange messages in real-time.

### Running with Docker

1. Pull the Docker image:

   ```bash
   docker pull ghcr.io/wiktorb2004/xchat-cpp/server:master
   ```

2. Build the Docker container:

   ```bash

   ```

3. Run the Docker container:

   ```bash

   ```

## Authentication

### Server Authentication

- The server verifies user credentials (username and password) before allowing access to the chat.
- User credentials are securely hashed and stored in the server's database.

### Client Authentication

- Users must provide their credentials (username and password) to log in to the chat application.
- Upon successful authentication, users gain access to the chat interface.
- Incorrect credentials result in authentication failure, with appropriate error messages displayed to the user.

For more details on authentication, refer to the server and client source code.

## Directory Structure

```
XChat/
├── .github/              # GitHub folder
│   └── workflows/        # Workflows folder
├── server/               # Server module
│   ├── src/              # Source files for server
│   ├── include/          # Header files for server
│   ├── tests/            # Test files for server
│   ├── CMakeLists.txt    # CMake configuration for server
│   └── Dockerfile        # Dockerfile for server module
│
├── client/               # Client module
│   ├── src/              # Source files for client
│   ├── include/          # Header files for client
│   ├── tests/            # Test files for client
│   ├── CMakeLists.txt    # CMake configuration for client
│   └── Dockerfile        # Dockerfile for client module
│
├── lib/                  # Needed libraries
├── docs/                 # Documentation of the project - generated after running doxygen
├── assets/               # Qt Assets
├── README.md             # Project README
├── LICENSE               # Project LICENSE
└── CMakeLists.txt        # Main CMakeLists.txt in root directory
```

## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your proposed changes.

## License

This project is licensed under the [Apache License](LICENSE).

### Attributions

<a href="https://www.flaticon.com/free-icons/send" title="send icons">Send icons created by Freepik - Flaticon</a> <br>
<a href="https://www.flaticon.com/free-icons/logout" title="logout icons">Logout icons created by Uniconlabs - Flaticon</a> <br>
<a href="https://www.flaticon.com/free-icons/close" title="close icons">Close icons created by Ilham Fitrotul Hayat - Flaticon</a> <br>
Photo by <a href="https://unsplash.com/@tbelabuseridze?utm_content=creditCopyText&utm_medium=referral&utm_source=unsplash">Tbel Abuseridze</a> on <a href="https://unsplash.com/photos/mountain-ranges-9OpS7JDaTBw?utm_content=creditCopyText&utm_medium=referral&utm_source=unsplash">Unsplash</a> <br>
<a href="https://www.flaticon.com/free-icons/new-message" title="new message icons">New message icons created by artcus - Flaticon</a> <br>
<a href="https://www.flaticon.com/free-icons/add" title="add icons">Add icons created by Freepik - Flaticon</a> <br>
<a href="https://www.flaticon.com/free-icons/tick" title="tick icons">Tick icons created by Freepik - Flaticon</a> <br>
<a href="https://www.flaticon.com/free-icons/cross-platform" title="cross platform icons">Cross platform icons created by Freepik - Flaticon</a> <br>
Photo by <a href="https://unsplash.com/@alexbemore?utm_content=creditCopyText&utm_medium=referral&utm_source=unsplash">Alexander Shatov</a> on <a href="https://unsplash.com/photos/a-black-square-button-with-a-white-x-on-it-d4_aCS3jsQ0?utm_content=creditCopyText&utm_medium=referral&utm_source=unsplash">Unsplash</a>

<hr>

Feel free to customize this Markdown file further according to your project's specific features, requirements, and preferences. Ensure that you update any installation, usage, or authentication instructions as needed based on the actual implementation of the XChat application.
