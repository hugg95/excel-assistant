// pick specified lines from such a number of csv files
// author: victor.li
// date: 2015/01/18
// version: 1.0.0

#include "stdafx.h"
#include <afx.h>
#include <locale.h>
#include <direct.h>
#include "target.h"
#define MAX_CSV_LINE 1000
#define SPECIFIED_POSITION 14

using namespace std;

void read_file(vector<CStringW> & pp_res, vector<CStringW> & lt_res,
    vector<CStringW> & mt_res, vector<CStringW> & ht_res, LPCWSTR file) {
    FILE* fp;
    errno_t err_no;
    err_no = _wfopen_s(&fp, file, _T("r, ccs=UNICODE"));
    if (0 == err_no) {
        //wchar_t c;
        wchar_t line[1000];
        while ((fgetws(line, 1000, fp)) != NULL) {
            _parse_line(pp_res, lt_res, mt_res, ht_res, line, SPECIFIED_POSITION);
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

void _parse_line(vector<CStringW> & pp_res, vector<CStringW> & lt_res,
    vector<CStringW> & mt_res, vector<CStringW> & ht_res, CStringW line, short index) {
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
        line.Replace(',', '\t');
        line = left + line + right;
    }
    
    vector<CStringW> arr = split(line, ',');
    for (unsigned int i = 0; i < arr.size(); i++) {
        if (arr.at(i).Find('\t') != -1) {
            arr.at(i).Replace('\t', ',');
        }
    }
    CStringW specifed_str = arr.at(index);
    vector<CStringW> _pp = split(PP, ',');
    vector<CStringW> _lt = split(LT, ',');
    vector<CStringW> _mt = split(MT, ',');
    vector<CStringW> _ht = split(HT, ',');

    if (belongs_to(specifed_str, _pp)) {
        pp_res.push_back(line);
        return;
    } else if (belongs_to(specifed_str, _lt)) {
        lt_res.push_back(line);
        return;
    } else if (belongs_to(specifed_str, _mt)) {
        mt_res.push_back(line);
        return;
    } else if (belongs_to(specifed_str, _ht)) {
        ht_res.push_back(line);
        return;
    }
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
    result.push_back(str.Trim());
    return result;
}

void write_folder(CStringW path) {
    FILE* fp;
    // for both reading and writing
    errno_t err_no = _wfopen_s(&fp, path, _T("r+"));
    // no such file or directory, then create new one
    if (err_no == 2) {
        if (!CreateDirectoryW(path, NULL)) {
            printf("create directory filed (%d)\n", GetLastError());
            return;
        }
    }
}

void write_file(CStringW path, vector<CStringW> lines) {
    int i = 0, len = lines.size();
    /*if (len < MAX_CSV_LINE) {
        num = 1;
    } else {
        if (len % MAX_CSV_LINE) {
            num = len / MAX_CSV_LINE + 1;
        } else {
            num = len / MAX_CSV_LINE;
        }
    }
    CStringW temp;
    for (int i = 0; i < num; i++) {
        temp.Format(_T("%d\.csv"), i);
        CStringW file_path = i + path;
    }*/
    FILE* fp;
    if (_wfopen_s(&fp, (LPWSTR)(LPCTSTR)path, _T("w")) == 0) {
        for (; i < len; i++) {
            fprintf_s(fp, "%S", lines.at(i));
            //fprintf_s(fp, "%c", '\n');
        }
        fclose(fp);
    }
}

void init_env() {
    
}

int _tmain(int argc, _TCHAR* argv[])
{
    vector<CStringW> files_arr;
    vector<CStringW> pp_res;
    vector<CStringW> lt_res;
    vector<CStringW> mt_res;
    vector<CStringW> ht_res;

    setlocale(LC_ALL, "chs");
    list_files(files_arr, _T("E:\\files"));
    for (unsigned int i = 0; i < files_arr.size(); i++) {
        LPWSTR file_path = (LPWSTR)(LPCTSTR)files_arr.at(i);
        read_file(pp_res, lt_res, mt_res, ht_res, file_path);
    }
    write_folder(_T("E:\\files\\dist"));
    if (pp_res.size() > 0)
        write_file(_T("E:\\files\\dist\\gen_pp.csv"), pp_res);
    if (lt_res.size() > 0)
        write_file(_T("E:\\files\\dist\\gen_lt.csv"), lt_res);
    if (mt_res.size() > 0)
        write_file(_T("E:\\files\\dist\\gen_mt.csv"), mt_res);
    if (ht_res.size() > 0)
        write_file(_T("E:\\files\\dist\\gen_ht.csv"), ht_res);
	return 0;
}

