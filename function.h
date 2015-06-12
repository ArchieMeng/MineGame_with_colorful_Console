/*
游戏主题功能函数实现
*/

#include<cassert>
#include"rank_list.h"
#define point_size int
#define count_size int
#define sign_size char
#define lei_rate ( sqrt( ful_point ) + 4 )


typedef struct point{
	int value;//该点的值，-1表示有地雷，其他正数表示周围的地雷数
	int state;//该点的显示状态,-1为被标记，0为隐藏状态，1为显示
} point ;

static point *array = NULL;//用于存储地图各点状况的数组名
static int ful_point = 0;//存储图上的点的总数
static int lei_num;//地雷总数
static int lei_true;//正确标记的地雷数
static int lei_signed;//玩家标记的地雷数
static int secore;//玩家分数
static int R;//行数
static int L;//列数
static char name[20];//玩家名
static char state = 1;//存储程序状态 1为正常进行，0为正常退出，-1为程序错误 , -2为踩到地雷
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
	printf("1:开始游戏\n");

	gotoxy(hout, 2, 3);
	printf("2:查看排行榜\n");

	gotoxy(hout, 2, 4);
	printf("请输入你的选择: ");
	scanf("%d",&sign);
	return sign;
}

/*
功能：计算是否所有的非地雷点都被显示,如果是则返回1，否则返回0
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
函数形参
函数功能：确定ptr位置不是雷，并使array[loc]的状态变为显示
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
  ptrp为当前操作的数组成员     temp为(ptrp周围的)临时的全局数组下标(临时拿出来做比较的)
*/
void set_num(point *ptrp, int temp )//设置非地雷点的数值
{
	if ( array[temp].value == -1)//如果有雷
	{
		ptrp->value++;
	}
}

/*
函数名：one_array_fun
函数参数：一个point结构的数组开头；一个需要操作的一维数组位置的下标；一个定义了，对该位置九宫格区域进行的操作的，形参为（当前中心元素，旁边元素的下标）的函数
函数功能：对每个点结构数组元素或周围元素进行操作
*/
void one_array_fun( point *ptrp, int loc , void (*fun)( point *p_this , int loc_nearby ) )
{
	int i,j;
	int temp;//临时变量

	fun(&ptrp[loc], loc);//调用操作函数,先对中心位置进行操作

	for ( i = -1; i <= 1; i++)//控制行数
	{
		for ( j = -1; j <= 1; j++)//控制列数
		{
			temp = loc + i * R + j; 
			if ( temp >= 0 && temp <= ful_point  && (temp / L == loc / L + i ) && temp != loc)//如果下标没有越界,或者越行(实际上temp在一个外层循环内只能在一行内操作),且不对中心元素进行操作
			{
				fun(&ptrp[loc], temp);//调用操作函数
			}	
		}
	}
}

/*
功能：在长度为len的int类型的array数组内查找value
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
形参：地图的行数和列数
功能：初始化地图数组，并埋下地雷
*/
void map( const int row , const int line )
{
	int i = 0;
	int j = 0;
	int *temp = NULL;
	array = ( point* )calloc( row * line , sizeof(point) );//初始化地图数组的长度

	if( array != NULL )
	{
		ful_point = row * line;
		lei_num = ful_point / lei_rate;//初始化雷数
		temp = (int*)calloc( lei_num , sizeof(int));
		R = row;
		L = line;
		srand(( unsigned int ) time( 0 ) );
		for( i = 0 ; i < lei_num ; i++ )//初始化雷区
		{
			do
			{
				temp[i] = rand() % ful_point ;//获得有雷点下标
			} while (search(temp[i] , temp , i));

			array[ temp[i] ].value = -1;
		}

		for( i = 0 ; i < ful_point ; i++ )//初始化非雷区数字
		{
			array[i].state = 0;//初始化各点的状态
			if (array[i].value == -1)
			{
				continue;
			}

			one_array_fun( array , i , set_num );//用于设置该非地雷的点的数值
		}

	}

	else//数组初始化失败
	{
		state = -1;
	}
}

