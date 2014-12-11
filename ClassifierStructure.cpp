#include <map>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<string>
#include<string.h>
#include <fstream>
#include <algorithm>
#include<math.h>
#include <time.h>
#include"ClassifierStructure.h"
#include"matlib/newmatap.h"
#include"matlib/newmatio.h"
using namespace std;
Matrix confusionMat;
bool Dataset::writeData(char filename[50]){
	ofstream myfile;
	myfile.open(filename);
	vector<DataItem> td;
	td = data;
	vector<float>::iterator it;

	while(!td.empty()){

		for ( it=td.back().feature.begin() ; it < td.back().feature.end(); it++ )
			myfile <<  *it << " ";

		/*while(!td.back().feature.empty()){
		  myfile<<td.back().feature.back()<<" ";
		  td.back().feature.pop_back();
		  }*/
		myfile<<td.back().classLabel<<"\n";
		td.pop_back();
	}
	myfile.close();
}
bool Dataset::readData(string filename){
	string line;
	ifstream myfile(filename.c_str());
	int i,j;
	double n;
	char temp[100];
	while(myfile.good()){
		DataItem d;
		i = 0;
		j = 0;
		temp[i] = '\0';
		getline(myfile,line);
		while(line[i] != '\0'){
			if(line[i] == ' '){
				temp[j] = '\0';
				n = atof(temp);
				d.feature.push_back(n);
				j = 0;
				i++;
				continue;
			}
			temp[j] = line[i];
			j++;
			i++;
		}
		temp[j] = '\0';
		strcpy(d.classLabel,temp);
		if(strlen(temp) > 0)
			data.push_back(d);
	}
	DataItem q;
	/*	while(!data.empty()){
		q = data.back();
		data.pop_back();
		cout<<q.classLabel<<endl;
		while(!q.feature.empty()){
		cout<<q.feature.back()<<endl;
		q.feature.pop_back();
		}
		}*/
	return true;

}
bool check(DataItem i1,DataItem i2){
	//cout<<i1.classLabel<<" "<<i2.classLabel<<endl;
	if(strcmp(i1.classLabel,i2.classLabel)>0)
		//	if(atof(i1.classLabel) > atof(i2.classLabel))
		return 1;
	return 0;
}
vector<Dataset> splitDataset(Dataset complete,int folds , int seed){
	sort(complete.data.begin(),complete.data.end(),check);
	map<char *,int>classCount;
	pair<map<char,int>::iterator,bool> ret;
	map<char *,int>::iterator it;
	int num;
	int flag = 0;
	int count = 0;
	Dataset temp;
	temp = complete;
	while(!complete.data.empty()){
		flag = 0;
		for ( it=classCount.begin() ; it != classCount.end(); it++ )
			if(strcmp((*it).first,complete.data.back().classLabel)==0){
				(*it).second++;
				flag = 1;
				break;
			}
		if(flag == 0){
			classCount.insert(pair<char *,int>(complete.data.back().classLabel,1));
			count++;
		}
		complete.data.pop_back();
	}

	confusionMat.ReSize(count,count);
	confusionMat = 0.0;
	complete = temp;
	int randary[100];
	int i;
	int x;
	Dataset *d = new Dataset[folds+1];
	int ind = 0;
	for ( it=classCount.begin() ; it != classCount.end(); it++ ){
		complete = temp;
		//  cout << (*it).first << " => " << (*it).second << endl;
		num = 1;
		for(i=0;i<(*it).second;i++)
			randary[i]  = 0;

		while(num <= folds){

			for(i=0;i<(*it).second/folds;i++){
				x = rand()%(*it).second;
				//srand (1);
				if(randary[x] == 0){
					randary[x] = num;
				}
				else{
					i--;
				}
			}
			num++;
		}
		for(i=0;i<(*it).second;i++)
			if(randary[i] == 0)
				randary[i] = rand()%folds + 1;
		i = 0;
		while(!complete.data.empty()){
			if(strcmp(complete.data.back().classLabel,(*it).first)==0){
				d[randary[i]].data.push_back(complete.data.back());
				i++;
			}
			complete.data.pop_back();
		}
	}
	/*	for(i=1;i<=folds;i++){
		while(!d[i].data.empty()){
		while(!d[i].data.back().feature.empty()){
//		printf("%f ",d[i].data.back().feature.back());
d[i].data.back().feature.pop_back();
}	
//	printf("%s %d\n",d[i].data.back().classLabel,i);
d[i].data.pop_back();
}
}*/
vector<Dataset> vecd;
for(i=1;i<=folds;i++)
vecd.push_back(d[i]);
/*	Dataset t;
	vector<Dataset> Obj;
	Obj = vecd;
	while(!Obj.empty()){
	t = Obj.back();
	while(!t.data.empty()){
	while(!t.data.back().feature.empty()){
	printf("%f ",t.data.back().feature.back());
	t.data.back().feature.pop_back();
	}   
	printf("%s\n",t.data.back().classLabel);
	t.data.pop_back();
	}   
	Obj.pop_back();
	printf("\n");
	}   
 */
return vecd;
}

