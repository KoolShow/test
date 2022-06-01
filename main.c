int main() {
	DWORD pid = 0;
        scanf("%d",pid);
	
	HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!hprocess) {
		std::cout << "can not get handle" << std::endl;
		return 1;
	}
	SIZE_T PathSize = (strlen(DLLPath) + 1) * sizeof(TCHAR);
	LPVOID StartAddress = VirtualAllocEx(hprocess, NULL, PathSize, MEM_COMMIT, PAGE_READWRITE);
	if (!StartAddress) {
		std::cout << "开辟内存失败" << std::endl;
		return 1;
	}
	if (!WriteProcessMemory(hprocess, StartAddress, DLLPath, PathSize, NULL)) {
		std::cout << "无法写入DLL路径" << std::endl;
		return 1;
	}
	PTHREAD_START_ROUTINE pfnStartAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
	if (!pfnStartAddress) {
		std::cout << "无法获取函数地址" << std::endl;
		return 1;
	}
	HANDLE hThread = CreateRemoteThreadEx(hprocess, NULL, NULL, pfnStartAddress, StartAddress, NULL, NULL, NULL);
	if (!hThread) {
		std::cout << "创建线程失败" << std::endl;
		return 1;
	}
	//WaitForSingleObject(hThread, INFINITE);//等待DLL结束
	std::cout << "注入成功！\n";
	CloseHandle(hThread);
	CloseHandle(hprocess);
	return 0;
}
