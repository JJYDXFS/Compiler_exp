#pragma once
// 所有结点保存在一维列表中，通过下标访问
typedef int NType;
typedef char InfoType;
typedef struct Edge {	// 边
	int NodeIndex;	// 顶点下标
	struct Edge *nextedge;	// 下一条邻边
	InfoType info;	// 转移条件，即读取的字符
}ArcNode;
typedef struct NFANode {	// 状态结点
	NType data;	// 保留信息，暂时没用到
	Edge *firstedge;	// 第一条邻边
}NFANode;
typedef struct NFA {	// NFA
	int start;	// 起始结点
	int end;	// 结束结点
}NFA;

