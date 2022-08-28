#include <iostream>
#include <windows.h>
using namespace std;

DWORD WINAPI tacheP(LPVOID arg);
DWORD WINAPI tacheV(LPVOID arg);

HANDLE mutex;
HANDLE sema;

int main()	// tâche principale
{
	cout<<"Synchronisation par semaphore"<<endl<<endl;
	mutex = CreateMutex(NULL, FALSE, "mutex");
	sema = CreateSemaphore(NULL,0,999999,"sema");

	HANDLE thread1 = CreateThread(NULL, 0, tacheP, NULL, 0, NULL);
	HANDLE thread2 = CreateThread(NULL, 0, tacheV, NULL, 0, NULL);

	while(1);
	return 0;
}
DWORD WINAPI tacheP(LPVOID arg) // tâche P 
{
	cout<<"tacheP:demarrage"<<endl;
	while(true)
	{
		Sleep(5000); // Pause de 5s
		WaitForSingleObject(mutex,INFINITE); // p(mutex)
		cout<<"Tache P debloque"<<endl;
		ReleaseSemaphore(sema,1,NULL); // v(sema)
		ReleaseMutex(mutex); // v(mutex)
	}
}
DWORD WINAPI tacheV(LPVOID arg) // tâche V
{
	cout<<"tacheV:demarrage"<<endl;
	while(true)
	{
		
		WaitForSingleObject(sema,INFINITE); // p(sema)
		WaitForSingleObject(mutex,INFINITE); // p(mutex)
		cout<<"Tache V est debloquee"<<endl;
		ReleaseMutex(mutex); // v(mutex)
	}
}