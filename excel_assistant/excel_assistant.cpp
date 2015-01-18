// pick specified lines from such a number of csv files
// author: victor.li
// date: 2015/01/18
// version: 1.0.0

#include "stdafx.h"
#include "string"
#include "afx.h"
#include "locale.h"
#include <vector>
using namespace std;

void read_file(char* file) {
    FILE* fp;
    errno_t err_no;
    err_no = fopen_s(&fp, file, "r");
    if (0 == err_no) {
        char c;
        c = fgetc(fp);
        while (c != EOF) {
            putchar(c);
            c = fgetc(fp);
        }
        fclose(fp);
    }
    //printf("%p\n", fp);

}

// list files in specified directory
void list_files(vector<CStringW> files_arr, LPCTSTR pstr) {
    printf("%d\n", files_arr.size());
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
    //read_file();
    //char* test = "E:\\需求";
    //CString pstr = (CString)test;
    //LPCTSTR pstr = test;
    list_files(files_arr, _T("E:\\需求"));
    //vector<CStringW>::iterator it = files_arr.begin();
    //for (; it < files_arr.end; it++) {
    //    printf("%s\n", files_arr.at(it));
    //}
    //printf("%d\n", files_arr.size());
    for (int i = 0; i < files_arr.size(); i++) {
        //printf("%s\n", files_arr.at(i));
    }

	return 0;
}

