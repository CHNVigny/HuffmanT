#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <malloc.h>
#include <string.h>
#include <string>
#define MAXVALUE  10000
#define N 128 //本程序只对ascii码表中出现的字符编码。
#define MAXBIT 128
#define MAXLEAF 128
#define MAXNODE MAXLEAF*2 -1
using namespace std;

typedef struct node1
{
	int weight;          //本节点的权重，出现的频率。
	int parent;			//父亲节点的权重
	int lch;			//左儿子的权重
	int rch;			//右儿子的权重
	int value;			//这个节点代表的字符的ascii码
}*HuffmanTree, HTNode;

typedef struct node2
{
	char bit[128];
	char value;
}*HuffmanCode, HCNode;

char* strRev(const char* str)
{
	size_t len = strlen(str);
	char* pTemp = new char[len + 1];
	memset(pTemp, 0, len + 1);
	memcpy(pTemp, str, len);
	char ch;
	for (size_t i = 0;i<len / 2;i++)
	{
		ch = pTemp[i];
		pTemp[i] = pTemp[len - 1 - i];
		pTemp[len - 1 - i] = ch;
	}
	return pTemp;
}

int count_not_zero(int *data, int n)//用于求数组中不为零的个数
{
	int i, j=0;
	for (i = 0;i < n;i++)
	{
		if (data[i])
		{
			j++;
		}
	}
	return j;
}

void Select(HuffmanTree HT, int n, int *x1, int *x2)	//在HT[k](1 <= k <= m)中选择两个双亲为0且权值最小的结点，记下标为s1，s2，n=N
{
	int j;
	int m1, m2;
	m1 = m2 = MAXVALUE;     /* m1、m2中存放两个无父结点且结点权值最小的两个结点 */
							/* 找出所有结点中权值最小、无父结点的两个结点，并合并之为一颗二叉树 */
	for (j = 0; j < n; j++)
	{
		if (HT[j].weight < m1 && HT[j].parent == 0)
		{
			m2 = m1;
			//x2 = x1;
			m1 = HT[j].weight;
			*x1 = j;
		}
		else if (HT[j].weight < m2 && HT[j].parent == 0)
		{
			m2 = HT[j].weight;
			*x2 = j;
		}
	}
}

