#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define BUFFER_SIZE 256
#define COM_PORT "COM7"

HANDLE hSerial;
DCB dcbSerialParams = { 0 };
COMMTIMEOUTS timeout;
BOOL bStatus;

void sendKey(char key)
{
    DWORD bytesWritten;
    WriteFile(hSerial, &key, 1, &bytesWritten, NULL);
}

void sendDataOverUART(int key, int pressDuration, int releaseDelay)
{
    // Create a buffer to hold the parameters
    unsigned char buffer[12]; // Assuming each parameter is 4 bytes (int size)

    // Populate the buffer with the parameter values
    memcpy(buffer, &key, sizeof(int));
    memcpy(buffer + sizeof(int), &pressDuration, sizeof(int));
    memcpy(buffer + (2 * sizeof(int)), &releaseDelay, sizeof(int));

    // Calculate the buffer size
    size_t bufferSize = sizeof(buffer);

    // Send the buffer over UART
    DWORD bytesWritten;
    WriteFile(hSerial, buffer, bufferSize, &bytesWritten, NULL);
}

int openSerialPort()
{
    hSerial = CreateFile("\\\\.\\COM7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open COM port %s\n", COM_PORT);
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    bStatus = GetCommState(hSerial, &dcbSerialParams);
    if (bStatus == FALSE)
    {
        printf("Error setting up UART parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    bStatus = SetCommState(hSerial, &dcbSerialParams);
    if (bStatus == FALSE)
    {
        printf("Failed to set UART serial parameters.\n");
        CloseHandle(hSerial);
        return 1;
    }
    else
    {
        printf("UART serial OK.\n");
        printf("--------------------------\n");
        printf("* Store Keypress on buffer *\n");
        printf("--------------------------\n");
    }

    COMMTIMEOUTS timeouts = { 0 };
    GetCommTimeouts(hSerial, &timeout);
    GetCommState(hSerial, &dcbSerialParams);
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Failed to set COM port timeouts\n");
        return 1;
    }
}

void closeSerialPort()
{
    CloseHandle(hSerial);
}

int main()
{
    openSerialPort();

    clock_t t1, t2, t3;
    int key;
    int pressDuration;
    int releaseDelay;

    while (1)
    {
        t1 = clock();
        key = getch();

        if (_kbhit())
        {
            t2 = clock();
            pressDuration = (int)((t2 - t1) * 1000.0 / CLOCKS_PER_SEC);
            // Send the data over UART
            sendDataOverUART(key, pressDuration, 0);
        }
        else
        {
            // Measure time after key release
            t2 = clock();
            releaseDelay = (int)((t2 - t1) * 1000.0 / CLOCKS_PER_SEC);

            // Send the data over UART
            sendDataOverUART(0, 0, releaseDelay);
        }

        // Store the data in a buffer or structure
        // For simplicity, let's just print it here
        printf("Key: %c\nPress Duration: %d ms\nRelease Delay: %d ms\n", key, pressDuration, releaseDelay);

        if (key == 'q')
        {
            // Calculate score based on time differences
            double totalTime = (double)(t2 - t1) / CLOCKS_PER_SEC;
            double score = (double)(pressDuration + releaseDelay) / totalTime;
            printf("Score: %.2f\n", score);
            break;  // Exit the loop and end the program
        }
        else if (key != 0 && key != 224)  // Key is pressed and held
        {
            // Keep updating the start time (t1) until key is released
            while (_kbhit())
            {
                key = getch();
                t1 = clock();
                // You can perform additional actions while the key is held if needed
            }
        }

        // Sleep for a while to avoid high CPU usage
        Sleep(100);
    }

    CloseHandle(hSerial);
    return 0;
}
