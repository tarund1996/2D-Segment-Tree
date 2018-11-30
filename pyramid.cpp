#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<stack>
#include<queue>
#include<math.h>
#include<string.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

int tp = 1e9;
int m,n,a,b,c,d;
int INPUT[1003][1003],PSUM[1003][1003] = {0};
int segt[2050][2050];
int BIG[2050][2050],SMALL[2050][2050];
int rows,columns,temprows,tempcolumns;

// check whether C x D grid lies within given A x B grid
bool liesinside(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	if(x1>=x3 and x1<=x4 and y1>=y3 and y1<=y4 and x2>=x3 and x2<=x4 and y2>=y3 and y2<=y4)
		return 1;
	return 0;
}

//Finding the minimum sum of a C x D grid within an A x B grid
//Refer the attached 2-d segment tree working for detailed steps.
int RMQ(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int i,int j,bool cut)
{
	if(x1==x3 and x2==x4 and y1==y3 and y2==y4)
		return segt[i][j];
	if(y1==y3 and y2==y4)
		cut=1;
	else if(x1==x3 and x2==x4)
		cut=0;
	if(cut==0)
	{
		if(liesinside(x1,y1,x2,y2,x3,y3,x4,((y4+y3)/2)))
		{
			return RMQ(x1,y1,x2,y2,x3,y3,x4,(y4+y3)/2,i,(2*j)+1,1);
		}
		else if(liesinside(x1,y1,x2,y2,x3,((y3+y4)/2+1),x4,y4))
		{
			return RMQ(x1,y1,x2,y2,x3,((y3+y4)/2+1),x4,y4,i,(2*j)+2,1);
		}
		else
		{
			int min1 = RMQ(x1,y1,x2,(y4+y3)/2,x3,y3,x4,(y4+y3)/2,i,(2*j)+1,1);
			int min2 = RMQ(x1,(((y3+y4)/2)+1),x2,y2,x3,(((y3+y4)/2)+1),x4,y4,i,(2*j)+2,1);
			return min(min1,min2);
		}
	}
	else
	{
		if(liesinside(x1,y1,x2,y2,x3,y3,(x3+x4)/2,y4))
		{
			return RMQ(x1,y1,x2,y2,x3,y3,(x3+x4)/2,y4,(2*i)+1,j,0);
		}
		else if(liesinside(x1,y1,x2,y2,((x3+x4)/2)+1,y3,x4,y4))
		{
			return RMQ(x1,y1,x2,y2,((x3+x4)/2)+1,y3,x4,y4,(2*i)+2,j,0);
		}
		else
		{
			int min1 = RMQ(x1,y1,(x3+x4)/2,y2,x3,y3,(x3+x4)/2,y4,(2*i)+1,j,0);
			int min2 = RMQ(((x3+x4)/2)+1,y1,x2,y2,((x3+x4)/2)+1,y3,x4,y4,(2*i)+2,j,0);
			return min(min1,min2);
		}
	}
	return 0;
}

//Building the 2-D segment tree based on the C x D sums grid
void BUILD_SEGTREE()
{
	for(int i=0;i<columns;i++)
	{
		temprows = rows;
		while(((temprows)&(temprows-1))!=0)
		{
			SMALL[temprows][i] = tp;
			temprows++;
		}
	}
	
	for(int i=0;i<temprows;i++)
	{
		tempcolumns = columns;
		while(((tempcolumns)&(tempcolumns-1))!=0)
		{
			SMALL[i][tempcolumns] = tp;
			tempcolumns++;
		}
	}
	
	columns = tempcolumns;
	rows = temprows;
	
	for(int i=rows-1;i<(2*rows)-1;i++)
	{
		for(int j=columns-1;j<(2*columns)-1;j++)
			segt[i][j] = SMALL[i-(rows-1)][j-(columns-1)];
	}
	
	for(int i=rows-1;i<(2*rows)-1;i++)
	{
		for(int j=(2*columns)-2;j>0;j-=2)
			segt[i][(j/2)-1] = min(segt[i][j],segt[i][j-1]);
	}
		
	for(int j=0;j<(2*columns)-1;j++)
	{
		for(int i=(2*rows)-2;i>0;i-=2)
			segt[(i/2)-1][j] = min(segt[i][j],segt[i-1][j]);
	}
}

// Finding partial sums of the original matrix for O(1) retrieval
void COMPUTE_PARTIAL_SUM()
{
	//Computing partial sum along rows
	for(int i=1;i<=n;i++)
	{
		PSUM[i][1] = INPUT[i][1];
		for(int j=2;j<=m;j++)
			PSUM[i][j] = PSUM[i][j-1] + INPUT[i][j];
	}
	// Computing partial sums along all columns
	for(int i=1;i<=m;i++)
		for(int j=2;j<=n;j++)
			PSUM[j][i] = PSUM[j-1][i] + PSUM[j][i];
}
//clearing all arrays after each input is executed
void CLEAR_ARRAYS() 
{
	for(int i=0;i<1003;i++)
		for(int k=0;k<1003;k++) 
			INPUT[i][k]=0;
			
	for(int i=0;i<1002;i++)
		for(int k=0;k<1002;k++) 
			PSUM[i][k]=0;
			
	for(int i=0;i<2050;i++)
		for(int k=0;k<2050;k++) 
			BIG[i][k] = SMALL[i][k] = segt[i][k] = 0;
}
	
