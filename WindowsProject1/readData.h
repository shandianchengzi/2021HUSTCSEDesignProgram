#include"hash.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef struct lineInfo             /* 线路辅助结构：线路的基础信息 */
	{
		int lineIndex;                  /* 线路的标号 */
		int lineFull;                   /* 线路满员的人数 */
		float lineLength;               /* 线路的长度 */
		int lineCount;                  /* 线路的站点数量 */
		float lineAverage;              /* 线路上边的平均长度 */
		char lineStations[50][20];      /* 线路上的站点 */
		int crowded[timepart];          /* 线路上不同时间段的拥挤百分数 */
	}lineInfo;

	/* 初始化lineInfo */
	lineInfo* Init_lineInfo();
	/* 函数功能：根据fp指向的文件建立结点信息的哈希表，读入信息 */
	/* 入口参数：文件指针fp，哈希表指针的指针stations，线路辅助结构指针lineDetail */
	/* 出口参数：读取状态。如果读取正常则返回0，否则返回-1 */
	Status read_Data(FILE* fp, hash_struct** stations, lineInfo* lineDetail);
#ifdef __cplusplus
}
#endif
