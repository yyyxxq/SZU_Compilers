#include<bits/stdc++.h>
#include<iostream>
#include"gramma.h"
void split_gramma(std::string &s,std::string &pre,std::string &suf){
	int n=s.length();
	int flag=0;
	for(int i=0;i<n;i++){
		if(s[i]=='-'||s[i]=='>')flag=1;
		if(s[i]=='-'||s[i]=='>')continue;
		if(flag==0)pre=pre+s[i];
		if(flag==1)suf=suf+s[i];
	}
}
void split_end(std::string &s,Gramma *gr){
	int n=s.length();
	std::string tmp;
	for(int i=0;i<n;i++){
		if(s[i]=='|'){
			gr->to.push_back(tmp);
			tmp="";
			continue;
		}
		tmp=tmp+s[i];
	}
	gr->to.push_back(tmp);
}
bool check1(Gramma *gr){
	for(uint i=0;i<gr->to.size();i++){
		if(gr->start.length()>gr->to[i].length())return false;
	}
	return true;
}
bool check2(Gramma *gr){
	int n=gr->start.length();
	for(int i=0;i<n;i++){
		if(gr->start[i]<'A'||gr->start[i]>'Z')return false;
	}
	return true;
}
bool check3(Gramma *gr){
	if(gr->start.length()!=1)return false;
	if(gr->start[0]<'A'||gr->start[0]>'Z')return false;
	for(int i=0;i<gr->to.size();i++){
		int sum=0;
		int m=gr->to[i].length();
		for(int j=0;j<m;j++){
			if(gr->to[i][j]>='A'&&gr->to[i][j]<='Z')sum++;
		}
		if(sum>1)return false;
		m--;
		if(gr->to[i][0]>='A'&&gr->to[i][0]<='Z')left_capital++;
		if(gr->to[i][m]>='A'&&gr->to[i][m]<='Z')right_capital++;
	}
	if(left_capital!=0&&right_capital!=0)return false;
	return true;
}
bool check(std::string &s,std::string &t,int st,int m){
	for(int i=0;i<m;i++){
		if(s[i+st]!=t[i])return 0;
	}
	return 1;
}
int dfs(std::string s){
	if(find_solution[s]==-1)return 0;
	//std::cout<<s<<std::endl;
	if(s.length()>end.length())return 0;
	if(s==end){
		ans.push_back(s);
		return 1;
	}
	int n=s.length();
	for(int i=0;i<gramma_vector.size();i++){
		Gramma* gr=&gramma_vector[i];
		int m=gr->start.length();
		for(int j=0;j<n;j++){
			if(check(s,gr->start,j,m)){
				for(int k=0;k<gr->to.size();k++){
					std::string tmp=s;
					tmp.replace(j,m,gr->to[k]);
					if(dfs(tmp)){
						ans.push_back(s);
						return 1;
					}
				}
			}
		}
	}
	find_solution[s]=-1;
	return 0;
}
int main(int argc,char *argv[]){
	std::ifstream cin1,cin2;
	cin1.open(argv[1]);
	std::string s;
	int level=3;
	while(std::getline(cin1,s)){
		std::string pre,suf;
		Gramma tmp;
		split_gramma(s,tmp.start,suf);
		split_end(suf,&tmp);
		//for(int i=0;i<tmp.to.size();i++)std::cout<<tmp.to[i]<<" ";
		//std::cout<<std::endl;
		//std::cout<<pre<<" "<<suf<<std::endl;
		if(!check1(&tmp))level=std::min(level,0);
		if(!check2(&tmp))level=std::min(level,1);
		if(!check3(&tmp))level=std::min(level,2);
		gramma_vector.push_back(tmp);
	}
	if(level==0)printf("此文法是零型文法\n");
	if(level==1)printf("此文法是一型文法\n");
	if(level==2)printf("此文法是二型文法\n");
	if(level==3){
		printf("此文法是三型文法\n");
		if(right_capital)printf("而且还是右线性文法\n");
		if(left_capital) printf("而且还是左线性文法\n");
	}
	if(argc==3){
		cin2.open(argv[2]);
		std::getline(cin2,end);
		if(dfs("S")==0){
			printf("no solution\n");
		}else{
			for(int i=ans.size()-1;i>=0;i--){
				std::cout<<ans[i]<<std::endl;
			}
		}
	}
	return 0;
}
