//recognize data structure & ways interface
#include "imgdata.h"
#include "waysInterface.h"
//===== end =============

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

#define DEF_IMGPATH "/home/sharon/Documents/code/qt_project/finalpro/testImage/1011.jpg"
#define DEF_XMLPATH "/home/sharon/Documents/code/qt_project/finalpro/testImage/1011.xml"

int main()
{
	printf("*************************************************\n");
    printf("INPUT YOUR DEALING IMG & ITS XML FILEPATH.\n");
    printf("USING DEFAULT FILE PATH...\n");
    string imgpath = DEF_IMGPATH;
    string xmlpath = DEF_XMLPATH;
    printf("imgpath = %s\n", imgpath.c_str());
    printf("xmlpath = %s\n", xmlpath.c_str());

    printf("\n\nload xml data...");
    ImgData img;
    img.loadIMGsrc(imgpath, xmlpath);    

    printf("\n\ninput your recognize command:\n");
    printf("[r]-recognize command;[b]-exit;[h]-help\n");
    printf("%s\n", ATTRWAYS::instance()->getWaysDetail().c_str());
    char cmd;
    while(1) {
        cmd = getchar();
        if(cmd == 'r') {
            int attridx;
            int waysidx;
            cin >> attridx >> waysidx;
            printf("\n\nrecognizing image by%d %d...", attridx, waysidx);
            bool ret = ATTRWAYS::instance()->RECOGNIZE(&img, attridx, waysidx);
            if(ret) {
                printf("==> recognize finished!\n");
                cout << ATTRWAYS::instance()->getresult() << endl;
            } else {
                printf("==> recognize occured some error!\n");
            }
        } else if(cmd == 'b') {
            break;
        } else if(cmd == 'h') {
            printf("USAGE:");
            printf("r <attr index> <attrway>.\n");
        } else continue;
    }
	return 0;
}
