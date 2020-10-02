#pragma once
#include <iostream>
// 所有结点保存在一维列表中，通过下标访问
typedef int NType;
typedef char InfoType;
typedef struct NFA {	// NFA
	int start;	// 起始结点
	int end;	// 结束结点
}NFA;
class Edge	// 边
{
public:
	int NodeIndex;	// 终点下标
	Edge *nextedge;	// 下一条邻边
	InfoType info;	// 转移条件，即读取的字符
	Edge() {
		NodeIndex = 0;
		nextedge = NULL;
		info = '\0';
	}
	Edge(const Edge &E) {	// 拷贝构造函数
		NodeIndex = E.NodeIndex;
		nextedge = E.nextedge;
		info = E.info;
	}
	/*~Edge() {
		if (nextedge != NULL) {
			delete[]nextedge;
			nextedge = NULL;
		}
	}*/
};
class NFANode	// 状态结点
{
public:
	NType data;	// 保留信息，暂时没用到
	Edge *firstedge;	// 第一条邻边
	NFANode() {
		data = '\0';
		firstedge = NULL;
	}
	NFANode(const NFANode &N) {	// 拷贝构造函数
		data = N.data;
		firstedge = N.firstedge;
	}
	/*~NFANode() {
		if (firstedge != NULL) {
			delete[]firstedge;
			firstedge = NULL;
		}
	}*/
};

// 初代数据结构 —— 结构体
//typedef struct Edge {	
//	int NodeIndex;	// 终点下标
//	struct Edge *nextedge;	// 下一条邻边
//	InfoType info;	// 转移条件，即读取的字符
//}Edge;
//typedef struct NFANode {	
//	NType data;	// 保留信息，暂时没用到
//	Edge *firstedge;	// 第一条邻边
//}NFANode;


