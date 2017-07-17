[phpjieba](https://github.com/andsky/phpjieba)
=========

phpJieba是"结巴"中文分词的php语言版本。


## 特点

+ 词典载入方式灵活，无需配置词典路径也可使用，需要定制自己的词典路径时也可灵活定制。
+ 底层算法实现是C++，性能高效。
+ 支持多种分词算法，各种分词算法见[CppJieba]的README.md介绍。
+ 支持动态补充词库。

## 安装
```
git clone https://github.com/andsky/phpjieba.git
cd phpjieba
phpize
./configure
make
make install

cp -R include/cjieba/dict /etc/jieba

## 用法

$j=new jieba();
$rs = $j->cut('他来到了网易杭研大厦', true);
print_r($rs);
$rs =$j->cut('他来到了网易杭研大厦', false);
print_r($rs);
$rs = $j->cut_all('我来到北京清华大学');
print_r($rs);
$rs = $j->cut_for_search('我来到北京清华大学');
print_r($rs);
$rs = $j->extract_tags('我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
print_r($rs);
$rs = $j->cut_tagging('我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
print_r($rs);


## 鸣谢

[Jieba中文分词]
[CppJieba]

