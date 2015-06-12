/*
��Ϸ���⹦�ܺ���ʵ��
*/

#include<cassert>
#include"rank_list.h"
#define point_size int
#define count_size int
#define sign_size char
#define lei_rate ( sqrt( ful_point ) + 4 )


typedef struct point{
	int value;//�õ��ֵ��-1��ʾ�е��ף�����������ʾ��Χ�ĵ�����
	int state;//�õ����ʾ״̬,-1Ϊ����ǣ�0Ϊ����״̬��1Ϊ��ʾ
} point ;

static point *array = NULL;//���ڴ洢��ͼ����״����������
static int ful_point = 0;//�洢ͼ�ϵĵ������
static int lei_num;//��������
static int lei_true;//��ȷ��ǵĵ�����
static int lei_signed;//��ұ�ǵĵ�����
static int secore;//��ҷ���
static int R;//����
static int L;//����
static char name[20];//�����
static char state = 1;//�洢����״̬ 1Ϊ�������У�0Ϊ�����˳���-1Ϊ������� , -2Ϊ�ȵ�����
extern HANDLE hout;

void one_array_fun(point *ptrp, int loc , void (*fun)( point *p , int near_by ) );

int menu()
{
	int sign;
	state = 1;
	lei_signed = 0;
	lei_true = 0;

	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	drawMainMenu(hout);

	gotoxy(hout, 2, 2);
	printf("1:��ʼ��Ϸ\n");

	gotoxy(hout, 2, 3);
	printf("2:�鿴���а�\n");

	gotoxy(hout, 2, 4);
	printf("���������ѡ��: ");
	scanf("%d",&sign);
	return sign;
}