/*统计一个文本文件的各个字符出现的频次 */
void count(int *data)//data是一个数组，每个元素的下标代表字符的ascii码
{
	int i;
	char w;
	ifstream fin;
	for (i = 0;i < N;i++)                             //初始化频率为0
		data[i] = 0;							//i代表字符的ascii码。
	fin.open("source.txt", ios::in);
	while (!fin.eof())                         //循环读入直至文件末尾                           
	{
		fin.get(w);
		data[(int)w]++;      //增加频率              
	}
	fin.close();
}
/*2.依此文本出现的字符和出现的频次，建立该文本字符集的哈夫曼编码表，以文件形式存储并显示。 */
//（1）我们先建立一个哈弗曼树，我们在这里运用排序的方法找到两个权值最小的点，然后生成新的点并且删除之前的两个点，然后再对新的点集进行排序。
void CreatHuffmanTree(HuffmanTree HT, int n, int *data)//这里的n代表共统计出了n个字符。
{
	int i;
	int j = 0;
	int s1, s2;
	for (i = 0; i<2 * n - 1; i++)            //初始化所有结点的权值，双亲，孩子结点都为0     
	{
		HT[i].parent = 0;
		HT[i].lch = 0;
		HT[i].rch = 0;
		//HT[j].weight = MAXVALUE;
	}
	for (i = 0;i < N;i++)
	{
		if (data[i])
		{
			HT[j].weight = data[i];          //为前n个单元中的叶子节点赋权值
			HT[j].value = i;
			j++;							//这一步完事之后一共有j个节点里面有权值。
		}
	}

	for (i = j; i < 2*j-1; i++)             //通过j-1次的选择、删除、合并来创建霍夫曼树
	{
		Select(HT, i, &s1, &s2);			// 在HT[k](1<=k<=m)中选择两个双亲为0且权值最小的结点，记下标为s1，s2
		HT[s1].parent = i;					// 得到新结点i,它的左右结点为s1,s2,权值为它们之和
		HT[s2].parent = i;
		HT[i].lch = s1;
		HT[i].rch = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//return j;//返回有效的真正代表字符的哈夫曼树节点个数
}

//（2）在建立完哈弗曼树之后我们就可以生成哈弗曼编码了，过程是已建立的哈夫曼树中，从叶结点开始，沿结点的双亲链域回退到根结点，每回退一步，就走过了哈夫曼树的一个分支，从而得到一位哈夫曼码值，由于一个字符的哈夫曼编码是从根结点到相应叶结点所经过的路径上各分支所组成的0，1序列，因此先得到的分支代码为所求编码的低位码，后得到的分支代码为所求编码的高位码：

void CreatHuffmanCode(HuffmanTree HT, HuffmanCode HC, int n)//n代表存了字符的HT的个数
{
	//char cd[n];
	int start, c, i;
	int f;
	char *cd1, *cd2;
	cd1 = (char*)malloc(n*sizeof(char));
	cd2 = (char*)malloc(n*sizeof(char));
	//cd[n - 1] = '\0';
	for (i = 0;i < n; i++)				//HT的前j个元素里面存了字符。
	{
		start = 0;                 //start开始时指向最前
		c = i;
		f = HT[c].parent;                //f指向c的双亲结点
		while (f != 0)                     //从叶子开始向上回溯，直到根结点
		{                   
			if (HT[f].lch == c)
				cd1[start] = '0';             //c是f的左孩子，则生成代码‘0’
			else
				cd1[start] = '1';                 //c是f的右孩子，则生成代码‘1‘
			c = f;                                 //  继续向上回溯
			f = HT[c].parent;
			start++;
		}
		cd1[start] = '\0';
		cout << cd1 << endl;
		cd2 = strRev(cd1);
		cout << cd2 << endl;
		for (int m = 0;m < 128;m++)
		{
			HC[i].bit[m] = -51;
		}
		//HC[i].bit = new char[n - start];               //为第i个字符编码分配空间
		strcpy_s(HC[i].bit, 128, cd2);//将求得的编码从临时空间cd复制到HC的当前行中
		HC[i].value = (char)HT[i].value;
	}
	free(cd1);
	free(cd2);
	ofstream fout;
	fout.open("HTree.txt");
	for (i = 0;i < n;i++)
	{
		char ch = HC[i].value;
		fout << ch << "\t" << HC[i].bit << endl;//以文本文件的形式显示
	}
	fout.close();
}

/*3.给出该字符集的任意文本文件的哈夫曼编码文件。*/
void Tranlate_To_HuffmanCode_File(HuffmanCode HC, int n)//n表示HC节点个数
{
	char w;
	ifstream fin;
	fin.open("source.txt", ios::in);
	ofstream fout;
	fout.open("result.txt");
	while (!fin.eof())                         //循环读入直至文件末尾                           
	{
		fin.get(w);								//读入字符到w中
		for (int i = 0;i < n;i++)
		{
			if (HC[i].value == w)
			{
				fout << HC[i].bit;
				break;
			}
		}
	}
	fin.close();
	fout.close();
}
//4.对哈夫曼编码文件进行译码。 
void TranslateHuffmanCode(HuffmanTree HT, HuffmanCode HC, int n)//n表示 HT节点的个数。
{
	int f, j=0, k;//j记录所有节点的祖先的位置，k记录祖先节点的个数。
	char c;
	ifstream fin;
	fin.open("result.txt");
	ofstream fout;
	fout.open("translation.txt");
	for (int i = 0;i < n;i++)
	{
		if (HT[i].parent = 0)
		{
			j = i;
			k++;
		}
	}
	while (!fin.eof())  //读到文件未
	{
		f = j;                              //从根结点开始
		//fin >> c;
		while (HT[f].lch != 0 && HT[f].rch != 0 && !fin.eof())
		//if ()         //到达叶子结点时输出字符
		{
			fin.get(c);
			if (c == '0')                              //当c为0是代表c为f的左孩子
				f = HT[f].lch;
			else
				f = HT[f].rch;
			
		}
		fout << HT[f].value;                       //显示结果
	}
	fin.close();
	fout.close();
}

int main()
{
	int *data, j, k;
	HuffmanTree HT;

	data = (int*)malloc(N*sizeof(int));
	count(data);
	k = count_not_zero(data, N);
	j = 2 * k - 1;
	HT = (HuffmanTree)malloc(j*sizeof(HTNode));
	CreatHuffmanTree(HT, k, data);//有k个字符,哈夫曼树有j个节点。
	HuffmanCode HC;
	HC = (HuffmanCode)malloc(k*sizeof(HTNode));
	CreatHuffmanCode(HT, HC, k);
	Tranlate_To_HuffmanCode_File(HC, k);
	TranslateHuffmanCode(HT, HC, j);
}