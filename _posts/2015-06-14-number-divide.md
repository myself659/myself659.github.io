---
layout: post
title: "实现无符号整型数的分解"
categories:
- 
tags:
- 


---

###题目
将一个无符号数N拆分为不多于M个数，使拆分的数之和等于N,条件：


1. N >= M；
2. N与M都是无符号整型数

求：一共有多少中拆分方法？

### 分析
参考代码

### 代码
代码实现计算拆分方法及打印拆分组合信息

```c++

#include <iostream>
#include <stdint.h>

using namespace std;


class CSolution
{
	private:
		uint32_t uiM;
		uint32_t uiN;
		uint32_t uiCount;
		uint32_t *puiRecord;
	public:
		CSolution();
		CSolution(uint32_t uiM, uint32_t uiN);
		~CSolution();
		uint32_t GetCount()  {return uiCount;}
		void PrintCombinations();
	
		uint32_t CountNum(uint32_t uiM, uint32_t uiN);	
		void GetCombination(uint32_t uiMin,
							uint32_t uiLeft,  
							uint32_t uiGetNum,   
							uint32_t uiTargetNum,  
							uint32_t *puiRecord);
};


CSolution::CSolution(uint32_t uiM,uint32_t uiN){
	this->uiM = uiM;
	this->uiN = uiN;
	this->puiRecord = new unsigned int [uiM];
	this->uiCount =  CountNum(uiM, uiN);
	return ;
}


CSolution::CSolution()
{
	uiM = 1;
	uiN = 1;
	puiRecord = new unsigned int [uiM];
	uiCount =  CountNum(uiM, uiN);

	return  ;
}

CSolution::~CSolution()
{
	delete [] puiRecord;
	return ;
}



uint32_t CSolution::CountNum(uint32_t uiM, uint32_t uiN)
{

	if((0 == uiM) || (0 == uiN) || ( 1 == uiM) || ( 1 == uiN))
	{
		return 1;
	}


	if(uiN < uiM)
	{
		return CountNum(uiN,uiN);
	}
	
	return CountNum(uiM - 1, uiN) + CountNum(uiM, uiN - uiM);
}	

/*
UINT uiMin    			最小值
UINT uiLeft     		剩下值 
UINT uiGetNum      		已拆分元素个数 
UINT uiTargetNum	 	需要拆分的个数
UINT *puiCombination  	拆分组合首地址 
*/
void CSolution::GetCombination(uint32_t uiMin,
							  uint32_t uiLeft,  
							  uint32_t uiGetNum,   
							  uint32_t uiTargetNum,  
							  uint32_t *puiRecord) 
{
	uint32_t i;
	uint32_t j;

	/*递归结束条件  */
	if(1 == uiTargetNum) 
	{
		cout << uiLeft<<endl;
		return ;
	}

	/* 将剩下值拆分为多个数,除了最后一个拆分值，其他的拆分数都应小于等于uiLeft/2 */
	for(i = uiMin; i <= uiLeft / 2; i++)
	{
		puiRecord[uiGetNum] = i;
		uiGetNum++;
		if(uiGetNum + 1 == uiTargetNum)
		{
			puiRecord[uiGetNum] = uiLeft - i; 
			for(j = 0; j  < uiTargetNum; j++)
			{
				cout <<puiRecord[j]<< " ";
			} 
			cout << endl;
		}
		else
		{
			GetCombination(i, uiLeft - i, uiGetNum, uiTargetNum, puiRecord);
		}
		
		uiGetNum--;
	}
		
	return  ;				
}


void  CSolution::PrintCombinations()
{
	uint32_t i;
	
	cout<<"print combinations as follow:"<<endl;
	
	for(i = 1; i  <=  uiM; i++)
	{
		GetCombination(1, uiN, 0, i, puiRecord);	
	}
	cout<<"The Total num is "<< GetCount()<<endl;
	
	return ;
}



int main(void)
{
	CSolution test(7, 11);

	
	test.PrintCombinations();

	return 0;
}



```
### 运行结果
```C++
[root@localhost cpp]# ./numdecom
print combinations as follow:
11
1 10 
2 9 
3 8 
4 7 
5 6 
1 1 9 
1 2 8 
1 3 7 
1 4 6 
1 5 5 
2 2 7 
2 3 6 
2 4 5 
3 3 5 
3 4 4 
1 1 1 8 
1 1 2 7 
1 1 3 6 
1 1 4 5 
1 2 2 6 
1 2 3 5 
1 2 4 4 
1 3 3 4 
2 2 2 5 
2 2 3 4 
2 3 3 3 
1 1 1 1 7 
1 1 1 2 6 
1 1 1 3 5 
1 1 1 4 4 
1 1 2 2 5 
1 1 2 3 4 
1 1 3 3 3 
1 2 2 2 4 
1 2 2 3 3 
2 2 2 2 3 
1 1 1 1 1 6 
1 1 1 1 2 5 
1 1 1 1 3 4 
1 1 1 2 2 4 
1 1 1 2 3 3 
1 1 2 2 2 3 
1 2 2 2 2 2 
1 1 1 1 1 1 5 
1 1 1 1 1 2 4 
1 1 1 1 1 3 3 
1 1 1 1 2 2 3 
1 1 1 2 2 2 2 
The Total num is 49
```