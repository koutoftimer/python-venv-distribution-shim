#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

void execute(char *command);
void build_command(char *command, int argc, char *argv[], char *root);

int main(int argc, char *argv[])
{
	char line[100500] = { 0 };
	char root[100500] = { 0 };

	build_command(line, argc, argv, root);
	execute(line);
	return 0;
}

char *wrap_in_quotes(char *arg)
{
	return strchr(arg, ' ') ? " \"%s\"" : " %s";
}

void build_command(char *command, int argc, char *argv[], char *root)
{
	GetModuleFileName(NULL, root, 100500);
	char *shim = root;
	while (strchr(shim, '\\'))
		shim = strchr(shim, '\\') + 1;
	root[strlen(root) - strlen(shim) - 1] = 0;

	sprintf(command, "\"%s\\Scripts\\%s\"", root, shim);
	for (int i = 1; i < argc; ++i) {
		size_t len = strlen(command);
		sprintf(command + len, wrap_in_quotes(argv[i]), argv[i]);
	}
}

// see:
// https://learn.microsoft.com/ru-ru/windows/win32/procthread/creating-processes
void execute(char *command)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,	// No module name (use command line)
			   command,	// Command line
			   NULL,	// Process handle not inheritable
			   NULL,	// Thread handle not inheritable
			   FALSE,	// Set handle inheritance to FALSE
			   0,	// No creation flags
			   NULL,	// Use parent's environment block
			   NULL,	// Use parent's starting directory
			   &si,	// Pointer to STARTUPINFO structure
			   &pi)	// Pointer to PROCESS_INFORMATION structure
	    ) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);

	CloseHandle(pi.hThread);
}