int main()
{
	_
	//Input
	ifstream input;
	input.open("demo.in");
	int no;
	input>>no;
	for(int iter=1;iter<=no;iter++) 
	{	
		cout<<"Test case "<<iter<<endl;
		
		CLEAR_ARRAYS();
		
		input>>m>>n>>a>>b>>c>>d;
		for(int i=1;i<=n;i++)
			for(int j=1;j<=m;j++)
				input>>INPUT[i][j];

		COMPUTE_PARTIAL_SUM();

		//Precomputing partial sums helps compute a square's sum in O(1)
		//A x B sums
		// x and y maintain the indices where the computed value is stored
		
		//BIG stores all possible A x B sums within the n X m array
		int x=0,y=0;
		for(int i=b;i<=n;i++)
		{
			for(int j=a;j<=m;j++)
			{
				BIG[x][y] = PSUM[i][j] + PSUM[i-b][j-a];
				BIG[x][y] = BIG[x][y] - PSUM[i-b][j];
				BIG[x][y] = BIG[x][y] - PSUM[i][j-a];
				y++;
			}
			x++;
			y = 0;
		}
	
		//C x D sums
		//SMALL stores all possible C x D sums within the n X m array
		x=0;
		y=0;
		
		for(int i=d;i<=n;i++)
		{

			for(int j=c;j<=m;j++)
			{
				SMALL[x][y] = PSUM[i][j] + PSUM[i-d][j-c];
				SMALL[x][y] = SMALL[x][y] - PSUM[i-d][j];
				SMALL[x][y] = SMALL[x][y] - PSUM[i][j-c];
				y++;
			}
			x++;
			y = 0;
		}
	
		rows = n-d+1; columns = m-c+1;
		temprows = rows;
		tempcolumns = columns;
	
		// building 2-D segment tree
		BUILD_SEGTREE();
	
		int res,temp,mx=-1;
		int check=0,limit1=0,limit2=0;
		pair<int,int> pos1,pos2;
	
		// pos2 holds the row and column value of the A x B array
		pos2.first = b-1;
		pos2.second = a-1;
	
		for(int i=0;i<n-b+1;i++)
		{
			for(int j=0;j<m-a+1;j++)
			{
				//limit1 and limit2 long with i and j represent the 
				// search space within which minimum is to be found in C xD
				limit1 = i+b-1-d;
				limit2 = j+a-1-c;		
			
				//The 5th, 6th, 7th and 8th parameters represent the entire C x D array
				// 9th and 10th parameters represent the tree traversal variables, both
				// of which are initially 0.
				// 11th parameter represents either a vertical or horizontal cut in the 
				// segment tree.
				
				res = RMQ(i+1,j+1,limit1,limit2,0,0,rows-1,columns-1,0,0,1);
			
				if(BIG[i][j] - res > mx)
				{
					mx = BIG[i][j] - res;
					pos1.first = i; //holds row number of the top left point of A x B rectangle.
					pos1.second = j; //hold column number of top left point of A x B rectangle.
					pos2.first = i+b-1; // holds row number of bottom right point A x B rectangle.
					pos2.second = j+a-1; // holds column number of bottom right point in A x B rectangle.
					temp = res; // temp holds the maximum value of BIG[i][j] - res found so far.
				} 
			}
		}
		// original array is 1-indexed -> adding one to the resultant coordinates
		// A x B result 
		cout<<pos1.second + 1<<" "<<pos1.first + 1<<endl;
	
		pair<int,int> cd_pos;
		check=0,limit1=0,limit2=0;
	
		limit1 = pos2.first - d;
		limit2 = pos2.second - c;
	
		// C x D result
		// pos1 and limit1 and limit2 define the search space in which the temp value needs to be found
		for(int p=pos1.first+1;p<=limit1;p++)
		{
			for(int q=pos1.second+1;q<=limit2;q++)	
			{
				if(SMALL[p][q] == temp)
				{
					cd_pos.first = p;// stores row number of the position where minimum C x D was found
					cd_pos.second = q; // stores column number of the position where minimum C x D was found.
					check=1; // if temp was found , break out of the loop
					break;
				}
			}
			if(check==1) break;
		}
		cout<<cd_pos.second + 1<<" "<<cd_pos.first + 1<<endl<<endl;
	}
	input.close();
	return 0;
}
	 
		

	
