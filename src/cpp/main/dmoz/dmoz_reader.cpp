#include <iostream>
#include "dmoz_reader.h"
#include <tr1/unordered_map>
#include <string>

using namespace std;

int DmozReader::ComputeDistance(string str1, string str2) {
  size_t pos1 = -1, pos2 = -1, last_pos1 = -1, last_pos2 = -1;
  int rem_depth1 = 0, rem_depth2 = 0;
  while (true) {
    last_pos1 = pos1;
    last_pos2 = pos2;
    pos1 = str1.find_first_of("/", pos1+1);
    pos2 = str2.find_first_of("/", pos2+1);
    //printf("%s\n", str1.substr(last_pos1+1, pos1 - last_pos1 - 1).c_str());
    //printf("%s\n", str2.substr(last_pos2+1, pos2 - last_pos2 - 1).c_str());
    if (str1.substr(last_pos1+1, pos1 - last_pos1 - 1).compare(str2.substr(last_pos2+1, pos2 - last_pos2 - 1))) break;
    //++depth;
    if (pos1 == string::npos || pos2 == string::npos) {
      rem_depth1 = -1;
      rem_depth2 = -1;
      break;
    }
  }
  while (true) {
    ++rem_depth1;
    //printf("in while: %d\n", rem_depth1);
    if (pos1 == string::npos) break;
    last_pos1 = pos1;
    pos1 = str1.find_first_of("/", pos1+1);
  }
  while (true) {
    ++rem_depth2;
    //printf("in while: %d\n", rem_depth2);
    if (pos2 == string::npos) break;
    last_pos2 = pos2;
    pos2 = str2.find_first_of("/", pos2+1);
  }
  //printf("%d %d %d\n", depth, rem_depth1, rem_depth2);
  return rem_depth1 + rem_depth2;
}

int DmozReader::ComputeDistanceFromRoot(string str1, string str2) {
  size_t pos1 = -1, pos2 = -1, last_pos1 = -1, last_pos2 = -1;
  int depth = 0;
  while (true) {
    last_pos1 = pos1;
    last_pos2 = pos2;
    pos1 = str1.find_first_of("/", pos1+1);
    pos2 = str2.find_first_of("/", pos2+1);
    //printf("%s\n", str1.substr(last_pos1+1, pos1 - last_pos1 - 1).c_str());
    //printf("%s\n", str2.substr(last_pos2+1, pos2 - last_pos2 - 1).c_str());
    if (str1.substr(last_pos1+1, pos1 - last_pos1 - 1).compare(str2.substr(last_pos2+1, pos2 - last_pos2 - 1))) break;
    ++depth;
    if (pos1 == string::npos || pos2 == string::npos) {
      break;
    }
  }
  return depth;
}

void DmozReader::Init(FILE* f) {
  input = f;
}

bool DmozReader::IsOpenTopic () {
  for (int i = 0; i < (int)strlen(actual_line)-5; ++i) {
    if (!strncmp(actual_line + i, "<Topic", 6)) return true;
  }
  return false;
}

bool DmozReader::IsEndTopic() {
  for (int i = 0; i < (int)strlen(actual_line)-6; ++i) {
    if (!strncmp(actual_line + i, "</Topic", 7)) return true;
  }
  return false;
}

bool DmozReader::IsOpenLink() {
  for (int i = 0; i < (int)strlen(actual_line)-4; ++i) {
    if (!strncmp(actual_line + i, "<link", 5)) return true;
  }
  return false;
}

void DmozReader::ReadTopic() {
  int start, end;
  for (start = 0; start < (int)strlen(actual_line)-4; ++start) {
    if (!strncmp(actual_line + start, "r:id=", 5)) break;
  }
  start += 6;
  for (end = start; end < (int)strlen(actual_line)-1; ++end) {
    if (!strncmp(actual_line + end, "\">", 2)) break;
  }
//  printf("%d %d\n", start, end);
  strncpy(actual_topic, actual_line + start, end - start);
  *(actual_topic + end - start) = '\0';
}

bool DmozReader::UntilNextTopic() {
  bool found;
  while (true) {
    fgets(actual_line, 10000, input);
    if (feof(input)) break;
    found = IsOpenTopic();
    if (found) return true;
  }
  return false;
}

bool DmozReader::UntilNextLink() {
  bool is_end_topic, is_open_link;
  while (true) {
    fgets(actual_line, 10000, input);
    if (feof(input)) break;
    is_end_topic = IsEndTopic();
    if (is_end_topic) break;
    is_open_link = IsOpenLink();
    if (is_open_link) return true;
  }
  return false;
 
}

int DmozReader::FindLinkStart() {
  int i;
  for (i = 0; i < (int)strlen(actual_line)-10; ++i) {
    if (!strncmp(actual_line + i, "r:resource=", 11)) return i+12;
  }
  return -1;
}

int DmozReader::FindLinkEnd() {
  int i;
  for (i = 0; i < (int)strlen(actual_line)-5; ++i) {
    if (!strncmp(actual_line + i, "</link", 6)) return i-2;
  }
  return -1;
}

void DmozReader::ReadUrl(int pos1, int pos2, char* url) {
  strncpy(url, actual_line + pos1, pos2-pos1);
  *(url + pos2-pos1) = '\0';
}

void DmozReader::ReadLinks() {
  char url[1024];
  int pos1, pos2;
  bool is_open_link;
  while (true) {
     pos1 = FindLinkStart();
     pos2 = FindLinkEnd();
     ReadUrl(pos1, pos2, url);
     url2topic[string(url)] = string(actual_topic);
     fgets(actual_line, 10000, input);
     //if (feof(input)) break;
     is_open_link = IsOpenLink();
     if (!is_open_link) break;
  }
}

void DmozReader::Read() {
  bool found;
  while (true) {
    found = UntilNextTopic();
    if (!found) break;
    ReadTopic();
    found = UntilNextLink();
    if (!found) continue;
    //printf("%s\n", actual_topic);
    ReadLinks();
  }
}

void DmozReader::Flush(FILE* f) {
  for (tr1::unordered_map<string, string>::iterator it = url2topic.begin();
          it != url2topic.end(); ++it) {
    fprintf(f, "%s %s\n", it->first.c_str(), it->second.c_str());
  }
}

tr1::unordered_map<string, string> DmozReader::getUrl2Topic() {
  return url2topic;
}


