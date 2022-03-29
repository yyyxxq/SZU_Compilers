#include<iostream>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<map>
class Gramma{
	public:
	std::string start;
	std::vector<std::string>to;
	std::vector<Gramma*>to_pointer;
};
std::vector<Gramma>gramma_vector;
int left_capital,right_capital;
std::string end;
std::map<std::string,int>find_solution;
std::vector<std::string>ans;
void split_gramma(std::string &s,std::string &pre,std::string &suf);//分割规则的左右两边
void split_end(std::string &s,Gramma *gr);//分割规则右边的元素
bool check1(Gramma *gr);//检验是否是一型文法 右边的长度>=左边的长度
bool check2(Gramma *gr);//检验是否是二型文法 左边必须是非终结符
bool check3(Gramma *gr);//检验是否是三型文法 式子左边只能有一个字符，而且必须是非终结符
                        //式子右边最多有二个字符。如果有二个字符必须是一个终结符和一个非终结符，如果只有一个字符，那么必须是终结符。
                        //式子右边的格式一定要一致。也就是说如果有一个是（终结符+非终结符）那么所有的式子都必须是（终结符+非终结符）， 如果有一个是（非终结符+终结符），那么所有的式子都必须是（非终结符+终结符）
