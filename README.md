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
- Windows application
- Manages client connections
- Handles screenshot requests
- Provides GUI interface for monitoring

## Project Structure

```
mon_rem/
├── Client/                 # Windows client application
│   ├── *.cpp              # Source files
│   ├── *.h                # Header files
│   └── Client.vcxproj     # Visual Studio project file
├── Server/                # C++ server application
│   ├── *.cpp              # Source files
│   ├── *.h                # Header files
│   └── Server.vcxproj     # Visual Studio project file
├── Common/                # Shared code
└── RemoteMonitoring.sln   # Visual Studio solution file
```

## Building the Project

### Using Visual Studio
1. Open `RemoteMonitoring.sln` in Visual Studio 2019 or later
2. Select the desired configuration (Debug/Release)
3. Build the solution (F7 or Build -> Build Solution)
4. The executables will be created in the respective Debug/Release directories

### Requirements

### Client
- Windows 10 or later
- Visual Studio 2019 or later with C++ development tools
- Windows SDK

### Server
- Windows 10 or later
- Visual Studio 2019 or later with C++ development tools
- Windows SDK

## Configuration

### Server Configuration
- Default port: 8080
- Configuration can be modified through the GUI interface

### Client Configuration
- Server connection settings in `Client/config.ini`
- Default server address: localhost:8080
- Screenshot capture interval configurable

## Usage

1. Start the server application:
   - Run `Server.exe` from the Server/Debug or Server/Release directory
2. Deploy the client application to target machines
3. The client will automatically connect to the server
4. Monitor connected clients through the server interface
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
2. Open the solution in Visual Studio
3. Build the project

### Contributing
1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License
This project is licensed under the MIT License - see the LICENSE file for details. 