/*
���ܣ������Ƿ����еķǵ��׵㶼����ʾ,������򷵻�1�����򷵻�0
*/
int count_showed()
{
	int i = 0;
	int showed_num = 0;
	for (i = 0; i < ful_point; i++)
	{
		if (array[i].state == 1)
		{
			showed_num++;
		}
	}
	if (showed_num == ful_point - lei_num)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
�����β�
�������ܣ�ȷ��ptrλ�ò����ף���ʹarray[loc]��״̬��Ϊ��ʾ
*/
void expose( point *ptr, int loc )
{	
	assert(ptr->value != -1);
	if (array[loc].state != 1)
	{
		if (array[loc].value == 0 && ptr != array + loc )
		{
			one_array_fun(array, loc, expose);
		}
		else
		{
			array[loc].state = 1;
		}
	}
}

/*
  ptrpΪ��ǰ�����������Ա     tempΪ(ptrp��Χ��)��ʱ��ȫ�������±�(��ʱ�ó������Ƚϵ�)
*/
void set_num(point *ptrp, int temp )//���÷ǵ��׵����ֵ
{
	if ( array[temp].value == -1)//�������
	{
		ptrp->value++;
	}
}

/*
��������one_array_fun
����������һ��point�ṹ�����鿪ͷ��һ����Ҫ������һά����λ�õ��±ꣻһ�������ˣ��Ը�λ�þŹ���������еĲ����ģ��β�Ϊ����ǰ����Ԫ�أ��Ա�Ԫ�ص��±꣩�ĺ���
�������ܣ���ÿ����ṹ����Ԫ�ػ���ΧԪ�ؽ��в���
*/
void one_array_fun( point *ptrp, int loc , void (*fun)( point *p_this , int loc_nearby ) )
{
	int i,j;
	int temp;//��ʱ����

	fun(&ptrp[loc], loc);//���ò�������,�ȶ�����λ�ý��в���

	for ( i = -1; i <= 1; i++)//��������
	{
		for ( j = -1; j <= 1; j++)//��������
		{
			temp = loc + i * R + j; 
			if ( temp >= 0 && temp <= ful_point  && (temp / L == loc / L + i ) && temp != loc)//����±�û��Խ��,����Խ��(ʵ����temp��һ�����ѭ����ֻ����һ���ڲ���),�Ҳ�������Ԫ�ؽ��в���
			{
				fun(&ptrp[loc], temp);//���ò�������
			}	
		}
	}
}

/*
���ܣ��ڳ���Ϊlen��int���͵�array�����ڲ���value
*/
int search( int value ,const int array[],const int len)
{
	int i;
	for ( i = 0; i < len; i++)
	{
		if (array[i] == value)
		{
			return 1;
		}
	}
	return 0;
}

/*
�βΣ���ͼ������������
���ܣ���ʼ����ͼ���飬�����µ���
*/
void map( const int row , const int line )
{
	int i = 0;
	int j = 0;
	int *temp = NULL;
	array = ( point* )calloc( row * line , sizeof(point) );//��ʼ����ͼ����ĳ���

	if( array != NULL )
	{
		ful_point = row * line;
		lei_num = ful_point / lei_rate;//��ʼ������
		temp = (int*)calloc( lei_num , sizeof(int));
		R = row;
		L = line;
		srand(( unsigned int ) time( 0 ) );
		for( i = 0 ; i < lei_num ; i++ )//��ʼ������
		{
			do
			{
				temp[i] = rand() % ful_point ;//������׵��±�
			} while (search(temp[i] , temp , i));

			array[ temp[i] ].value = -1;
		}

		for( i = 0 ; i < ful_point ; i++ )//��ʼ������������
		{
			array[i].state = 0;//��ʼ�������״̬
			if (array[i].value == -1)
			{
				continue;
			}

			one_array_fun( array , i , set_num );//�������ø÷ǵ��׵ĵ����ֵ
		}

	}

	else//�����ʼ��ʧ��
	{
		state = -1;
	}
}

/*
���ܣ������ѡ����Ϸ�ѶȲ���ʼ����ͼ
*/
void setting()
{
	int sign;
	int r = 0;
	int l = 0;
	system("cls");

	do{
		drawMainMenu(hout);

		gotoxy(hout, 2, 2);
		printf("С���ӣ�Ҫ������:");

		gotoxy(hout, 2, 3);
		printf("1:�� 2���е� 3������ 4���Զ���(����Ҫ��10����)");

		gotoxy(hout, 2, 4);
		scanf("%d", &sign);


		if (sign == 4)
		{
			do
			{
				gotoxy(hout, 2, 5);
				printf("����������: ");
				scanf("%d",&r);
				gotoxy(hout, 2, 6);
				printf("����������: ");
				scanf("%d", &l);
			} while ( r * l < 10 );
		}

		gotoxy(hout, 2, 5);
		switch (sign)
		{
		case 1:printf("С���ӣ���ɵ�Ǳ��ѶȰ���\n"); map(4, 4); break;
		case 2:printf("С���ӣ��������Ͳ�������\n"); map(6, 6); break;
		case 3:printf("���꣬����ó����\n"); map(10, 10); break;
		case 4:gotoxy(hout, 2, 7); printf("���Լ�����������������ɻ�\n"); map(r, l); break;
		default:printf("���ܲ��ܺú�����...");
		}

		Sleep(1000);
	} while (sign > 4 || sign < 1);
}

/*
���ܣ��ѵ�ͼ���
*/
void print_map()
{
	int i, j;
	system("cls");

	drawSingleBoard(hout, L , R );
	gotoxy(hout, 1 , 0 );
	printf("����������%-3d\t��ұ�ǵ�����:%-3d", lei_num, lei_signed);

	for (i = 0; i < R; i++)//�б���
	{
		for (j = 0; j < L; j++)//�б���
		{
			if (state == 1)//��Ϸ���ͼ
			{
				if (array[i * R + j].state != 0)//Ҫ����ʾ�ĵ�����
				{
					if (array[i * R + j].state == -1)//����ǵĵ�����
					{
						drawNum(hout, j, i , '@' , YELLOW );
					}
					else
					{
						assert(array[i * R + j].value != -1);
						if (array[i * R + j].value != 0) //��ֵ�����
						{
							drawNum(hout, j, i, array[i * R + j].value, WHITE);
						}
						else//��Χû���׵ĵ�����
						{
							drawNum(hout, j, i, ' ' , WHITE);
						}
					}
				}

				else//���ص�����
				{
					drawNum(hout, j, i, ' ' , GRAY );
				}

			}

			else//��Ϸ�������ͼ
			{
				if (array[i * R + j].value == -1)
				{
					drawNum(hout, j, i, '*' , RED );
				}
				else
				{
					if (array[i * R + j].value != 0) //��ֵ�����
					{
						drawNum(hout, j, i, array[i * R + j].value, WHITE);
					}
					else//��Χû���׵ĵ�����
					{
						drawNum(hout, j, i, ' ', WHITE);
					}
				}
			}
			
		}//��ѭ����β
	}//��ѭ����β
}

/*
���ܣ���ȡ��ҵ�������Ϣ����������Ӧ.�����Ҳȵ����ף��򷵻�-2������1
*/
char act_to_input()
{
	int judge = 0, x = 1 , y = 1;

	gotoxy(hout,0, R * 6 + 2);

	setcolor(hout,WHITE, BLACK);
	printf("������ĸ��ǵ����أ�: \n");
	printf("����ʾ����\n");
	printf("1(1,1)��ʾ��ǵ�һ�е�һ�е�Ϊ����\n0(1,1)��ʾȷ����һ�е�һ�е���û�е��׵�  \n(�����ո񣬲��������ĵı�����)\n");

	scanf("%d(%d,%d)", &judge, &x, &y);
	do{
		if (judge != 1 && judge != 0 || x > R + 1 || y > L + 1 || x <= 0 || y <= 0 || array[ ( x - 1 ) * R + y - 1 ].state == 1)
		{
			printf("����������������룡\n");
		}
		else
		{
			break;
		}
		scanf("%d(%d,%d)", &judge, &x, &y);
	} while ( true );

	x--;//������ʵ��������������ת��Ϊ�����±�Ĺ���
	y--;

	if (judge == 1)
	{
		array[x * R + y].state = -1;
		lei_signed++;
		if ( array[x * R + y].value == -1)
		{
			lei_true++;
		}
		return 1;
	}
	else
	{
		switch (array[x * R + y].value)
		{
		case -1: return -2;
		case 0:one_array_fun(array, x * R + y, expose); return 1;//��Χ���׵ĵ����ʾ
		default: expose(array + x * R + y, x * R + y);  return 1;//��Χ���׵����ʾ
		}
	}
}

char play()
{
	int time_dur;//��Ϸ����ʱ��
	int t1, t2;
	time_t timer;
	struct tm *time_now;
	setting();

	t1 = clock();
	if (state != -1)
	{
		do
		{
			print_map();
			state = act_to_input();
			if (count_showed() )
			{
				state = 0;
				print_map();
				setcolor(hout, WHITE, BLACK);
				gotoxy(hout, 0, R * 6 + 2);
				printf("��Ӯ�ˣ���\n");
			}
			else if (state == -2)
			{
				print_map();
				gotoxy(hout, 0, R * 6 + 3);
				setcolor(hout, WHITE, BLACK);
				printf("�����������");
			}
		} while (state == 1);

		free(array);

		t2 = clock();
		time_dur =  (int) ((double)(t2 - t1) / CLOCKS_PER_SEC);
		if (state == 0)
		{
			secore = 100000 / time_dur * lei_rate ;
		}
		else
		{
			secore = 0;
		}
		timer = time(NULL);
		time_now = localtime(&timer);

		Sleep(3000);
		gotoxy(hout, 0, R * 6 + 3);
		printf("��ҷ�����%d ��ʱ:%d\n",secore,time_dur);
		printf("�������������:(����20���ַ�)\n");
		scanf("%20s", name);
		putchar('\n');
		system("cls");

		data_in(name, secore, time_now );
		rank();
		return 0;
	}
	else
	{
		printf("�������!\n");
		return 1;
	}
} 