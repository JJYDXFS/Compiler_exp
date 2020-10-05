# 关于当前数据结构进展的简单说明
```c++
// 采用邻接表格式存储所有NFA，将所有NFA视作一张大图（最后总会拼在一起）
// 所有图上结点保存在一维列表vector<NFANode> NodeList中，通过下标访问任意结点(如：NodeList[i])
typedef int NType;	// 状态结点的信息，暂未使用
typedef char InfoType;	// 边的信息，此处指转移条件（状态A接收输入'a'转移到状态B）
typedef struct Edge {	// 边
	int NodeIndex;	// 终点下标
	struct Edge *nextedge;	// 起点的下一条邻边
	InfoType info;	// 转移条件
}ArcNode;
typedef struct NFANode {	// 状态结点
	NType data;	// 保留信息，暂时没用到
	Edge *firstedge;	// 结点的第一条邻边
}NFANode;
typedef struct NFA {	// NFA
	int start;	// 起始结点下标
	int end;	// 结束结点下标
}NFA;
```

