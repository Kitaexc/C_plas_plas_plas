#define WIN32_LEAN_AND_MEAN // Определение макроса для исключения редко используемых компонентов Windows из Windows.h, что уменьшает размер скомпилированного кода и ускоряет сборку.

#include <Windows.h> // Подключение заголовочного файла Windows.h, который содержит объявления для большинства функций Windows API.
#include <iostream> // Подключение заголовочного файла iostream для работы с потоками ввода-вывода.
#include <WinSock2.h> // Подключение заголовочного файла WinSock2.h для использования сокетов Windows.
#include <WS2tcpip.h> // Подключение заголовочного файла WS2tcpip.h для дополнительных функций TCP/IP в Windows.

using namespace std; // Использование пространства имен std для упрощения кода.

int main() { // Начало главной функции программы.
    WSADATA wsaData; // Объявление структуры WSADATA для хранения информации о реализации Windows Sockets.
    ADDRINFO hints; // Объявление структуры ADDRINFO для хранения информации, необходимой для getaddrinfo.
    ADDRINFO* addrResult; // Объявление указателя на структуру ADDRINFO для хранения результатов функции getaddrinfo.
    SOCKET ConnectSocket = INVALID_SOCKET; // Объявление сокета и инициализация его значением INVALID_SOCKET.
    char recvBuffer[512]; // Объявление буфера для приема данных размером 512 байт.

    const char* sendBuffer1 = "Hello from client 1"; // Объявление и инициализация первой строки сообщения для отправки серверу.
    const char* sendBuffer2 = "Hello from client 2"; // Объявление и инициализация второй строки сообщения для отправки серверу.

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Инициализация библиотеки Winsock версии 2.2 и сохранение результата.
    if (result != 0) { // Проверка успешности инициализации Winsock.
        cout << "WSAStartup failed with result: " << result << endl; // Вывод сообщения об ошибке инициализации.
        return 1; // Завершение программы с кодом ошибки.
    }

    ZeroMemory(&hints, sizeof(hints)); // Обнуление памяти, выделенной под структуру hints.
    hints.ai_family = AF_INET; // Указание семейства адресов IPv4.
    hints.ai_socktype = SOCK_STREAM; // Указание типа сокета - потоковый сокет (TCP).
    hints.ai_protocol = IPPROTO_TCP; // Указание протокола TCP.

    result = getaddrinfo("localhost", "666", &hints, &addrResult); // Получение адресной информации для подключения к localhost на порт 666.
    if (result != 0) { // Проверка успешности выполнения getaddrinfo.
        cout << "getaddrinfo failed with error: " << result << endl; // Вывод сообщения об ошибке getaddrinfo.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }

    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // Создание сокета с указанными параметрами.
    if (ConnectSocket == INVALID_SOCKET) { // Проверка успешности создания сокета.
        cout << "Socket creation failed" << endl; // Вывод сообщения об ошибке создания сокета.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }

    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); // Подключение к серверу по указанному адресу.
    if (result == SOCKET_ERROR) { // Проверка успешности подключения.
        cout << "Unable to connect to server" << endl; // Вывод сообщения об ошибке подключения.
        closesocket(ConnectSocket); // Закрытие сокета.
        ConnectSocket = INVALID_SOCKET; // Сброс значения сокета.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }

    result = send(ConnectSocket, sendBuffer1, (int)strlen(sendBuffer1), 0); // Отправка первого сообщения серверу.
    if (result == SOCKET_ERROR) { // Проверка успешности отправки.
        cout << "Send failed, error: " << result << endl; // Вывод сообщения об ошибке отправки.
        closesocket(ConnectSocket); // Закрытие сокета.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }
    cout << "Sent: " << result << " bytes" << endl; // Вывод информации об отправленных данных.

    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0); // Отправка второго сообщения серверу.
    if (result == SOCKET_ERROR) { // Проверка успешности отправки.
        cout << "Send failed, error: " << result << endl; // Вывод сообщения об ошибке отправки.
        closesocket(ConnectSocket); // Закрытие сокета.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }
    cout << "Sent: " << result << " bytes" << endl; // Вывод информации об отправленных данных.

    result = shutdown(ConnectSocket, SD_SEND); // Завершение передачи данных по сокету.
    if (result == SOCKET_ERROR) { // Проверка успешности завершения передачи.
        cout << "Shutdown failed, error: " << result << endl; // Вывод сообщения об ошибке завершения передачи.
        closesocket(ConnectSocket); // Закрытие сокета.
        freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
        WSACleanup(); // Завершение работы с библиотекой Winsock.
        return 1; // Завершение программы с кодом ошибки.
    }

    do { // Цикл для приема данных от сервера.
        ZeroMemory(recvBuffer, 512); // Обнуление буфера для приема данных.
        result = recv(ConnectSocket, recvBuffer, 512, 0); // Прием данных от сервера.
        if (result > 0) { // Проверка успешности приема данных.
            cout << "Received " << result << " bytes" << endl; // Вывод информации о количестве принятых байт.
            cout << "Received data: " << recvBuffer << endl; // Вывод принятых данных.
        }
        else if (result == 0) { // Проверка завершения соединения.
            cout << "Connection closed" << endl; // Вывод сообщения о закрытии соединения.
        }
        else { // Обработка ошибок при приеме данных.
            cout << "Recv failed, error: " << WSAGetLastError() << endl; // Вывод сообщения об ошибке приема.
        }
    } while (result > 0); // Продолжение цикла, пока принимаются данные.

    closesocket(ConnectSocket); // Закрытие сокета.
    freeaddrinfo(addrResult); // Освобождение памяти, выделенной для addrResult.
    WSACleanup(); // Завершение работы с библиотекой Winsock.
    return 0; // Завершение программы с кодом 0 (успешное завершение).
}
