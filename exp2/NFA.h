#pragma once
// ���н�㱣����һά�б��У�ͨ���±����
typedef int NType;
typedef char InfoType;
typedef struct Edge {	// ��
	int NodeIndex;	// �����±�
	struct Edge *nextedge;	// ��һ���ڱ�
	InfoType info;	// ת������������ȡ���ַ�
}ArcNode;
typedef struct NFANode {	// ״̬���
	NType data;	// ������Ϣ����ʱû�õ�
	Edge *firstedge;	// ��һ���ڱ�
}NFANode;
typedef struct NFA {	// NFA
	int start;	// ��ʼ���
	int end;	// �������
}NFA;

