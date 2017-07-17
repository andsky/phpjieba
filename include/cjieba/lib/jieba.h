#ifndef CJIEBA_C_API_H
#define CJIEBA_C_API_H

#include <stdlib.h>
#include <stdbool.h>

typedef void* Jieba;
Jieba NewJieba(const char* dict_path, const char* hmm_path, const char* user_dict, const char* idf_path, const char* stop_word);
void FreeJieba(Jieba);

typedef struct {
  const char* word;
  const char* tagging;
  size_t len;
  double weight;
} CJiebaWord;

CJiebaWord* Cut(Jieba handle, const char* sentence, size_t len, bool hmm);

CJiebaWord* CutAll(Jieba handle, const char* sentence, size_t len);

CJiebaWord* CutForSearch(Jieba handle, const char* sentence, size_t len);

CJiebaWord* Extract(Jieba handle, const char* sentence, size_t len, size_t topn);

CJiebaWord* CutWithTagging(Jieba handle, const char* sentence, size_t len);

void FreeWords(CJiebaWord* words);

bool InsertUserWord(Jieba handle, const char* word);
bool DelUserWord(Jieba handle, const char* word);

#endif
