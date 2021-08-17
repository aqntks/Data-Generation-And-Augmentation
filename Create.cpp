#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tinyxml2.h"
#include "putUnitext.h"
#include <fstream>
#include <string>
#include<ctime>
#include <filesystem>
#include <vector>


using namespace cv;
using namespace std;
using namespace tinyxml2;

using std::cout; using std::cin;
using std::endl; using std::vector;
using std::filesystem::directory_iterator;

int main(int, char**)
{
    std::cout << " 생성 시작\n";
    srand((unsigned int)time(NULL));
    tinyxml2::XMLDocument doc;

    String jumin_title_path = "C:/Users/home/Desktop/work/id/crop/title_jumin";
    String jumin_issue_path = "C:/Users/home/Desktop/work/id/crop/jumin_issue";
    string jumin_issue[10];
    int i = 0;
    for (const auto& file : directory_iterator(jumin_issue_path))
    {
        jumin_issue[i] = file.path().u8string();
        i++;
    }      

    // 이미지, xml 로드
	Mat img = imread("C:/Users/home/Desktop/work/test/jumin_1.jpg");
    const char* path = "C:/Users/home/Desktop/work/test/jumin_1.xml";

    // 한글 목록 로드 (591자)
    string str;
    int index = 0;
    string hangul[591];
    ifstream file("hangul_591.txt");
    while (getline(file, str))
    {
        hangul[index] = str;
        index++;
    }

    // xml 로드
    doc.LoadFile(path);
    XMLElement* xml_obj = doc.FirstChildElement("annotation")->FirstChildElement("object");
    while (xml_obj)
    {
        XMLElement* xml_name = xml_obj->FirstChildElement("name");
        string strTest = xml_name->GetText();

        // 멀티바이트 to 유니코드
        CString sRow(_T(""));
        sRow += CA2W(strTest.c_str(), CP_UTF8); 
        strTest = string(CT2CA(sRow));

        string x1 = xml_obj->FirstChildElement("bndbox")->FirstChildElement("xmin")->GetText();
        string y1 = xml_obj->FirstChildElement("bndbox")->FirstChildElement("ymin")->GetText();
        string x2 = xml_obj->FirstChildElement("bndbox")->FirstChildElement("xmax")->GetText();
        string y2 = xml_obj->FirstChildElement("bndbox")->FirstChildElement("ymax")->GetText();

        int w = stoi(x2) - stoi(x1);
        int h = stoi(y2) - stoi(y1);

        // char 버퍼
        char buffer[50];
        bool font = false;

        // 숫자 합성 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 숫자 
        if (strTest == "regnum_dg" || strTest == "addr_dg")
        {
            // 0 ~ 9 난수
            int ranNum = rand() % 10;
        
            string value = to_string(ranNum);

            strcpy_s(buffer, value.c_str());
            font = true;
        }
        // 한글 합성 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글 한글
        if (strTest == "name_ko" || strTest == "addr_ko" || strTest == "issueplace_ko")
        {
            // 0 ~ 9 난수
            int ranNum = rand() % 591;
            string value = hangul[ranNum];

            strcpy_s(buffer, value.c_str());
            font = true;
        }
        if (strTest == "regnum_hyp")
        {
            strcpy_s(buffer, "-");
            font = true;
        }
        if (strTest == "issuedate_dot")
        {
            strcpy_s(buffer, ".");
            font = true;
        }
        if (strTest == "title_jumin")
        {

            string jumin_title_file[25];
            int i = 0;
            for (const auto& file : directory_iterator(jumin_title_path))
            {
                jumin_title_file[i] = file.path().u8string();
                i++;
            }
            int fileRanNum = rand() % 25;
            string value = jumin_title_file[fileRanNum];

            Mat crop_img = imread(value);
            cv::Mat imageROI(img, cv::Rect(stoi(x1), stoi(y1), w, h));
            cv::resize(crop_img, crop_img, Size(w, h));
            img(Rect(stoi(x1), stoi(y1), w, h)) = crop_img;
            crop_img.copyTo(imageROI, crop_img);           
        }
        if (strTest == "issuedate_dg")
        {
           
            int ranNum = rand() % 10;
            string value = jumin_issue[ranNum];
            string jumin_issue_file[100];
            int i = 0;
            for (const auto& file : directory_iterator(value))
            {
                jumin_issue_file[i] = file.path().u8string();
                i++;
            }
            int FileRanNum = rand() % jumin_issue->size();
            value = jumin_issue_file[FileRanNum];

            Mat crop_img = imread(value);
            cv::Mat imageROI(img, cv::Rect(stoi(x1), stoi(y1), w, h));
            cv::resize(crop_img, crop_img, Size(w, h));
            img(Rect(stoi(x1), stoi(y1), w, h)) = crop_img;
            crop_img.copyTo(imageROI, crop_img);
        }
          
        // 폰트 삽입
        if (font) 
        {
            putUniText(img, img, buffer, Point(stoi(x1), stoi(y1)), CV_RGB(33, 33, 33), w, h);
        }
        xml_obj = xml_obj->NextSiblingElement();
    }
    imwrite("C:/Users/home/Desktop/work/img.jpg", img);
}
