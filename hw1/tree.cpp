#include <iostream>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <vector>
#include <limits.h>
#include <cmath>
#include <math.h>

using namespace std;

int column;

float mean(vectore<float> &v,int start,int end)
{
	float mean = 0.0;
	//cout<<"start and end"<<start<<" "<<end<<endl;
	for(int k=start;k<end+1;k++) mean += v[k]/(end-start+1);
	return mean;	
}

float var(vector<float> &v,int start,int end)
{
	float mean = mean (v,start,end);
	//	cout <<"mean"<<mean<<endl;
	float var = 0.0;
	for(int k=start;k<end+1;k++) var += ((v[k]-mean)*(v[k]-mean))/(end-start+1);
	return var;	

}
	
struct Node{
public:
	Node* L;
	Node* R;
	int feature_id;
	bool is_leaf;
	//int num_features;
	float error_if_leaf;
	float predicted_value;
	int i_node,j_node;

	Node(vector<vector<float>> &train)
	{
		L = NULL;
		R = NULL;
		is_leaf = true;
		l1 = train.size;
		l2 = train[0].size;
		vector<float> error_column_node ;/////////////////
		for (int i=0;i<l1;i++)
		{
			error_column_node.push_back(l2-1][i])
		}
		predicted_value = mean(error_column_node,0,l1);
		error_if_leaf = sqrt(var(error_column_node,0,l1));
	}

	bool sortcol(const vector<float>& v1, const vector<float>& v2) 
	{
 		return v1[column] < v2[column];
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

	float val1 = sqrt(var(error_column,0,row));
	float val2 = sqrt(var(error_column,row+1,l1-1));
	//cout<<row<<" "<<val1<<" "<<val2<<endl;
	return (val1*(row + 1) + val2*(l1 - row - 1))/l1;
	}

	float calc_best_fit(vector< vector<float> > &train)  //train[0] will contain first training example with n features and final value , thus lenght n+1
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
			//cout<<i<<endl;
			float error = split(train,j);
			if(min_error > error)
			{
				x = i;
				y = train[j][i];
				min_error = error; 
			}
		}
	}
	i_node = x;
	y_node = y;
	return min_error;
	cout<<x<<" "<<y<<" "<<min_error<<endl;
	}

	void train_tree(vector<vector<float>> train_data)
	{
		float children_error = calc_best_fit(train_data);
		if(children_error >= error_if_leaf)
		{
			is_leaf = true;
			return;
		}
		else
		{
			is_leaf = false;
			column = i_node;
			sort(train_data.begin(),train_data.end(),sortcol);
			vector<vector<float> trainl,trainr;
			for (int r=0;r<=y_node;r++) trainl.push_back(train_data[r]);
			for (int r=y_node+1;r<train_data.size;r++) trainr.push_back(train_data[r]);	
			L = new Node(trainl);
			R = new Node(trainr);
			L->train_tree(trainl);
			R->train_tree(trainr);
			return;	
		}
	}

	float predict(vector<float> test)
	{
		if(is_leaf) return predicted_value;
		else if (test[i_node]<=y) return L->predict;
		else return R->predict;
	}

};	


int main()
{
	vector<float> v(5,0);
	vector<vector<float> > a(5,v);

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			a[i][j] = rand()%5 + 1;
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	calc_best_fit(a);

	return 0;
}