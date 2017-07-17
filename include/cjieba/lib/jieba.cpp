extern "C" {
    #include "jieba.h"
}

#include "cppjieba/Jieba.hpp"
#include "cppjieba/KeywordExtractor.hpp"

using namespace std;

extern "C" {

Jieba NewJieba(const char* dict_path, const char* hmm_path, const char* user_dict, const char* idf_path, const char* stop_word) {
  Jieba handle = (Jieba)(new cppjieba::Jieba(dict_path, hmm_path, user_dict, idf_path, stop_word));
  return handle;
}

void FreeJieba(Jieba handle) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  delete x;
}

CJiebaWord* Cut(Jieba handle, const char* sentence, size_t len, bool hmm = true) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  vector<string> words;
  string s(sentence, len);
  x->Cut(s, words, hmm);
  CJiebaWord* res = (CJiebaWord*)malloc(sizeof(CJiebaWord) * (words.size()+1));
  for (std::vector<string>::iterator it = words.begin(); it!=words.end(); ++it){
    int pos = it-words.begin();
    res[pos].word = (*it).c_str();
    res[pos].len = (*it).size();
  }
  res[words.size()].word = NULL;
  res[words.size()].len = 0;
  return res;
}

CJiebaWord* CutAll(Jieba handle, const char* sentence, size_t len) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  vector<string> words;
  string s(sentence, len);
  x->CutAll(s, words);
  
  CJiebaWord* res = (CJiebaWord*)malloc(sizeof(CJiebaWord) * (words.size() + 1));
   for (std::vector<string>::iterator it = words.begin(); it!=words.end(); ++it){
    int pos = it-words.begin();
    res[pos].word = (*it).c_str();
    res[pos].len = (*it).size();
  }
  res[words.size()].word = NULL;
  res[words.size()].len = 0;
  return res;
}

CJiebaWord* CutForSearch(Jieba handle, const char* sentence, size_t len) {
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  vector<string> words;
  string s(sentence, len);
  x->CutForSearch(s, words);
  
  CJiebaWord* res = (CJiebaWord*)malloc(sizeof(CJiebaWord) * (words.size() + 1));
  for (std::vector<string>::iterator it = words.begin(); it!=words.end(); ++it){
    int pos = it-words.begin();
    res[pos].word = (*it).c_str();
    res[pos].len = (*it).size();
  }
  res[words.size()].word = NULL;
  res[words.size()].len = 0;
  return res;
}

CJiebaWord* CutWithTagging(Jieba handle, const char* sentence, size_t len)
{
  cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
  vector<pair<string, string> > words;
  string s(sentence, len);
  x->Tag(s, words);

  CJiebaWord* res = (CJiebaWord*)malloc(sizeof(CJiebaWord) * (words.size() + 1));

  for (std::vector<pair<string, string> >::iterator it = words.begin(); it!=words.end(); ++it){
    int pos = it-words.begin();
    res[pos].word = (*it).first.c_str();
	res[pos].tagging = (*it).second.c_str();
	res[pos].len = (*it).first.size();
  }
  res[words.size()].word = NULL;
  res[words.size()].len = 0;
  return res;
}

void FreeWords(CJiebaWord* words) {
  free(words);
}

bool JiebaInsertUserWord(Jieba handle, const char* word)
{
	cppjieba::Jieba* x = (cppjieba::Jieba*)handle;
	return x->InsertUserWord(string(word), "u");
}



CJiebaWord* Extract(Jieba handle, const char* sentence, size_t len, size_t topn) {
  cppjieba::Jieba *x = (cppjieba::Jieba*)handle;
  vector<cppjieba::KeywordExtractor::Word> words;
  string s(sentence, len);
  x->extractor.Extract(s, words, topn);
  CJiebaWord* res = (CJiebaWord*)malloc(sizeof(CJiebaWord) * (words.size() + 1));
  for (std::vector<cppjieba::KeywordExtractor::Word>::iterator it = words.begin(); it!=words.end(); ++it){
    int pos = it-words.begin();
    res[pos].word = (*it).word.c_str();
    res[pos].len = (*it).word.size();
	res[pos].weight = (*it).weight;
  }
  res[words.size()].word = NULL;
  res[words.size()].len = 0;
  return res;
}

} // extern "C"
