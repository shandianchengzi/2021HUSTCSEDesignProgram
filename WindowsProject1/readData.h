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
	/* 根据fp指向的文件建立结点信息的哈希表，读入线路信息 */
	Status read_Data(FILE* fp, hash_struct** stations, lineInfo* lineDetail);
#ifdef __cplusplus
}
#endif
