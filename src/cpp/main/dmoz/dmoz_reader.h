#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <tr1/unordered_map>

using namespace std;


class DmozReader {
    private:
        FILE* input;
        char actual_topic[1024];
        char actual_line[10000];
        tr1::unordered_map<string, string > url2topic;
        bool UntilNextTopic();
        bool UntilNextLink();
        void ReadLinks();
        bool IsOpenTopic();
        bool IsEndTopic();
        bool IsOpenLink();
        int FindLinkStart();
        int FindLinkEnd();
        void ReadUrl(int p1, int p2, char* url);
        void ReadTopic();
    public:
        void Read();
        void Flush(FILE* f);
        void Init(FILE* f);
        static int ComputeDistance(string str1, string str2);
        static int ComputeDistanceFromRoot(string str1, string str2);

        tr1::unordered_map<string, string> getUrl2Topic();
};



