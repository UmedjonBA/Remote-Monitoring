# Remote Monitoring System

A client-server application for monitoring work activity in an organization.

## Components

### Client (Windows)
- Written in C/C++
- Runs silently on user login
- Captures system information and screenshots
- Communicates with server via TCP/IP
- Uses WinAPI for system monitoring
- Implements screenshot capture functionality

### Server
- Written in C++
- Cross-platform application
- Manages client connections
- Handles screenshot requests
- Provides web interface for monitoring

## Project Structure

```
mon_rem/
├── Client/                 # Windows client application
│   ├── src/               # Source code
│   ├── include/           # Header files
│   └── CMakeLists.txt     # Build configuration
├── Server/                # C++ server application
│   ├── src/               # Source code
│   ├── include/           # Header files
│   └── CMakeLists.txt     # Build configuration
└── README.md              # This file
```

## Building the Project

### Client
1. Navigate to the Client directory:
   ```bash
   cd mon_rem/Client
   ```
2. Create build directory and configure:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
3. Build the project:
   ```bash
   cmake --build .
   ```
4. The executable will be created in the `build` directory

### Server
1. Navigate to the Server directory:
   ```bash
   cd mon_rem/Server
   ```
2. Create build directory and configure:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
3. Build the project:
   ```bash
   cmake --build .
   ```
4. The executable will be created in the `build` directory

## Requirements

### Client
- Windows 10 or later
- CMake 3.15 or later
- Visual Studio 2019 or later with C++ development tools
- Windows SDK

### Server
- CMake 3.15 or later
- C++17 compatible compiler
- Boost libraries
- OpenSSL

## Configuration

### Server Configuration
- Default port: 8080
- Configuration file: `Server/config.json`
- Web interface available at: `http://localhost:8080`

### Client Configuration
- Server connection settings in `Client/config.ini`
- Default server address: localhost:8080
- Screenshot capture interval configurable

## Usage

1. Start the server application:
   ```bash
   cd mon_rem/Server/build
   ./server
   ```
2. Deploy the client application to target machines
3. The client will automatically connect to the server
4. Monitor connected clients through the web interface
5. Request screenshots as needed

## Troubleshooting

### Common Issues

- Client Connection Issues:
  - Verify server is running
  - Check network connectivity
  - Ensure correct server address in config
  - Check firewall settings

- Screenshot Issues:
  - Verify client has proper permissions
  - Check disk space
  - Ensure screenshot directory exists

### Logs
- Server logs are available in the console output
- Client logs are stored in the application directory

## Development

### Building from Source
1. Clone the repository
2. Install required dependencies
3. Follow build instructions above

### Contributing
1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License
This project is licensed under the MIT License - see the LICENSE file for details. 