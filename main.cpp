/*
扫雷游戏：
B14090412 
B14090434 蓝之怿
B14090435 蒙宇韬
制作

特别感谢
界面绘制接口提供者：同济大学 1452286数-朱可仁
*/

/*
主体程序实现
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
			printf("是否退出游戏？:\n");
			printf("1:继续游戏\n");
			printf("0:退出\n");
			
			scanf("%d",&flag);
			system("cls");
		}while( flag != 1 && flag != 0 );

	}while( flag != 0 );
	return 0;
}