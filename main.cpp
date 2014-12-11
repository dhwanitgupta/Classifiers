#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include <algorithm>
#include"ClassifierStructure.h"
int main(){
	Dataset D;
	int algo , comb;
	char filename[100];
	scanf("%s",filename);
	scanf("%d%d",&algo,&comb);
	D.readData(filename);
	crossValidate(D,5,0,algo,comb);
	return 0;
}
