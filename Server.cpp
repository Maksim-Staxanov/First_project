#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>
using namespace std;
class Main_1
{
private:
    int s;// Основной сокет
    sockaddr_in *server_addr, *client_addr;//Адреса сервера и клиента
    int rc;//Переменная под проверку на ошибки
    int s_worker;
    string hello;
    int Shifr;// Переменная, отвечающая за выбор шифра
    char * shift;
    void Hello();// Вывод приветственного сообщения
    void WrightShifr();// Выбор шифра
    void ShiftShifr();//Сдвиговый шифр
    void ShifrReplacement();//Шифр простой замены
public:
    void Head();//Главный метод, вызывающий все остальные
};
string convert(string data, int shift);
string disconvert(string data, int shift);
string Shifr_convert(string s, string table);
string Shifr_disconvert(string s, string table);
int main()
{
    Main_1 prim;
    prim.Head();// Создание экземпляра класса, который хранит все сокеты
    return 0;
}
void Main_1::Head()
{
    s = socket(AF_INET, SOCK_STREAM, 0);// Создание основного сокета
    if (s==-1)
    {
        cout<<errno;
    }

    server_addr = new (sockaddr_in);// Привязка сокета к локальному адресу и порту
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons (44444);
    server_addr->sin_addr.s_addr = htonl ((127<<24) + 1);
    rc = bind(s,(const sockaddr*) server_addr,sizeof(sockaddr_in));
    if (rc == -1)
    {
        cout<<errno;
    }

    rc = listen(s, 10);//Ожидание соединения
    if (rc == -1)
    {
        cout<<errno;
    }

    while (1)//Обработка клиента
    {
//********************** шаг 5-6 ***************************
        client_addr = new (sockaddr_in);// Получение нового рабочего сокета для нового соединения
        socklen_t addrlen=sizeof (sockaddr_in);
        s_worker = accept(s,(struct sockaddr*)client_addr,&addrlen);
        if (s_worker == -1)
        {
            cout<<errno;
        }
// Выполнение обмена данными
        shift= new char[257];
        Hello();
        WrightShifr();//Выбор шифра
        if (Shifr==1)
        {
            ShiftShifr();//Cдвиговый шифр
        }
        else if (Shifr==2)
        {
            ShifrReplacement();//Шифр простой замены

        }

        close(s_worker);//Закрытие рабочего сокета
    }

    close(s);//Закрытие основного сокета
}
void Main_1::Hello()
{
    hello="Hello, i am server. Choose: 1 - Shift shifr \n 2- Shifr replacement";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }
}
void Main_1::WrightShifr()
{
    rc=recv(s_worker,(void*)shift,256,0);
    if (rc==-1)
    {
        cout<<errno;

    }
    Shifr=atof(shift);
}
void Main_1::ShiftShifr()
{
    hello="Write you shift";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    rc=recv(s_worker,(void*)shift,256,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    int shift1=atof(shift);

    hello="Write you string";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    char * buf = new char[257];
    rc=recv(s_worker,(void*)buf,256,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    string s1="Полученная строка: ";
    string s2="Полученный сдвиг: ";
    cerr<<s1<<buf<<endl<<s2<<shift1<<endl;
    hello="Choose encription - 1 \n Disencription - 2 ";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    rc=recv(s_worker,(void*)shift,256,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    int encr=atof(shift);

    if(encr==1)
    {
        string data=convert(buf,shift1);
        rc=send(s_worker,(const void*)data.data(),data.size(),0);
        cerr<<"Преобразованная строка: "<<data<<endl;
        if (rc==-1)
        {
            cout<<errno;
        }
    }
    else if(encr==2)
    {
        string data=disconvert(buf,shift1);
        rc=send(s_worker,(const void*)data.data(),data.size(),0);
        cerr<<"Преобразованная строка: "<<data;
        if (rc==-1)
        {
            cout<<errno;
        }
    }
}
void Main_1::ShifrReplacement()
{

    hello="Write you 26-numbers table replacement\n";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    char * table = new char[30];
    rc=recv(s_worker,(void*)table,29,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    table[rc]='\0';

    hello="Write you string\n";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    char * buf = new char[257];
    rc=recv(s_worker,(void*)buf,256,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    cout<<"Полученная строка: "<<buf<<"Полученная таблица замены: "<<table;

    hello="Choose encription - 1 \n Disencription - 2 \n";
    rc=send(s_worker,(const void*)hello.data(),hello.size(),0);
    if (rc==-1)
    {
        cout<<errno;
    }

    rc=recv(s_worker,(void*)shift,256,0);
    if (rc==-1)
    {
        cout<<errno;
    }
    int encr=atof(shift);
    if(encr==1)
    {
        string s=Shifr_convert(buf,table);
        rc=send(s_worker,(const void*)s.data(),s.size(),0);
        cerr<<"Преобразованная строка: "<<s<<endl;
        if (rc==-1)
        {
            cout<<errno;
        }

    }
    else if(encr==2)
    {
        string s=Shifr_disconvert(buf,table);
        rc=send(s_worker,(const void*)s.data(),s.size(),0);
        cerr<<"Преобразованная строка: "<<s<<endl;
        if (rc==-1)
        {
            cout<<errno;
        }
    }
}

string convert(string data, int shift)
{
    int SizeAlphabet = 26;
    shift=shift%26;
    for (int i=0; data[i]!='\0'; i++)
    {

        if ((data[i] >= 'A') && (data[i] <= 'Z'))
        {
            if (data[i] + shift > 'Z')
                data[i] = data[i]+ shift - SizeAlphabet;
            else data[i] = data[i] + shift;
        }
        else if ((data[i] >= 'a') && (data[i] <= 'z'))
        {
            if (data[i] + shift > 'z')
                data[i] = data[i] + shift - SizeAlphabet;
            else data[i] = data[i] + shift;
        }

    }
    return data;
}
string disconvert(string data, int shift)
{
    int SizeAlphabet = 26;
    shift=shift%26;
    for (int i=0; data[i]!='\0'; i++)
    {

        if ((data[i] >= 'A') && (data[i] <= 'Z'))
        {
            if (data[i] - shift < 'A')
                data[i] = data[i]- shift + SizeAlphabet;
            else data[i] = data[i] - shift;
        }
        else if ((data[i] >= 'a') && (data[i] <= 'z'))
        {
            if (data[i] - shift < 'a')
                data[i] = data[i] - shift + SizeAlphabet;
            else data[i] = data[i] - shift;
        }

    }
    return data;
}

string Shifr_convert(string s, string table)
{
    int SizeAlphabet = 25;
    for (int i = 0; s[i] != '\0'; i++)
    {

        if ((s[i] >= 'a') && (s[i] <= 'z'))
        {
            if (s[i] - 'a' <= SizeAlphabet)
                s[i]= table[s[i] - 'a'];
        }
        else if ((s[i] >= 'a') && (s[i] <= 'z'))
        {
            if (s[i] - 'a' <= SizeAlphabet)
                s[i] = table[s[i] - 'a'];
        }
    }
    return s;
}

string Shifr_disconvert(string s, string table)
{
    for (int i=0; s[i]!='\0'; i++)
    {

        for (int j = 0; j <= 25; j++)
        {
            if (table[j] == s[i])
            {
                s[i] = 'a' + j;
                goto metka1;
            }

        }
metka1:
        ;
    }
    return s;
}
