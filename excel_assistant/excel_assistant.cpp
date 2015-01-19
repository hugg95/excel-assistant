// pick specified lines from such a number of csv files
// author: victor.li
// date: 2015/01/18
// version: 1.0.0

#include "stdafx.h"
#include "afx.h"
#include "locale.h"
#include "vector"

using namespace std;

// read file
void read_file(const wchar_t* file) {
    FILE* fp;
    errno_t err_no;
    err_no = _wfopen_s(&fp, file, _T("r, ccs=UNICODE"));
    printf("%d\n", err_no);
    if (0 == err_no) {
        wchar_t c;
        c = fgetwc(fp);
        while (c != WEOF) {
            printf("%S\n", c);
            //putchar(c);
            c = fgetwc(fp);
        }
        fclose(fp);
    }

}

// list files in specified directory
// and put each files into the passed vector container
void list_files(vector<CStringW> & files_arr, LPCTSTR pstr) {
    CFileFind finder;
    CStringW strWildcard(pstr);
    strWildcard += _T("\\*.*");
    BOOL bWorking = finder.FindFile(strWildcard);
    while (bWorking) {
        bWorking = finder.FindNextFile();
        if (finder.IsDots()) {
            continue;
        }
        if (finder.IsDirectory()) {
            CStringW str = finder.GetFilePath();
            //TRACE(_T("%s\n"), (LPCTSTR)str);
            list_files(files_arr, str);
        } else {
            CStringW str = finder.GetFilePath();
            files_arr.push_back(str);
        }
    }
    finder.Close();
}

void parse_file() {
}

// create a new file
void write_file(char* file) {
}

int _tmain(int argc, _TCHAR* argv[])
{
    vector<CStringW> files_arr;
    setlocale(LC_ALL, "chs");
    list_files(files_arr, _T("E:\\需求"));
    for (unsigned int i = 0; i < files_arr.size(); i++) {
        wchar_t* file_path = (LPWSTR)(LPCTSTR)files_arr.at(i);
        printf("%S\n", file_path);
        read_file(file_path);
    }
    //read_file("E:\\需求\\2000年中国出口智利.csv");

	return 0;
}

