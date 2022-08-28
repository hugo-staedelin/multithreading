/**
 * \file MultiTaches.cpp
 * \brief Thread, Semaphore, Mutex, Event, Pipe
 * \author JL Pascal
 * \version 0.1
 * \date 11 septembre 2007
 *
 * Mise en oeuvre threads et mécanismes de synchronisation et d'echange
 *
 */


#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#define NB 80

using namespace std;
#define BS      (char)8

DWORD WINAPI tache1(LPVOID arg);
DWORD WINAPI tache2(LPVOID arg);
DWORD WINAPI tache3(LPVOID arg);
DWORD WINAPI tache4(LPVOID arg);
DWORD WINAPI tache5(LPVOID arg);

DWORD WINAPI tache7(LPVOID arg);
DWORD WINAPI tache8(LPVOID arg);
DWORD WINAPI tache9(LPVOID arg);
DWORD WINAPI tache10(LPVOID arg);
DWORD WINAPI tache11(LPVOID arg);
DWORD WINAPI tache12(LPVOID arg);
DWORD WINAPI tache13(LPVOID arg);
DWORD WINAPI tache14(LPVOID arg);

HANDLE sema1, sema2;
HANDLE SemaOpDemande, SemaOpTermine, SemaES, SemaTube1, SemaTube2;
HANDLE even1;
HANDLE mutex1, mutex2;
HANDLE tube1R, tube1W;


char caractere;

