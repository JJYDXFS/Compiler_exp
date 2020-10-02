#pragma once
#include <iostream>
// ���н�㱣����һά�б��У�ͨ���±����
typedef int NType;
typedef char InfoType;
typedef struct NFA {	// NFA
	int start;	// ��ʼ���
	int end;	// �������
}NFA;
class Edge	// ��
{
public:
	int NodeIndex;	// �յ��±�
	Edge *nextedge;	// ��һ���ڱ�
	InfoType info;	// ת������������ȡ���ַ�
	Edge() {
		NodeIndex = 0;
		nextedge = NULL;
		info = '\0';
	}
	Edge(const Edge &E) {	// �������캯��
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
class NFANode	// ״̬���
{
public:
	NType data;	// ������Ϣ����ʱû�õ�
	Edge *firstedge;	// ��һ���ڱ�
	NFANode() {
		data = '\0';
		firstedge = NULL;
	}
	NFANode(const NFANode &N) {	// �������캯��
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

// �������ݽṹ ���� �ṹ��
//typedef struct Edge {	
//	int NodeIndex;	// �յ��±�
//	struct Edge *nextedge;	// ��һ���ڱ�
//	InfoType info;	// ת������������ȡ���ַ�
//}Edge;
//typedef struct NFANode {	
//	NType data;	// ������Ϣ����ʱû�õ�
//	Edge *firstedge;	// ��һ���ڱ�
//}NFANode;


