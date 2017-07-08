#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>
using namespace std;
string convert(string data, int shift)
{
int SizeAlphabet = 26;
for (int i=0; data[i]!='\0';i++)
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
int main()
{
//********************** шаг 1 *****************************
 int s = socket(AF_INET, SOCK_STREAM, 0);
 if (s==-1) {cout<<errno; return 0;}
//********************** шаг 2 *****************************
 sockaddr_in *server_addr, *client_addr;
 server_addr = new (sockaddr_in);
 server_addr->sin_family = AF_INET;
 server_addr->sin_port = htons (44444);
 server_addr->sin_addr.s_addr = htonl ((127<<24) + 1);
 int rc = bind(s,(const sockaddr*) server_addr,sizeof(sockaddr_in));
 if (rc == -1) {cout<<errno; return 0;}
//********************** шаг 3 *****************************
 rc = listen(s, 10);
 if (rc == -1) {cout<<errno; return 0;}
//********************** шаг 4 *****************************
 while (1)
 {
//********************** шаг 5-6 ***************************
 client_addr = new (sockaddr_in);
 socklen_t addrlen=sizeof (sockaddr_in);
 int s_worker = accept(s,(struct sockaddr*)client_addr,&addrlen);
 if (s_worker == -1) {cout<<errno; return 0;}
//********************** шаг 7 *****************************
 char * buf = new char[257];
 rc=recv(s_worker,(void*)buf,256,0);
 if (rc==-1) {cout<<errno; return 0;}
 buf[rc]='\0';
 string data=convert(buf,13);
 rc=send(s_worker,(const void*)data.data(),data.size(),0);
 if (rc==-1) {cout<<errno; return 0;}

//********************** шаг 8 *****************************
 close(s_worker);
 }
//********************** шаг 9 *****************************
 close(s);
 return 0;
}