/*
功能：让玩家选择游戏难度并初始化地图
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
		printf("小伙子，要玩心马？:");

		gotoxy(hout, 2, 3);
		printf("1:简单 2：中等 3：困难 4：自定义(至少要有10个点)");

		gotoxy(hout, 2, 4);
		scanf("%d", &sign);


		if (sign == 4)
		{
			do
			{
				gotoxy(hout, 2, 5);
				printf("请输入行数: ");
				scanf("%d",&r);
				gotoxy(hout, 2, 6);
				printf("请输入列数: ");
				scanf("%d", &l);
			} while ( r * l < 10 );
		}

		gotoxy(hout, 2, 5);
		switch (sign)
		{
		case 1:printf("小伙子，这傻是比难度啊！\n"); map(4, 4); break;
		case 2:printf("小伙子，不作死就不会死！\n"); map(6, 6); break;
		case 3:printf("尼玛，大神好崇拜你\n"); map(10, 10); break;
		case 4:gotoxy(hout, 2, 7); printf("你自己玩好啦，自作死不可活\n"); map(r, l); break;
		default:printf("你能不能好好输入...");
		}

		Sleep(1000);
	} while (sign > 4 || sign < 1);
}

/*
功能：把地图输出
*/
void print_map()
{
	int i, j;
	system("cls");

	drawSingleBoard(hout, L , R );
	gotoxy(hout, 1 , 0 );
	printf("地雷数量：%-3d\t玩家标记地雷数:%-3d", lei_num, lei_signed);

	for (i = 0; i < R; i++)//行遍历
	{
		for (j = 0; j < L; j++)//列遍历
		{
			if (state == 1)//游戏输出图
			{
				if (array[i * R + j].state != 0)//要求显示的点的输出
				{
					if (array[i * R + j].state == -1)//被标记的点的输出
					{
						drawNum(hout, j, i , '@' , YELLOW );
					}
					else
					{
						assert(array[i * R + j].value != -1);
						if (array[i * R + j].value != 0) //有值点输出
						{
							drawNum(hout, j, i, array[i * R + j].value, WHITE);
						}
						else//周围没地雷的点的输出
						{
							drawNum(hout, j, i, ' ' , WHITE);
						}
					}
				}

				else//隐藏点的输出
				{
					drawNum(hout, j, i, ' ' , GRAY );
				}

			}

			else//游戏结束输出图
			{
				if (array[i * R + j].value == -1)
				{
					drawNum(hout, j, i, '*' , RED );
				}
				else
				{
					if (array[i * R + j].value != 0) //有值点输出
					{
						drawNum(hout, j, i, array[i * R + j].value, WHITE);
					}
					else//周围没地雷的点的输出
					{
						drawNum(hout, j, i, ' ', WHITE);
					}
				}
			}
			
		}//列循环结尾
	}//行循环结尾
}

/*
功能：获取玩家的输入信息，并作出反应.如果玩家踩到地雷，则返回-2，否则1
*/
char act_to_input()
{
	int judge = 0, x = 1 , y = 1;

	gotoxy(hout,0, R * 6 + 2);

	setcolor(hout,WHITE, BLACK);
	printf("你觉得哪个是地雷呢？: \n");
	printf("输入示例：\n");
	printf("1(1,1)表示标记第一行第一列点为地雷\n0(1,1)表示确定第一行第一列点是没有地雷的  \n(不带空格，不能用中文的标点符号)\n");

	scanf("%d(%d,%d)", &judge, &x, &y);
	do{
		if (judge != 1 && judge != 0 || x > R + 1 || y > L + 1 || x <= 0 || y <= 0 || array[ ( x - 1 ) * R + y - 1 ].state == 1)
		{
			printf("输入错误，请重新输入！\n");
		}
		else
		{
			break;
		}
		scanf("%d(%d,%d)", &judge, &x, &y);
	} while ( true );

	x--;//这两行实现玩家输入的数据转换为数组下标的功能
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
		case 0:one_array_fun(array, x * R + y, expose); return 1;//周围无雷的点的显示
		default: expose(array + x * R + y, x * R + y);  return 1;//周围有雷点的显示
		}
	}
}

char play()
{
	int time_dur;//游戏持续时间
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
				printf("你赢了！！\n");
			}
			else if (state == -2)
			{
				print_map();
				gotoxy(hout, 0, R * 6 + 3);
				setcolor(hout, WHITE, BLACK);
				printf("哈哈！你挂了");
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
		printf("玩家分数：%d 用时:%d\n",secore,time_dur);
		printf("请输入你的名字:(至多20个字符)\n");
		scanf("%20s", name);
		putchar('\n');
		system("cls");

		data_in(name, secore, time_now );
		rank();
		return 0;
	}
	else
	{
		printf("程序错误!\n");
		return 1;
	}
} 