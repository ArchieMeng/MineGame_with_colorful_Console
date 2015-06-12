#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"gui_by_system.h"

typedef struct record{
	struct tm time;//����ʱ��
	char name[20];//�����
	int secore;//�÷�
}record;

static record player[200];//�洢�����Ϸ��¼������
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

int rank();
void value(record *dst , const char *usr_name, const int secore, struct tm *saved_time);
int data_in(char *usr_name, int secore , struct tm *saved_time);
int rec_cmp(const void*, const void*);

/*
���ܣ�����¼���ݼ��뵽��¼����
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
�βΣ�Ҫ����ֵ�Ľṹ���ַ����Ҫ��ֵ�Ľṹ������
���ܣ�ʵ�ָ�һ��record�ṹ�帳ֵ�Ĺ���
����ֵ��1Ϊ�����˳���0Ϊ�쳣�˳�
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
	size_t rec_size;//��ȡ�ļ�¼����
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
			printf("���а�");

			gotoxy(hout, 3, 3);
			printf("����\t�������");

			gotoxy(hout, 41 , 3);//�롰������ơ�����20���ַ��ľ���
			printf("�÷�\t\t���ʱ��");

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
				printf("���а�");

				gotoxy(hout, 3, 3);
				printf("����\t�������");

				gotoxy(hout, 41, 3);//�롰������ơ�����20���ַ��ľ���
				printf("�÷�\t\t���ʱ��");

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
				printf("û����ʷ��Ϸ��¼\n");
			}
		}
		Sleep(1000);
		gotoxy(hout, 3, i + 4);
		printf("���������ַ�����:\n");

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
���ܣ���record���е÷ֱȽ�
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