/*
ɨ����Ϸ��
B14090412 
B14090434 ��֮��
B14090435 �����
����

�ر��л
������ƽӿ��ṩ�ߣ�ͬ�ô�ѧ 1452286��-�����
*/

/*
�������ʵ��
*/

#include<stdio.h>
#include<stdlib.h>
#include"function.h"
int main()
{
	int flag = 0 ;

	do{


		do{
			system("cls");
			flag = menu();			
		}while(flag != 2 && flag != 1);

		switch(flag)
		{
		case 1:play(); break;
		case 2:rank(); break;
		}

		do{
			system("cls");
			printf("�Ƿ��˳���Ϸ��:\n");
			printf("1:������Ϸ\n");
			printf("0:�˳�\n");
			
			scanf("%d",&flag);
			system("cls");
		}while( flag != 1 && flag != 0 );

	}while( flag != 0 );
	return 0;
}