Dataset mergeDatasets(vector<Dataset> toMerge,int numDatasets,vector<int> indicesToMerge){
	Dataset merge;
	Dataset temp;
	DataItem d;
	int i;
	int index;
	while(!indicesToMerge.empty()){
		index = indicesToMerge.back();
		//cout<<"ind "<<index<<endl;
		indicesToMerge.pop_back();
		//	cout<<numDatasets<<" " << index<<endl;
		temp = toMerge.at(numDatasets - index -1);
		while(!temp.data.empty()){
			merge.data.push_back(temp.data.back());
			temp.data.pop_back();
		}
	}
	sort(merge.data.begin(),merge.data.end(),check);
	return merge;
}
LinearClassifier::LinearClassifier(){
	row  = 1;
	y.ReSize(10000,100);
	y = 1.0;
	col = 1;
	numofc = 0;
	mapindex = 0;
}
bool LinearClassifier::saveModel(string filename){
	ofstream myfile;
	myfile.open(filename.c_str());
	myfile <<"a vectors"<<endl;
	myfile << A <<endl;
	myfile.close();

}
bool LinearClassifier::loadModel(string filename){

	string line;
	ifstream myfile(filename.c_str());
	int i,j;
	double n;
	char temp[100];
	int c = 10000;
	char cls[100];
	strcpy(cls,"NULL");
	for(i=0;i<100;i++)
		numofelement[i] = 0;

	while(myfile.good()){
		i = 0;
		j = 0;
		temp[i] = '\0';
		getline(myfile,line);
		col = 2;
		while(line[i] != '\0'){
			if(line[i] == ' '){
				temp[j] = '\0';
				n = atof(temp);
				y(row,col) = n;
				col++;
				j = 0;
				i++;
				continue;
			}
			temp[j] = line[i];
			j++;
			i++;
		}
		temp[j] = '\0';
		if(strcmp(cls,"NULL") == 0 && strlen(temp)>0){
			strcpy(cls,"dasd");
			strcpy(distinctClass[numofc],temp);
			numofc++;
		}
		else if(strcmp(distinctClass[numofc-1],temp)!=0 && strlen(temp) > 0){
			strcpy(distinctClass[numofc],temp);
			numofc++;
		}

		if(strlen(temp) > 0){
			strcpy(Label[row],temp);
			row++;
		}
		numofelement[numofc]++;
		if(c == 10000)
			c = col;
	}
	col = c;
	//	for(i=0;i<numofc;i++)
	//	printf("%s\n",distinctClass[i]);
	/*for(i=1;i<row;i++){
	  for(j=1;j<=c;j++){
	  printf("%f ",y(i,j));
	  }
	  printf("%s\n",Label[i]);
	  }	*/
	return true;
}
float LinearClassifier::learnModel(Dataset trainData,int algorithm , int combination){
	trainData.writeData("temporary");
	LinearClassifier();
	loadModel("temporary");
	remove("temporary");
	int i,j,k,l;
	/*	cout<<row<<" "<<col<<endl;
		for(i=1;i<row;i++){
		for(j=1;j<=col;j++){
		printf("%f ",y(i,j));
		}
		printf("%s\n",Label[i]);
		}	*/
	Matrix Ytemp(row,col-1);
	vector<Matrix> Y;
	int numofy = 1;
	int mul = 1;
	Ytemp = 0.0;
	int xx ;
	int rowfac = row;
	vector<int> nume;
	//cout<<"row " << row<<endl;
	//cout<<"num " <<numofc<<endl;
	if(combination == 1){
		for(i=0;i<numofc;i++){
			for(j=1;j<row;j++){
				mul = -1;
				if(strcmp(distinctClass[i],Label[j])==0)
					mul = 1;
				for(k=1;k<col;k++){
					Ytemp(j,k) = mul*y(j,k);
				}
			}
			Y.push_back(Ytemp);
			nume.push_back(row);
		}
	}
	if(combination == 2){
		//	for(i=0;i<=numofc;i++)
		//		cout<<numofelement[i]<<endl;
		numofelement[numofc]--;
		//Ytemp.ReSize((2*row)/numofc+1,col-1);
		for(i=0;i<numofc;i++){
			for(j=i+1;j<numofc;j++){
				Ytemp.ReSize(numofelement[i+1] + numofelement[j+1],col-1);
				Ytemp = 0.0;
				map[mapindex][0] = i;
				map[mapindex][1] = j;
				mapindex++;
				xx = 1;
				for(k=1;k<row;k++){
					if(strcmp(distinctClass[i],Label[k]) ==0 || strcmp(distinctClass[j],Label[k]) == 0){
						mul = 1;
						if(strcmp(distinctClass[j],Label[k])==0)
							mul = -1;
						for(l=1;l<col;l++)
							Ytemp(xx,l) = mul*y(k,l);
						xx++;
					}		
				}
				//	cout<<Ytemp<<endl;
				Y.push_back(Ytemp);		
				nume.push_back(xx-1);
			}
		}
	}
	numofy = Y.size();
	Matrix a(col-1,1);
	a = 1;
	A.ReSize(col-1,numofy);
	Matrix m(rowfac,col-1);
	int p = 1;
	int dv= 0;
	if(algorithm == 1){
		Matrix yk(col-1,1);
		Matrix sumy(col-1,1);
		Matrix at(1,col-1);
		a = 1.0;
		sumy = 0.0;
		while(!Y.empty()){
			m = Y.back();
			Y.pop_back();
			a = 1.0;
			sumy = 0.0;
			rowfac = nume.back();
			nume.pop_back();
			for(k=1;k<=100;k++){
				dv = 0;
				sumy = 0.0;

				for(i=1;i<rowfac;i++){
					for(j=1;j<col;j++){
						yk(j,1) = m(i,j);
					}
					at = a.t();
					//cout<<yk<<endl;
					//cout<<at<<" "<<yk<<endl;
					at = at*yk;
					//		cout<<at<<endl;
					if(at(1,1)  < 0){
						sumy = sumy + yk;
						dv++;
					}
				}
				//	cout<<a<<" " << sumy << endl;
				//cout<<dv<<endl;
				a = a + sumy/(k);
			}
			for(i = 1;i<col;i++)
				A(i,p) = a(i,1);
			p++;
		}
		return float(float(dv)/float(rowfac));
	}
	//cout<<dv<<endl;
	//cout<<A<<endl;
	if(algorithm == 2){
		Matrix b(rowfac,1);
		Matrix mt(col-1,rowfac);
		Matrix mr(col-1,col-1);
		Matrix I(col-1,col-1);
		I = 0.0;
		for(i=1;i<=col-1;i++)
			for(j=1;j<=col-1;j++)
				if(i == j)
					I(i,j) = 3;
		b = 1;
		Matrix e,eptv;
		int check = 0;
		int neg = 0;
		while(!Y.empty()){
			rowfac  = nume.back();
			m.ReSize(rowfac,col-1);
			m = Y.back();
			Y.pop_back();
			nume.pop_back();
			b.ReSize(rowfac,1);
			mt.ReSize(col-1,rowfac);
			b = 1;
			for(k=1;k<100;k++){
				e = m*a - b;
				eptv = e;
				check = 0;
				neg = 0;
				for(i=1;i<rowfac;i++){
					if(e(i,1) > 0.00001)
						check = 1;
					if(e(i,1) < 0){
						e(i,1) = 0;
						neg++;
					}

				}
				if(check == 0 && k > 20 )
					break;
				b = b + e/(k*50);
				e = eptv;
				mt = m.t();
				mr = mt*m;
				mr = (mr + I).i();
				mt = mr*mt;
				a = mt*b;
			}
			for(i = 1;i<col;i++)
				A(i,p) = a(i,1);
			p++;
		}
		return float(float(neg)/float(rowfac));
	}
	if(algorithm == 3){
		Matrix yk(col-1,1);
		Matrix sumy(col-1,1);
		Matrix at(1,col-1);
		a = 1.0;
		sumy = 0.0;
		while(!Y.empty()){
			m = Y.back();
			Y.pop_back();
			a = 1.0;
			sumy = 0.0;
			rowfac = nume.back();
			nume.pop_back();
			for(k=1;k<=100;k++){
				dv = 0;
				sumy = 0.0;

				for(i=1;i<rowfac;i++){
					yk(j,1) = m(i,1);
					at = a.t();
					//cout<<yk<<endl;
					//cout<<at<<" "<<yk<<endl;
					at = at*yk;
					//		cout<<at<<endl;
					if(at(1,1)  < 0){
						sumy = sumy + yk;
						dv++;
					}
				}
				//	cout<<a<<" " << sumy << endl;
				//cout<<dv<<endl;
				a = a + sumy/(k);
			}
			for(i = 1;i<col;i++)
				A(i,p) = a(i,1);
			p++;
		}
		return float(float(dv)/float(rowfac));
	}
	if(algorithm == 4){
		Matrix b(rowfac,1);
		Matrix mt(col-1,rowfac);
		Matrix mr(col-1,col-1);
		Matrix I(col-1,col-1);
		I = 0.0;
		for(i=1;i<=col-1;i++)
			for(j=1;j<=col-1;j++)
				if(i == j)
					I(i,j) = 3;
		b = 1;
		Matrix e,eptv;
		int check = 0;
		int neg = 0;
		while(!Y.empty()){
			rowfac  = nume.back();
			m.ReSize(rowfac,col-1);
			m = Y.back();
			Y.pop_back();
			nume.pop_back();
			b.ReSize(rowfac,1);
			mt.ReSize(col-1,rowfac);
			b = 1;
			e = m*a - b;
			eptv = e;
			check = 0;
			neg = 0;

			b = b + e/(k*50);
			e = eptv;
			mt = m.t();
			mr = mt*m;
			mr = (mr + I).i();
			mt = mr*mt;
			a = mt*b;
			for(i = 1;i<col;i++)
				A(i,p) = a(i,1);
			p++;
		}
		return float(float(neg)/float(rowfac));
		//cout<<A<<endl;
	}
}

