// pick specified lines from such a number of csv files
// author: victor.li
// date: 2015/01/18
// version: 1.0.0

#include "stdafx.h"
#include "afx.h"
#include "locale.h"
#include "vector"
#include "target.h"

using namespace std;

// read file
void read_file(LPCWSTR file);

// list files in specified directory
// and put each files into the passed vector container
void list_files(vector<CStringW> & files_arr, LPCTSTR pstr);

// split a string using the specified symbol
vector<CStringW> split(CStringW str, char symbol);

BOOL belongs_to(CStringW str, vector<CStringW> target_array);

void _parse_line(CStringW line, short index);

// create a new file and write into the content
void write_file();

void read_file(LPCWSTR file) {
    FILE* fp;
    errno_t err_no;
    err_no = _wfopen_s(&fp, file, _T("r, ccs=UNICODE"));
    if (0 == err_no) {
        //wchar_t c;
        wchar_t line[1000];
        while ((fgetws(line, 1000, fp)) != NULL) {
            _parse_line(line, 14);
        }
        //c = fgetwc(fp);
        /*while (c != WEOF) {
            if (c == '\n') {
                printf("%s\n", "---------");
            }
            putwchar(c);
            c = fgetwc(fp);
        }*/
        fclose(fp);
    }

}

void list_files(vector<CStringW> & files_arr, LPCTSTR pstr) {
    CFileFind finder;
    CStringW strWildcard(pstr);
    strWildcard += _T("\\*.csv*");
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

void _parse_line(CStringW line, short index) {
    CStringW copy_line = line;
    int fquote_index = line.Find('\"');
    if (fquote_index != -1) {
        int lquote_index = 0, _index;
        while ((_index = line.Find('\"')) != -1) {
            line = CStringW(line.Right(line.GetLength() - _index - 1));
            lquote_index += _index + 1;
        }
        int len = lquote_index - fquote_index;
        CStringW left = copy_line.Left(fquote_index);
        line = CStringW(copy_line.Right(copy_line.GetLength() - fquote_index));
        CStringW right = CStringW(line.Right(line.GetLength() - len));
        line = CStringW(line.Left(len));
        line.Replace('\,', '\t');
        line = left + line + right;
    }
    
    vector<CStringW> arr = split(line, ',');
    for (int i = 0; i < arr.size(); i++) {
        if (arr.at(i).Find('\t') != -1) {
            arr.at(i).Replace('\t', ',');
        }
    }
    CStringW specifed_str = arr.at(index);
    printf("%S\n", specifed_str);

}

BOOL belongs_to(CStringW str, vector<CStringW> target_array) {
    int i = 0, len = target_array.size();
    for (; i < len; i++) {
        if (target_array.at(i) == str) {
            return TRUE;
        }
    }
    return FALSE;
}

vector<CStringW> split(CStringW str, char symbol) {
    int index, len;
    vector<CStringW> result;
    while ((index = str.Find(symbol)) != -1) {
        result.push_back(str.Left(index));
        len = str.GetLength();
        str = CStringW(str.Right(len - index - 1));
    }
    result.push_back(str);
    return result;
}

void write_file(CStringW path, CStringW line) {
    FILE* fp;
    errno_t err_no = _wfopen_s(&fp, path, _T("r"));
    // no such file or directory
    if (err_no == 2) {
        if (!CreateDirectoryW(path, NULL)) {
            printf("create directory filed (%d)\n", GetLastError());
            return;
        }
    }

    //fwrite(&fp, sizeof(line), );
}

void init_env() {

}

int _tmain(int argc, _TCHAR* argv[])
{
    vector<CStringW> files_arr;
    setlocale(LC_ALL, "chs");
    list_files(files_arr, _T("E:\\–Ë«Û"));
    for (unsigned int i = 0; i < files_arr.size(); i++) {
        LPWSTR file_path = (LPWSTR)(LPCTSTR)files_arr.at(i);
        read_file(file_path);
    }
    
	return 0;
}