int main(int argc, char* argv[])
{
	cout << "temps reel " << endl << endl;

	int  param;   // parametre eventuel

	// Dans la suite décommenter les lignes ResumeThread   (threadx); 
	// pour démarrer les taches


	//*************************************************************
	// Exemple d'une etreinte fatale avec 2 sections critiques
	//*************************************************************
	mutex1 = CreateMutex(NULL, FALSE, L"mutex1");
	mutex2 = CreateMutex(NULL, FALSE, L"mutex2");
	param = 1;
	HANDLE thread1 = CreateThread(NULL, 0, tache1, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread1);
	HANDLE thread2 = CreateThread(NULL, 0, tache2, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread2);

	//*************************************************************
	// Exemple d'une communication entre 3 taches : simulation d'un read
	//*************************************************************
	SemaOpDemande = CreateSemaphore(NULL,0,999999,L"SemaOpDemande");
	SemaOpTermine = CreateSemaphore(NULL,0,999999,L"SemaOpTermine");
	SemaES        = CreateSemaphore(NULL,0,999999,L"SemaES");

	HANDLE thread3 = CreateThread(NULL, 0, tache3, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread3);
	HANDLE thread4 = CreateThread(NULL, 0, tache4, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread4);
	HANDLE thread5 = CreateThread(NULL, 0, tache5, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread5);

	//*************************************************************
	// Exemple d'un blocage sur 2 semaphores
	//*************************************************************
	sema1 = CreateSemaphore(NULL,0,999999,L"sema1");
	sema2 = CreateSemaphore(NULL,0,999999,L"sema2");

	HANDLE thread7 = CreateThread(NULL, 0, tache7, &param, CREATE_SUSPENDED, NULL);
	SetThreadPriority(thread7, 30);
	//ResumeThread   (thread7);

	HANDLE thread8 = CreateThread(NULL, 0, tache8, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread8);
	// ReleaseSemaphore(sema2,1,NULL); // va débloquer tache1 et tache2

	//*************************************************************
	// Exemple d'une Synchro par evenement
	//*************************************************************
	even1 = CreateEvent(NULL, TRUE, FALSE, L"even1");

	HANDLE thread9 = CreateThread(NULL, 0, tache9, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread9);
	HANDLE thread10 = CreateThread(NULL, 0, tache10, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread10);

	//*************************************************************
	// Exemple d'une communication par tube anonyme
	//*************************************************************
	CreatePipe(&tube1R, &tube1W, NULL, 0);

	SemaTube1 = CreateSemaphore(NULL,0,999999,L"SemaTube1");
	SemaTube2 = CreateSemaphore(NULL,0,999999,L"SemaTube2");

	HANDLE thread11 = CreateThread(NULL, 0, tache11, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread11);
	HANDLE thread12 = CreateThread(NULL, 0, tache12, &param, CREATE_SUSPENDED, NULL);
	//ResumeThread   (thread12);

	//*************************************************************
	// Exemple d'une communication par tube nommé
	//*************************************************************
	
	// thread13 ou thread14  peuvent être dans un autre executable
	HANDLE thread13 = CreateThread(NULL, 0, tache13, &param, CREATE_SUSPENDED, NULL);
	ResumeThread   (thread13);
	Sleep (1000); // attendre que tache13 crée le tube 
	HANDLE thread14 = CreateThread(NULL, 0, tache14, &param, CREATE_SUSPENDED, NULL);
	ResumeThread   (thread14);

	while (1);
	//cout << "une touche pour sortir " << endl << endl; cin.ignore (2);

	TerminateThread(thread1, -1);   	CloseHandle    (thread1);
	TerminateThread(thread2, -1);   	CloseHandle    (thread2);
	TerminateThread(thread3, -1);   	CloseHandle    (thread3);    
	TerminateThread(thread4, -1);       CloseHandle    (thread4);
	TerminateThread(thread5, -1);   	CloseHandle    (thread5);

	TerminateThread(thread7, -1);   	CloseHandle    (thread7);
	TerminateThread(thread8, -1);   	CloseHandle    (thread8);
	TerminateThread(thread9, -1);   	CloseHandle    (thread9);
	TerminateThread(thread10, -1);  	CloseHandle    (thread10);
	TerminateThread(thread11, -1);   	CloseHandle    (thread11);
	TerminateThread(thread12, -1);  	CloseHandle    (thread12);
	TerminateThread(thread13, -1);  	CloseHandle    (thread13);
	TerminateThread(thread14, -1);  	CloseHandle    (thread14);


	return 0;
}
DWORD WINAPI tache1(LPVOID arg) {
	//********************************
	int var = *(int*) arg;

	cout << "tache" << var << " :demarrage " << endl;

	while(1)	{
		WaitForSingleObject(mutex1, INFINITE);
		WaitForSingleObject(mutex2, INFINITE);
		cout << "tache1:dans la section critique " << endl;
		ReleaseMutex(mutex2);
		ReleaseMutex(mutex1);
	}
	ExitThread(0);
}
DWORD WINAPI tache2(LPVOID arg) {
	//********************************

	cout << "tache2:demarrage " << endl;

	while(1)	{
		WaitForSingleObject(mutex2, INFINITE);
		WaitForSingleObject(mutex1, INFINITE);
		cout << "tache2:dans la section critique " << endl;
		ReleaseMutex(mutex1);
		ReleaseMutex(mutex2);
	}
	ExitThread(0);
}

DWORD WINAPI tache3(LPVOID arg) {
	//********************************

	cout << "tache3:demarrage " << endl;

	while(1)	{
		ReleaseSemaphore(SemaOpDemande,1,NULL);     // V
		cout << "tache3:demande de lecture " << endl;
		WaitForSingleObject(SemaES,INFINITE);     // P
		cout << "tache3:lecture effectuee = " << caractere << endl;
	}
	ExitThread(0);
}
DWORD WINAPI tache4(LPVOID arg) {
	//********************************

	cout << "tache4:demarrage " << endl;

	while(1)	{
		WaitForSingleObject(SemaOpDemande,INFINITE);     // P
		cout << "tache4:recut demande de lecture " << endl;
		WaitForSingleObject(SemaOpTermine,INFINITE);     // P
		cout << "tache4:recut le caractère clavier " << endl;
		ReleaseSemaphore(SemaES,1,NULL);     // V
	}
	ExitThread(0);
}
DWORD WINAPI tache5(LPVOID arg) {
	//********************************

	cout << "tache5:demarrage " << endl;

	while(1)	{
		caractere= _getch ();
		ReleaseSemaphore(SemaOpTermine,1,NULL);     // V
		cout << "tache5:debloque tache4 " << endl;
	}
	ExitThread(0);
}

