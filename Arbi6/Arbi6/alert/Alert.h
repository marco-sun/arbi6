#include "StrCoding.h"
#define char_num(char) char >= '0' && char <='9'
char* C (string str)
{
    char *chars = new char[str.size() + 1];
    strcpy_s(chars, str.size() + 1, str.c_str());
    return chars;
}
template <class T> string S (T num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}
void PlayMP3 (string file)
{
    MCI_OPEN_PARMS mciOpen;
    mciOpen.lpstrDeviceType = "mpegvideo";
    mciOpen.lpstrElementName = file.c_str();
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);

    MCI_PLAY_PARMS mciPlay;
    mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
}
void GoogleAlert (string msg)
{
    string url = "http://translate.google.cn/translate_tts?tl=zh-CN&q=" + UrlUTF8(C(msg));
    static int index_file = 0; string file = "dealstream\\" + S(++index_file) + ".mp3";
    HRESULT result = URLDownloadToFile(NULL, url.c_str(), file.c_str(), 0, NULL);
    if (result == S_OK) PlayMP3(file);
}
void ThreadGoogleAlert (PVOID msg)
{
    GoogleAlert((char*)msg);
}
string sep_num (string str)
{
    string result;
    for (size_t i = 0; i < str.size() - 1; i++)
        result += char_num(str[i]) && str[i + 1] != '.' ? S(str[i]) + "-" : S(str[i]);
    return result + str[str.size() - 1];
}