float LinearClassifier::classifyDataset(Dataset testset,int comb){
	float pi;
	float sums[numofc+1];
	int i = 0;
	int j,k;
	int T = 0;
	int  F = 0;
	Matrix at(col,1);
	Matrix yk(1,col);
	Matrix ans;
	/*cout<<"AAA"<<endl;
	  for(j=1;j<=numofc;j++){
	  for(i=1;i<col;i++)
	  cout<<A(i,j)<<" ";
	  cout<<endl;
	  }*/
	int siz = 0;
	int gg = 0;
	while(!testset.data.empty()){
		for(k = 0 ;k <numofc + 1;k++)
			sums[k] = 0.0;

		j = 2;
		yk(1,1) = 1;
		while(!testset.data.back().feature.empty()){
			yk(1,col - j + 1) = testset.data.back().feature.back();
			j++;
			testset.data.back().feature.pop_back();
		}
		for(j=1;j<=numofc;j++){
			for(i=1;i<col;i++){
				at(i,1) = A(i,j);
			}
			ans = yk*at;
			sums[j-1] = ans(1,1);
		}
		siz = 0;
		if(comb == 1){
			for(i=0;i<numofc;i++){
				if(sums[i] > 0.000 ){
					siz = 1;
					if(strcmp(testset.data.back().classLabel,distinctClass[numofc - i- 1])==0){
						T++;
						confusionMat(i+1,i+1) += 1;
					}
					else{
						for(j=0;j<numofc;j++)
							if(strcmp(testset.data.back().classLabel,distinctClass[numofc - j - 1])==0){
								confusionMat(j+1,i+1) += 1;
							}
						F++;

					}
					break;
				}
			}
			if(siz == 0){
				F++;
			}
		}
		else{
			int sumofc[numofc+1];
			for(i=0;i<numofc;i++)
				sumofc[i] = 0;
			for(i=0;i<numofc;i++){
				if(sums[i] > 0){
					sumofc[map[numofc - i - 1][0]]++;
				}
				else
					sumofc[map[numofc - i - 1][1]]++;
			}
			int max = sumofc[0];
			int mi = 0;
			for(i=1;i<numofc;i++){
				if(sumofc[i] > max){
					max = sumofc[i];
					mi = i;
				}
			}
			if(strcmp(testset.data.back().classLabel,distinctClass[mi])==0){
				confusionMat(mi+1,mi+1) += 1;
				T++;
			}
			else{
				F++;
				for(j=0;j<numofc;j++)
					if(strcmp(testset.data.back().classLabel,distinctClass[j])==0){
						confusionMat(j+1,mi+1) += 1;
						break;
					}

			}
			//cout<<testset.data.back().classLabel<<" "<<distinctClass[mi]<<" " << sumofc[mi]<<endl;
		}
		testset.data.pop_back();		
	}
	//cout<<confusionMat<<endl;
	//cout<<T<<endl;
	//cout<<F<<endl;
	return float(F/float(T+F));
}

float crossValidate(Dataset complete ,int folds ,float stdDev,int algo,int comb){
	vector<Dataset> Obj,imobj;
	Obj = splitDataset(complete,folds,2);
	vector<int> indtomerge;
	int i;
	int j;
	float error = 0.0;
	float sumofsq = 0.0;
	for(i = 0 ; i <folds;i++){
		while(!indtomerge.empty())
			indtomerge.pop_back();

		for(j=0;j<folds;j++){
			if(j != i)
				indtomerge.push_back(j);
		}
		//	while(!indtomerge.empty()){
		//		cout<<indtomerge.back()<< " ";
		//		indtomerge.pop_back();
		//	}
		//	cout<<endl;
		Dataset mrg;
		mrg = mergeDatasets(Obj,Obj.size(),indtomerge);
		LinearClassifier L;
		L.learnModel(mrg,algo,comb);
		error += L.classifyDataset(Obj.at(i),comb);
		sumofsq += (1.00-error)*(1.00-error);
	}
	sumofsq /= folds;
	error /= folds;
	cout<<confusionMat<<endl;
	cout<<error<<endl;	
	sumofsq = sumofsq - (1.00-error)*(1.00-error)/folds;
	cout<<sqrt(sumofsq)<<endl;

}