DWORD WINAPI tache7(LPVOID arg) {
	//********************************

	cout << "tache7:demarrage " << endl;

	while(1)	{
		WaitForSingleObject(sema1,INFINITE);
		Sleep(50);
		cout << "->"<<BS;
		ReleaseSemaphore(sema2,1,NULL);

	}
	ExitThread(0);
}
DWORD WINAPI tache8(LPVOID arg) {
	//********************************	
	cout << "tache8:Demarrage" << endl;
	//HANDLE sema1 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"sema1");
	while(1)	{
		WaitForSingleObject(sema2,INFINITE);
		Sleep(50);
		cout << "+>"<<BS;
		ReleaseSemaphore(sema1,1,NULL);
	}
	ExitThread(0);
}
DWORD WINAPI tache9(LPVOID arg) {
	//********************************

	cout << "tache9:Demarrage" << endl;
	//	HANDLE even = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"even1");

	while(1)	{
		cout << "tache9:attente evenement" << endl;

		WaitForSingleObject(even1, INFINITE); 
		cout << "tache9:evenement recu" << endl;
		ResetEvent(even1);
	}
	ExitThread(0);
}
DWORD WINAPI tache10(LPVOID arg) {
	//********************************

	cout << "tache10:Demarrage" << endl;
	//HANDLE even  = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"even1");

	while(1)	{
		cout << "tache10: signale la tache9" << endl;
		SetEvent(even1); 
		Sleep (5000);
	}

	ExitThread(0);
}
DWORD WINAPI tache11(LPVOID arg) {
	//********************************

	cout << "tache11:Demarrage" << endl;
	DWORD nb;
	char buffer[NB];

	while(1)	{
		cin.getline(buffer, NB);
		WriteFile(tube1W, buffer, NB, &nb, NULL);
		ReleaseSemaphore(SemaTube1,1,NULL);
		WaitForSingleObject(SemaTube2,INFINITE);
		ReadFile(tube1R, buffer, NB, &nb, NULL);
		cout << "tache11: " << buffer << endl;
	}
	ExitThread(0);
}
DWORD WINAPI tache12(LPVOID arg) {
	//********************************

	cout << "tache12:Demarrage" << endl;
	DWORD nb;
	char buffer[NB];

	while(1)	{
		WaitForSingleObject(SemaTube1,INFINITE);
		ReadFile(tube1R, buffer, NB, &nb, NULL);
		cout << "tache12: " << buffer << endl;
		strcpy (buffer,"OK");
		WriteFile(tube1W, buffer, NB, &nb, NULL);
		ReleaseSemaphore(SemaTube2,1,NULL);
	}
	ExitThread(0);
}
DWORD WINAPI tache13(LPVOID arg) {
	//********************************

	cout << "tache13:Demarrage" << endl;
	HANDLE tube2 = CreateNamedPipe(L"\\\\.\\pipe\\testpipe", 	// Name
		PIPE_ACCESS_DUPLEX | WRITE_DAC, // OpenMode
		PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE | PIPE_WAIT, // PipeMode
		2, // MaxInstances
		65536, // OutBufferSize
		65536, // InBuffersize
		2000, // TimeOut
		NULL); // Security
	if (tube2 == INVALID_HANDLE_VALUE)		cout << "error ";
	ConnectNamedPipe(tube2, NULL);
	DWORD nb;
	char buffer[NB];

	while(1)	{
		ReadFile(tube2, buffer, NB, &nb, NULL);
		cout << "tache13 : recoit " << buffer << endl;
		
	}
	DisconnectNamedPipe(tube2);
	ExitThread(0);
}

DWORD WINAPI tache14(LPVOID arg) {
	//********************************

	cout << "tache14:Demarrage" << endl;
	HANDLE  tube2      = CreateFile(L"\\\\.\\pipe\\testpipe",
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_WRITE_THROUGH,NULL);

	DWORD nb;
	char buffer[NB];

	while(1)	{
		cin.getline(buffer, NB);
		ConnectNamedPipe(tube2, NULL);
		WriteFile(tube2, buffer, NB, &nb, NULL);
		DisconnectNamedPipe(tube2);
	}
	ExitThread(0);
}

