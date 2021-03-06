#include"uthash.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef int Status;
	//站点结点的数据结构
	typedef struct node {            /* 数据域中具体内容 */
		int isTransfer;             /* 该站点是否是换乘站点 */
		char whose_where[50];       /* 该站点是哪条线路的第几号 */
		int index;                  /* 该站点的编号 */
	}node;

	typedef struct hash_struct {     /* 站点辅助结构：Hash表 */
		char name[20];              /* key：站点名字 */
		node data;                  /* 结点数据域 */
		UT_hash_handle hh;          /* makes this structure hashable */
	}hash_struct;

	/* 函数功能：在hash表中添加结点 */
	/* 入口参数：stations是hash表，station_name是key值，data是数据域 */
	/* 出口参数：指向被添加的结点的存储地址的指针 */
	hash_struct* add_station(hash_struct** stations, char* station_name, node data);

	/* 函数功能：在hash表中找key为station_name的结点 */
	/* 入口参数：stations是hash表，station_name是key值 */
	/* 出口参数：指向找到的结点的存储地址的指针，若没有找到，则返回-1 */
	hash_struct* find_station(hash_struct* stations, char* station_name);

	/* 函数功能：删除hash表中的一个哈希结点 */
	/* 入口参数：stations是hash表，station是指向被删除结点的指针 */
	void delete_station(hash_struct** stations, hash_struct* station);

#ifdef __cplusplus
}
#endif
