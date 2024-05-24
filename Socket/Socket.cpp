// Socket.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <WS2tcpip.h>
#include <WinSock2.h>



using namespace std;
int main()
{   
/////////////////////////
// Объявление переменных
    WSADATA wsaData; //Структура, которая хранит в себе сведенья о том как работать с сокетами
    ADDRINFOA hints; //Структура, хранящая в себе сведенья об узле
    ADDRINFO* addrResult; //Структура, хранящая в себе сведенья об узле
    char recvBuffer[512]; //Массив для хранения входящих данных
    SOCKET ConnectSocket = INVALID_SOCKET; 
    //В качестве базового значения для инициализации сокета указывается INVALID_SOCKET,
    //который в сути замещает -1
    const char* sendBuffer = "Hello from client"; //Сообщение
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //Ининициализация Winsock, первым парметром указывается версия Winsock, вторым то куда будут записаны инструкции исходя из версии
    // WSAStartup - позволяет получить инструкции по работе с сокетами
/////////////////////////
    if (result != 0) {
        cout << "WSAStartup failed" << endl;
        return 1;
    }
    
    ZeroMemory(&hints, sizeof(hints));
    //ZeroMemory чистит память в диапазоне бит начиная (1 параметр) на доину (2 парметр)
    hints.ai_family = AF_INET;//Указание семейства IP как IPv4(AF_INET)
    hints.ai_socktype = SOCK_STREAM;//Указание типа сокета 
    hints.ai_protocol = IPPROTO_TCP;//Задаение протокола передачи данных

    result = getaddrinfo("127.0.0.1", "6464", &hints, &addrResult);//Дополнение ранее переданных иструкций сведеньями о сокете
    if (result != 0) {
        cout << "WSAStartup failed" << endl;
        WSACleanup();
        return 1;
    }

    ConnectSocket = socket(addrResult -> ai_family, addrResult->ai_socktype, addrResult -> ai_protocol);//Создание сокета с учетом ранее записанных в структуры параметров (сокета к которому подключаемся)
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Socket creation with error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = connect(ConnectSocket, addrResult->ai_addr, addrResult->ai_addrlen);//Осуществляем подключение согласно сокету
    if (result == INVALID_SOCKET) {
        cout << "Socket creation with error" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);//Отправляем сообщение на (куда, что, длинна )
    if (result == SOCKET_ERROR) {
        cout << "Send failed error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "Send " << result << endl;

    result = shutdown(ConnectSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        cout << "Shutdown failed error" << result << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    
    

    do {
        ZeroMemory(recvBuffer, 512, 0);
        result = recv(ConnectSocket, recvBuffer, 512, 0);
        if (result > 0) {
            cout << "Recieved " << result << " bytes " << endl;
            cout << "Recieved data " << recvBuffer << endl;

        }
        else if (result == 0) 
            cout << "Connection closed " << endl;
        
        else 
            cout << "Recieved with Error" << endl;
        
    }while (result > 0);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
