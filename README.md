##WinAPI_sendNreadTonesFromToEEPROM

This project demonstrates a WinAPI program that sends keypress data over UART and reads data from EEPROM. It uses the Windows API functions to establish a serial connection, send keypress information, and receive data from an EEPROM device.

Getting Started
To get started with this project, follow the steps below:

Clone the repository:

bash
Copy code
git clone <repository_url>
Compile the project using a compatible C compiler.

Connect the target device that communicates over UART using the COM7 port. Make sure the device is properly connected and powered on.

Run the compiled executable file.

Usage
The program waits for user keypresses and sends the corresponding key and duration information over the COM7 port. It also receives data from the EEPROM and stores it in a buffer or structure.

When the program is running, follow these instructions:

Press any key to send the keypress information over UART. The program will measure the press duration and send the data.

Release the key to calculate the release delay and send the data.

The program will display the key, press duration, release delay, and other information on the console.

Press 'q' to exit the program. The program will calculate the score based on the time differences and display it.

Dependencies
This project relies on the following dependencies:

Windows API
windows.h
stdio.h
conio.h
time.h
Make sure these dependencies are available and properly configured in your development environment.

Contributing
Contributions to this project are welcome. Feel free to submit bug reports, feature requests, or pull requests to improve the project.

License
This project is licensed under the MIT License. You are free to modify and distribute the code according to the terms of the license.

Acknowledgements
This project is inspired by the need for UART communication and EEPROM data handling in Windows environments. Special thanks to the contributors and the Windows API community for their valuable resources and support.

For more information on the project or any questions, please refer to the repository or contact the project owner.