#include <iostream>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <vector>
#include <limits.h>
#include <cmath>

using namespace std;

int column;
	
struct Node{
public:
	Node* L;
	Node* R;
	int feature_id;
	bool is_leaf;
	int num_features;

	Node()
	{
		L = NULL;
		R = NULL;
		is_leaf = true;
	}

	void calc_best_fit(int** train,int l1,int l2)  //train[0] will contain first training example with n features and final value , thus lenght n+1
	{
		sort(train,train+l1);
		for(int i=0;i<l1;i++)
		{
			for(int j=0;j<l2;j++)
			{
				cout<<train[i][j]<<" ";
			}
			cout<<endl;
		}
	}

	// int predict()
	// {

	// }
};	

bool sortcol(const vector<float>& v1, const vector<float>& v2) 
{
 return v1[column] < v2[column];
}

float var(vector<float> &v,int start,int end)
{
	float mean = 0.0;
	for(int k=start;k<=end;k++) mean += v[k]/(end-start+1);
	float var = 0.0;
	for(int k=start;k<=end;k++) var += ((v[k]-mean)*(v[k]-mean))/(end-start);
	return var;	

}

float split(vector<vector<float> > &train,int row)
{
	int l1 = train.size();
	int l2 = train[0].size();

	vector<float> error_column;
	for(int i=0;i<l1;i++)
	{
		error_column.push_back(train[l2-1][i]);
	}

	float val1 = var(error_column,0,row);
	float val2 = var(error_column,row+1,l1-1);
	return (val1*(row + 1) + val2*(l1 - row - 1))/l1;
}

void calc_best_fit(vector< vector<float> > &train)  //train[0] will contain first training example with n features and final value , thus lenght n+1
{
	int l1 = train.size();
	int l2 = train[0].size();
	int x,y;
	float min_error = INT_MAX;

	for(int i=0;i<l2;i++)
	{
		column = i;
		sort(train.begin(),train.end(),sortcol);
		for(int j=0;j<l1-1;j++)
		{
			float error = split(train,j);
			if(min_error > error)
			{
				x = i;
				y = j;
				min_error = error; 
			}
		}
	}

	cout<<x<<" "<<y<<" "<<min_error<<endl;
}

int main()
{
	vector<float> v(5,0);
	vector<vector<float> > a(5,v);

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			a[i][j] = rand()%10 + 1;
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	calc_best_fit(a);

	return 0;
}