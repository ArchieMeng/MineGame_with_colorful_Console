#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"gui_by_system.h"

typedef struct record{
	struct tm time;//保存时间
	char name[20];//玩家名
	int secore;//得分
}record;

static record player[200];//存储玩家游戏记录的数组
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

int rank();
void value(record *dst , const char *usr_name, const int secore, struct tm *saved_time);
int data_in(char *usr_name, int secore , struct tm *saved_time);
int rec_cmp(const void*, const void*);

/*
功能：将记录数据加入到记录当中
*/
int data_in(char *usr_name , int secore , struct tm *saved_time)
{
	record temp;
	int sign;
	FILE *fp = fopen("history.bat","ab");
	if( fp != NULL )
	{
		value(&temp, usr_name, secore, saved_time);
		sign = fwrite(&temp, sizeof(record), 1, fp);
		fclose(fp);
	}
	else
	{
		return 0;
	}
	return 1;
}

/*
形参：要被赋值的结构体地址，需要赋值的结构体内容
功能：实现给一个record结构体赋值的功能
返回值：1为正常退出，0为异常退出
*/
void value(record *dst , const char *usr_name , const int secore , struct tm *saved_time)
{
	strcpy(dst->name, usr_name);
	dst->secore = secore;
	memcpy(&(dst->time), saved_time, sizeof(struct tm));
}

int rank()
{
	FILE *fp;
	size_t rec_size;//读取的记录总数
	size_t i = 0;
	fp = fopen("history.bat", "rb");

	if (fp != NULL)
	{
		rec_size = fread(player, sizeof(record), 200, fp);
		drawMainMenu(hout);
		gotoxy(hout, 3, 2);

		if (rec_size > 4)
		{
			qsort(player, rec_size, sizeof(record), rec_cmp);

			gotoxy(hout,3,2);
			printf("排行榜");

			gotoxy(hout, 3, 3);
			printf("名次\t玩家名称");

			gotoxy(hout, 41 , 3);//与“玩家名称”保持20个字符的距离
			printf("得分\t\t完成时间");

			for ( i = 0; i < 5; i++)
			{
				gotoxy(hout, 3, i + 4);
				printf("%d\t%s",i + 1 , player[i].name);

				gotoxy(hout, 41, i + 4);
				printf("%d\t%s",player[i].secore, asctime(&player[i].time));
			}
			fclose(fp);
		}
		else
		{
			if (rec_size != 0)
			{
				qsort(player, rec_size, sizeof(record), rec_cmp);

				gotoxy(hout, 3, 2);
				printf("排行榜");

				gotoxy(hout, 3, 3);
				printf("名次\t玩家名称");

				gotoxy(hout, 41, 3);//与“玩家名称”保持20个字符的距离
				printf("得分\t\t完成时间");

				for ( i = 0; i < rec_size; i++)
				{
					gotoxy(hout, 3, i + 4);
					printf("%d\t%s", i + 1, player[i].name);

					gotoxy(hout, 41, i + 4);
					printf("%d\t%s", player[i].secore, asctime(&player[i].time));
				}
				fclose(fp);
			}
			else
			{
				gotoxy(hout, 3, 6);
				printf("没有历史游戏记录\n");
			}
		}
		Sleep(1000);
		gotoxy(hout, 3, i + 4);
		printf("输入任意字符结束:\n");

		gotoxy(hout, 3, i + 5);

		getchar();
		getchar();
		return 1;
	}
	else
	{
		gotoxy(hout, 3, 6);
		printf("ERROR!!\n");
		Sleep((unsigned)3000);
		return 0;
	}

}

/*
功能：对record进行得分比较
*/
int rec_cmp(const void *pres, const void *next)
{
	if ( ((record*)next)->secore > ((record*)pres)->secore )
	{
		return 1;
	}
	if (((record*)next)->secore < ((record*)pres)->secore)
	{
		return -1;
	}
	return 0;
}