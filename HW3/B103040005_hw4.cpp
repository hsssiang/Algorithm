#include<iostream>
#include<cstdlib>
#include<cmath>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<random>
#include<algorithm>
#include<map>
using namespace std;

int cmp(pair<int,float>a,pair<int,float>b){
    return a.second<b.second;
}

int main()
{
    fstream c,result;
    string temp,path;
    int temp_w,temp_p,itemNum,capacity,maxworth;
    vector <int>weight,price;
    srand(time(NULL));
    int population_size = 1000, Evaluation_max;
	int avg=0;
	vector<pair<int, float> > cp;

    cout << "input a file name:";
    cin >> temp;
    
    path = "./dataset/" + temp + "/item.txt";
    c.open(path.c_str());
    if( !c ){
		cout << "Cannot find the file\n";
		return 0;
	}
	path = "ans_" + temp + ".txt";
    result.open(path.c_str(), ios::out);

	cout << "input the evalutaion_max accroding to the data_set: ";
	cin >> Evaluation_max;

    c >> itemNum >> capacity;
	int count=0;
    while(c >> temp_w >> temp_p)
    {
    	weight.push_back(temp_w);
    	price.push_back(temp_p);
		cp.push_back({count, float(temp_p)/float(temp_w)});
		count++;
    }
	sort(cp.begin(),cp.end(),cmp);
    c.close();
    

	int run=0;
	for(run=0;run<30;run++){
		
		int evaluation_max=Evaluation_max;
		map<string, int> dup;
		int **s = (int **) malloc(sizeof(int *) * population_size);
		for (int i = 0; i < population_size; i++) {
			s[i] = (int *) malloc(sizeof(int) * itemNum);
			for (int j = 0; j < itemNum; j++) {
				s[i][j] = 0;
			}
		}

		
		for (int i = 0; i < population_size; i++) {
			for (int j = 0; j < itemNum; j++) {
				s[i][j] = rand()%2;
			}
		}
		int *O_value = (int *) malloc(sizeof(int) * (population_size));
		for(int i=0;i<population_size;i++){
			O_value[i]=0;
		}

        //evaluation    
		int row=0;
		int totalP=0,totalC=0;
		for(int i=0;i<population_size;i++){
			string T="";
			int j;
			totalP=0,totalC=0;
			for(j=0;j<itemNum;j++){
				if(s[i][j]){
                    totalC+=weight[j];
                    totalP+=price[j];
                    T+='1';
                }
                else{
                    T+='0';
                }
			}
			if(totalC<=capacity){
				O_value[i]=totalP;
			}
			else{
				for(int k=0;k<itemNum;k++){
					if(s[i][cp[k].first]){
                        T[cp[k].first]='0';
						s[i][cp[k].first]=0;
						totalP-=price[cp[k].first];
						totalC-=weight[cp[k].first];
					}
					if(totalC<=capacity){
						O_value[i]=totalP;
						break;
					}
				}
			}
            if(dup.find(T) == dup.end()){
                for(int k=0;k<itemNum;k++)
                    s[row][k] = s[i][k];
                dup[T] = 1;
                row++;
            }
            else if(dup[T] < 5){
                for(int k=0;k<itemNum;k++)
                    s[row][k] = s[i][k];
                dup[T]++;
                row++;
            }
		}
		
		while(evaluation_max){
			int **domi = (int **) malloc(sizeof(int *) * population_size);
			for (int i = 0; i < population_size; i++) {
				domi[i] = (int *) malloc(sizeof(int) * itemNum);
				for (int j = 0; j < itemNum; j++) {
					domi[i][j] = 0;
				}
			}
			//Selection
			int *shu=(int *)malloc(sizeof(int) * row);
			for(int j=0;j<row;j++){
				shu[j]=j;
			}
			int check=0,count=0;

			random_shuffle(&shu[0],&shu[row-1]);
			while(check < population_size){
				if(O_value[shu[2*count]]>=O_value[shu[2*count+1]]){
					for(int j=0;j<itemNum;j++){
						domi[check][j] = s[shu[2*count]][j] ;
					}
				}
				else{
					for(int j=0;j<itemNum;j++){
						domi[check][j] = s[shu[2*count+1]][j] ;
					}
				}
				if(count >= (row/2)-1){
					random_shuffle(&shu[0],&shu[row-1]);
					count=0;
				}
				count++;
				check++;
			}

			
			//crossover - uniform
			default_random_engine e(time(0));
			uniform_real_distribution<double> rate(0,1);
			int *Mask = (int *)malloc(sizeof(int) * itemNum);
			for(int i=0;i<itemNum;i++){
				Mask[i] = 0;
			}
			for(int i=0;i<itemNum;i++){
				Mask[i] = rand()%2;
			}

			float cross_rate = 1.0;
			for(int i=0; i+1 <population_size/2; i=i+2){
				if(rate(e) < cross_rate ){
					for(int j=0; j<itemNum; j++){
						if( Mask[j] ){
							int temp = domi[i][j];
							domi[i][j]=domi[i+1][j];
							domi[i+1][j] = temp;
						}
					}
				}
			}


			//mutation
			float mutation_rate = 0.001;
			for(int i=0;i<population_size;i++){
				if(rate(e) <  mutation_rate){
					int randNum = rand()%itemNum/2;
					int m[randNum+1];
					for(int j=0;j<randNum+1;j++){
						m[j] = j;
					}
					random_shuffle(&m[0],&m[randNum+1]);
					for(int j=0;j<randNum+1;j++){
						domi[i][m[j]]= !domi[i][m[j]];
					}
				}
			}
			//evaluation
			for(int i=0;i<population_size;i++){
				O_value[i]=0;
			}
			row = 0;
			map<string, int> Dup;
			for(int i=0;i<population_size;i++){
				int j;
				string T="";
				totalP=0,totalC=0;
				for(j=0;j<itemNum;j++){
					if(domi[i][j]){
						totalC+=weight[j];
						totalP+=price[j];
						T+='1';
					}
					else{
						T+='0';
					}
				}
				if(totalC <= capacity){
					O_value[i]=totalP;
				}
				else{
					for(int k=0;k<cp.size();k++){
						if(domi[i][cp[k].first]){
							T[cp[k].first]='0';
							domi[i][cp[k].first]=0;
							totalP-=price[cp[k].first];
							totalC-=weight[cp[k].first];
						}
						if(totalC<=capacity){
							O_value[i]=totalP;
							break;
						}
					}
				}
				if(Dup.find(T) == Dup.end()){
					for(int k=0;k<itemNum;k++)
						s[row][k] = domi[i][k];
					Dup[T] = 1;
					row++;
				}
				else if(Dup[T] < 5){
					for(int k=0;k<itemNum;k++)
						s[row][k] = domi[i][k];
					Dup[T]++;
					row++;
				}
			}

			for(int i = 0; i < population_size; i++)
    			free(domi[i]);
			free(domi);

			free(shu);
			evaluation_max--;
		}

		int max=0;
		maxworth = O_value[0];
		for(int i=0; i<population_size; i++){
			if(maxworth < O_value[i]){
				maxworth = O_value[i];
				max = i;
			}
		}

	 	cout<<"round: "<<run+1<<endl;
		cout<<"maxworth: " << maxworth <<endl;
		cout<<"solution: ";
		for(int i=0;i<itemNum;i++){
			cout<<s[max][i];
		}
		cout<<endl;
		result << "round: "<<run+1<<endl;
		result << "maxworth: " << maxworth <<endl;
		result << "solution: ";
		for(int i=0;i<itemNum;i++){
			result << s[max][i];
		}
		result << endl;

		avg+=maxworth;
		for(int i = 0; i < population_size; i++)
    		free(s[i]);
		free(s);

		free(O_value);
	}
   
	cout<<endl;
	cout << "average " << avg/30 << endl;
	result << "average:" << avg/30 << "\n";
	
	cout << endl;
	result.close();
}
