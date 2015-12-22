#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <malloc.h>
#include <string.h>
#define MAXVALUE  10000
#define N 128 //������ֻ��ascii����г��ֵ��ַ����롣
#define MAXBIT 128
#define MAXLEAF 128
#define MAXNODE MAXLEAF*2 -1
using namespace std;

typedef struct node1
{
	int weight;          //���ڵ��Ȩ�أ����ֵ�Ƶ�ʡ�
	int parent;			//���׽ڵ��Ȩ��
	int lch;			//����ӵ�Ȩ��
	int rch;			//�Ҷ��ӵ�Ȩ��
	int value;			//����ڵ������ַ���ascii��
}*HuffmanTree, HTNode;

typedef struct node2
{
	char bit[128];
}*HuffmanCode, HCNode;

void strRev(char *s)//�����ַ�����ת
{
	char temp, *endchar;
	endchar = s + strlen(s) - 1;
	while (endchar > s)
	{
		temp = *s;
		*s = *endchar;
		*endchar = temp;
		--endchar;
		++s;
	}
}

int count_not_zero(int *data, int n)//�����������в�Ϊ��ĸ���
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

void Select(HuffmanTree HT, int n, int *x1, int *x2)	//��HT[k](1 <= k <= m)��ѡ������˫��Ϊ0��Ȩֵ��С�Ľ�㣬���±�Ϊs1��s2��n=N
{
	int i, j;
	int m1, m2;
	for (i = 0; i < n; i++)
	{
		m1 = m2 = MAXVALUE;     /* m1��m2�д�������޸�����ҽ��Ȩֵ��С��������� */
								/* �ҳ����н����Ȩֵ��С���޸�����������㣬���ϲ�֮Ϊһ�Ŷ����� */
		for (j = 0; j < n + i; j++)
		{
			if (HT[j].weight < m1 && HT[j].parent == 0)
			{
				m2 = m1;
				x2 = x1;
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
}

/*ͳ��һ���ı��ļ��ĸ����ַ����ֵ�Ƶ�� */
void count(int *data)//data��һ�����飬ÿ��Ԫ�ص��±�����ַ���ascii��
{
	int i;
	char w;
	ifstream fin;
	for (i = 0;i < N;i++)                             //��ʼ��Ƶ��Ϊ0
		data[i] = 0;							//i�����ַ���ascii�롣
	fin.open("source.txt", ios::in);
	while (!fin.eof())                         //ѭ������ֱ���ļ�ĩβ                           
	{
		fin.get(w);
		data[(int)w]++;      //����Ƶ��              
	}
	fin.close();
}
/*2.�����ı����ֵ��ַ��ͳ��ֵ�Ƶ�Σ��������ı��ַ����Ĺ�������������ļ���ʽ�洢����ʾ�� */
//��1�������Ƚ���һ������������������������������ķ����ҵ�����Ȩֵ��С�ĵ㣬Ȼ�������µĵ㲢��ɾ��֮ǰ�������㣬Ȼ���ٶ��µĵ㼯��������
void CreatHuffmanTree(HuffmanTree HT, int n, int *data)
{
	int i;
	int j = 0;
	int s1, s2;
	for (i = 0; i<2 * n - 1; i++)            //��ʼ�����н���Ȩֵ��˫�ף����ӽ�㶼Ϊ0     
	{
		HT[i].parent = 0;
		HT[i].lch = 0;
		HT[i].rch = 0;
		//HT[j].weight = MAXVALUE;
	}
	for (i = 0;i < n;i++)
	{
		if (data[i])
		{
			HT[j].weight = data[i];          //Ϊǰn����Ԫ�е�Ҷ�ӽڵ㸳Ȩֵ
			HT[j].value = i;
			j++;							//��һ������֮��һ����j���ڵ�������Ȩֵ��
		}
	}

	for (i = j; i < 2*j-1; i++)             //ͨ��j-1�ε�ѡ��ɾ�����ϲ���������������
	{
		Select(HT, i, &s1, &s2);			// ��HT[k](1<=k<=m)��ѡ������˫��Ϊ0��Ȩֵ��С�Ľ�㣬���±�Ϊs1��s2
		HT[s1].parent = i;					// �õ��½��i,�������ҽ��Ϊs1,s2,ȨֵΪ����֮��
		HT[s2].parent = i;
		HT[i].lch = s1;
		HT[i].rch = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//return j;//������Ч�����������ַ��Ĺ��������ڵ����
}

//��2���ڽ������������֮�����ǾͿ������ɹ����������ˣ��������ѽ����Ĺ��������У���Ҷ��㿪ʼ���ؽ���˫��������˵�����㣬ÿ����һ�������߹��˹���������һ����֧���Ӷ��õ�һλ��������ֵ������һ���ַ��Ĺ����������ǴӸ���㵽��ӦҶ�����������·���ϸ���֧����ɵ�0��1���У�����ȵõ��ķ�֧����Ϊ�������ĵ�λ�룬��õ��ķ�֧����Ϊ�������ĸ�λ�룺

void CreatHuffmanCode(HuffmanTree HT, HuffmanCode HC, int n)//�����n������������λ����Ӧ�ú�HC.bit��λ��һ���ࡣ128��
{
	//char cd[n];
	int start, c, i;
	int f;
	char *cd;
	cd = (char*)malloc(n*sizeof(char));
	cd[n - 1] = '\0';
	for (i = 0;i < n; i++)
	{
		start = n - 1;                 //start��ʼʱָ����󣬼����������λ��
		c = i;
		f = HT[i].parent;                //fָ��c��˫�׽��
		while (f != 0)                     //��Ҷ�ӿ�ʼ���ϻ��ݣ�ֱ�������
		{
			--start;                     //����һ��startָ��ǰһ��λ��
			if (HT[f].lch == c)
				cd[start] = '0';             //c��f�����ӣ������ɴ��롮0��
			else
				cd[start] = '1';                 //c��f���Һ��ӣ������ɴ��롮1��
			c = f;                                 //  �������ϻ���
			f = HT[f].parent;
		}
		//HC[i].bit = new char[n - start];               //Ϊ��i���ַ��������ռ�
		strcpy_s(HC[i].bit, 128, &cd[start]);                 //����õı������ʱ�ռ�cd���Ƶ�HC�ĵ�ǰ����
	}
	delete cd;
	ofstream fout;
	fout.open("HTree.txt");
	for (i = 0;i < n;i++)
	{
		char ch = i;
		fout << ch << "\t" << HC[i].bit << endl;//���ı��ļ�����ʽ��ʾ
	}
	fout.close();
}

/*3.�������ַ����������ı��ļ��Ĺ����������ļ���
����һ���Ѿ�ʵ�֡�*/
//4.�Թ����������ļ��������롣 
void TranslateHuffmanCode(HuffmanTree HT, HCNode HC, int n)
{
	int f;
	char c;
	ifstream fin;
	fin.open("CodeFile.txt");
	ofstream fout;
	fout.open("RecieveFile.txt");
	f = 2 * n - 1;
	while (!fin.eof())  //�����ļ�δ
	{
		fin >> c;

		if (HT[f].lch == 0 && HT[f].rch == 0)         //����Ҷ�ӽ��ʱ����ַ�
		{
			char ch = f + 96;
			fout << ch;
			cout << c;                         //��ʾ���
			f = 2 * n - 1;                              //�Ӹ���㿪ʼ
		}
		if (c == '0')                              //��cΪ0�Ǵ���cΪf������
			f = HT[f].lch;
		else
			f = HT[f].rch;
	}
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
	CreatHuffmanTree(HT, j, data);//��k���ַ�,����������j���ڵ㡣
	HuffmanCode HC;
	HC = (HuffmanCode)malloc(k*sizeof(HTNode));
	CreatHuffmanCode(HT, HC, j);
}