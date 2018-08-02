#include <iostream>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <vector>
#include <limits.h>
#include <cmath>
#include <math.h>
#include <fstream>

using namespace std;

int column;

float mean(vector<float> &v,int start,int end)
{
	float mean = 0.0;
	//cout<<"start and end"<<start<<" "<<end<<endl;
	for(int k=start;k<end+1;k++) mean += v[k]/(end-start+1);
	return mean;	
}

float var(vector<float> &v,int start,int end)
{
	float mean_curr = mean (v,start,end);
	//	cout <<"mean"<<mean<<endl;
	float var_curr = 0.0;
	for(int k=start;k<end+1;k++) var_curr += ((v[k]-mean_curr)*(v[k]-mean_curr))/(end-start+1);
	return var_curr;	

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
	vector<vector< float> > node_data;

	Node(vector<vector<float> > &train)
	{
		int l1 = train.size();
		int l2 = train[0].size();
		//node_data = train;
		L = NULL;
		R = NULL;
		is_leaf = true;
		//cout<<"reached here";
		int rows = train.size();
		int cols = train[0].size();
		for(int r=0;r<rows;r++)
		{	vector<float> temp;
			for(int c=0;c<cols;c++)
			{
				temp.push_back(train[r][c]);
			}
			node_data.push_back(temp);
		}
		// cout << l2;
		vector<float> error_column_node ;/////////////////
		for (int i=0;i<l1;i++)
		{
			error_column_node.push_back(train[i][l2-1]);
		}
		predicted_value = mean(error_column_node,0,l1-1);
		//cout<<predicted_value;
		error_if_leaf = sqrt(var(error_column_node,0,l1-1));
		//cout<<"reached here";
	}

	static bool sortcol(const vector<float>& v1, const vector<float>& v2) 
	{
 		return v1[column] < v2[column];
	}

	float split(vector<vector<float> > train,int row)
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

	float calc_best_fit()  //train[0] will contain first training example with n features and final value , thus lenght n+1
	{
	int l1 = node_data.size();
	int l2 = node_data[0].size();
	int x,y;
	float min_error = INT_MAX;

	for(int i=0;i<l2-1;i++)
	{
		column = i;
		sort(node_data.begin(),node_data.end(),sortcol);
		for(int j=0;j<l1;j++)
		{
			//cout<<i<<endl;
			float error = split(node_data,j);
			if(min_error > error)
			{
				x = i;
				y = node_data[j][i];
				min_error = error; 
			}
		}
	}
	i_node = x;
	j_node = y;
	return min_error;
	//cout<<x<<" "<<y<<" "<<min_error<<endl;
	}

	void train_tree()
	{	
		vector<vector<float> > train_data;
		//cout<<"chalu";
		int rows = node_data.size();
		if(rows == 1)
		{
			is_leaf = true;
			return;
		}
		int cols = node_data[0].size();
		for(int r=0;r<rows;r++)
		{	vector<float> temp;
			for(int c=0;c<cols;c++)
			{
				temp.push_back(node_data[r][c]);
			}
			train_data.push_back(temp);
		}
		float children_error = calc_best_fit();
		if(children_error >= error_if_leaf)
		{
			//cout<<"shit";
			is_leaf = true;
			return;
		}
		else
		{
			is_leaf = false;
			column = i_node;
			sort(train_data.begin(),train_data.end(),sortcol);
			vector<vector<float> > trainl;
			vector<vector<float> > trainr;
			//cout<<j_node;
			//cout<<train_data.size();
			for(int h=0;h<train_data.size();h++){
				if(train_data[h][i_node]<=j_node) trainl.push_back(train_data[h]);
				else trainr.push_back(train_data[h]);
			}
			int x1 = trainl.size();
			int x2 = trainr.size();
			if(x1==0 or x2==0) {is_leaf=true;return;}
			L = new Node(trainl);
			R = new Node(trainr);
		// 	for(int j=0;j<trainl.size();j++)
		// 		{for (int k=0;k<L->node_data[0].size();k++)cout<<L->node_data[j][k]<<" ";
		// cout<<endl;}
		// 	cout<<L->predicted_value<<endl;
		// 	cout<<L->calc_best_fit()<<endl;
		// 	cout<<L->j_node;
		// 	cout<<L->i_node;
			L->train_tree();
			R->train_tree();
			return;	
		}
	}

	float predict(vector<float> test)
	{
		if(is_leaf) return predicted_value;
		else if (test[i_node]<=j_node) return L->predict(test);
		else return R->predict(test);	
	}

};	


// vector<vector<float> > readfile()
// {	
// }

int main()
{	// readfile();
	vector<float> v(3,0);
	vector<vector<float> > a(100,v);

	for(int i=0;i<100;i++)
	{
		for(int j=0;j<3;j++)
		{	if(j<2){
			a[i][j] = rand()%4 + 1;}
			else
				a[i][j] = a[i][j-1]*a[i][j-2];

			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	Node root(a);
	// cout<<mean(a[0],0,4)<<endl;
	vector <float> l;l.push_back(2);l.push_back(3);
	root.train_tree();
	cout<<root.predict(l)<<endl;
	return 0;
}