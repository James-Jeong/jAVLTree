#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/javltree.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of Static Functions
////////////////////////////////////////////////////////////////////////////////

static KeyType JAVLTreeCheckKeyType(KeyType type);
static JNodePtr JAVLTreeRotateLL(JNodePtr node);
static JNodePtr JAVLTreeRotateLR(JNodePtr node);
static JNodePtr JAVLTreeRotateRR(JNodePtr node);
static JNodePtr JAVLTreeRotateRL(JNodePtr node);
static JAVLTreePtr JAVLTreeRebalance(JAVLTreePtr tree);
static int JAVLTreeGetHeight(JNodePtr node);
static int JAVLTreeGetHeightDiff(JNodePtr node);
static void JAVLTreeDeleteNodes(JNodePtr node);
static JNodePtr JAVLTreeMoveNode(JNodePtr node, void *key, KeyType type);
static void _PreorderTraverse(JNodePtr node, KeyType type);
static void _InorderTraverse(JNodePtr node, KeyType type);
static void _PostorderTraverse(JNodePtr node, KeyType type);
static void _PrintKey(JNodePtr node, KeyType type);

///////////////////////////////////////////////////////////////////////////////
// Functions for JNode
///////////////////////////////////////////////////////////////////////////////

/**
 * @fn JNodePtr NewJNode()
 * @brief 새로운 노드 구조체 객체를 생성하는 함수
 * @return 성공 시 생성된 노드 구조체 객체의 주소, 실패 시 NULL 반환
 */
JNodePtr NewJNode()
{
	JNodePtr newNode = (JNodePtr)malloc(sizeof(JNode));

	if(newNode == NULL)
	{
		return NULL;
	}

	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = NULL;

	return newNode;
}

/**
 * @fn DeleteResult DeleteJNode(JNodePtrContainer container)
 * @brief 노드 구조체 객체를 삭제하는 함수
 * @param container 노드 구조체 객체의 주소를 저장한 이중 포인터, 컨테이너 변수(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult DeleteJNode(JNodePtrContainer container)
{
	if(container == NULL || *container == NULL) return DeleteFail;
	free(*container);
	*container = NULL;
	return DeleteSuccess;
}

/**
 * @fn void* JNodeGetKey(const JNodePtr node)
 * @brief 노드에 저장된 데이터의 주소를 반환하는 함수
 * @param node 노드 구조체 객체의 주소(입력, 읽기 전용)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JNodeGetKey(const JNodePtr node)
{
	if(node == NULL) return NULL;
	return node->key;
}

/**
 * @fn void* JNodeSetKey(JNodePtr node, void *key)
 * @brief 노드에 데이터의 주소를 저장하는 함수
 * @param node 노드 구조체 객체의 주소(출력)
 * @param key 저장할 데이터의 주소(입력)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JNodeSetKey(JNodePtr node, void *key)
{
	if(node == NULL || key == NULL) return NULL;
	node->key = key;
	return node->key;
}

///////////////////////////////////////////////////////////////////////////////
// Functions for JAVLTree
///////////////////////////////////////////////////////////////////////////////

/**
 * @fn JAVLTreePtr NewJAVLTree(KeyType type)
 * @brief 새로운 AVL Tree 구조체 객체를 생성하는 함수
 * @param type 저장할 키 데이터 유형(입력) 
 * @return 성공 시 생성된 AVL Tree 구조체 객체의 주소, 실패 시 NULL 반환
 */
JAVLTreePtr NewJAVLTree(KeyType type)
{
	if(JAVLTreeCheckKeyType(type) == Unknown) return NULL;

	JAVLTreePtr newTree = (JAVLTreePtr)malloc(sizeof(JAVLTree));
	if(newTree == NULL)
	{
		return NULL;
	}

	newTree->type = type;
	newTree->root = NULL;
	newTree->data = NULL;

	return newTree;
}

/**
 * @fn DeleteResult DeleteJAVLTree(JAVLTreePtrContainer container)
 * @brief AVL Tree 구조체 객체를 삭제하는 함수
 * @param container AVL Tree 구조체 객체의 주소를 저장한 이중 포인터, 컨테이너 변수(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult DeleteJAVLTree(JAVLTreePtrContainer container)
{
	if(container == NULL || *container == NULL) return DeleteFail;

	JNodePtr rootNode = (*container)->root;
	if(rootNode != NULL)
	{
		JAVLTreeDeleteNodes(rootNode);
		free(rootNode);
	}

	free(*container);
	*container = NULL;

	return DeleteSuccess;
}

/**
 * @fn void* JAVLTreeGetKey(const JAVLTreePtr tree)
 * @brief AVL Tree에 저장된 데이터의 주소를 반환하는 함수
 * @param tree AVL Tree 구조체 객체의 주소(입력, 읽기 전용)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JAVLTreeGetData(const JAVLTreePtr tree)
{
	if(tree == NULL) return NULL;
	return tree->data;
}

/**
 * @fn void* JAVLTreeSetData(const JAVLTreePtr tree, void *data)
 * @brief AVL Tree에 데이터의 주소를 저장하는 함수
 * @param tree AVL Tree 구조체 객체의 주소(출력)
 * @param key 저장할 데이터의 주소(입력)
 * @return 성공 시 저장된 데이터의 주소, 실패 시 NULL 반환
 */
void* JAVLTreeSetData(JAVLTreePtr tree, void *data)
{
	if(tree == NULL || data == NULL) return NULL;
	tree->data = data;
	return tree->data;
}

/**
 * @fn JAVLTreePtr JAVLTreeAddNode(JAVLTreePtr tree, void *key)
 * @brief AVL Tree에 새로운 노드를 추가하는 함수
 * 중복 허용하지 않음
 * @param tree AVL Tree 구조체 객체의 주소(출력)
 * @param key 저장할 노드의 데이터 주소(입력)
 * @return 성공 시 AVL Tree 구조체의 주소, 실패 시 NULL 반환
 */
JAVLTreePtr JAVLTreeAddNode(JAVLTreePtr tree, void *key)
{
	if((tree == NULL || key == NULL)) return NULL;

	JNodePtr parentNode = NULL;
	JNodePtr currentNode = tree->root;

	while(currentNode != NULL)
	{
		if(key == currentNode->key) return NULL;

		parentNode = currentNode;
		currentNode = JAVLTreeMoveNode(currentNode, key, tree->type);
	}

	JNodePtr newNode = NewJNode();
	if(JNodeSetKey(newNode, key) == NULL) return NULL;

	if(parentNode != NULL)
	{
		switch(tree->type)
		{
			case IntType:
				if(*((int*)(parentNode->key)) > *((int*)(key))) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			case CharType:
				if(*((char*)(parentNode->key)) > *((char*)(key))) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			case StringType:
				if((char*)(parentNode->key) > (char*)(key)) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			default:
				return NULL;
		}
	}
	else tree->root = newNode;

	if(JAVLTreeRebalance(tree) == NULL) return NULL;
	return tree;
}

/**
 * @fn DeleteResult JAVLTreeDeleteNodeKey(JAVLTreePtr tree, void *key)
 * @brief AVL Tree에 저장된 데이터를 삭제하는 함수
 * @param tree AVL Tree 구조체 객체의 주소(츨력)
 * @param key 삭제할 데이터의 주소(입력)
 * @return 성공 시 DeleteSuccess, 실패 시 DeleteFail 반환(DeleteResult 열거형 참고)
 */
DeleteResult JAVLTreeDeleteNodeKey(JAVLTreePtr tree, void *key)
{
	if(tree == NULL || key == NULL) return DeleteFail;

	DeleteResult result = DeleteSuccess;

	JNodePtr dummyNode = NewJNode();
	if(dummyNode == NULL) return DeleteFail;
	dummyNode->right = tree->root;

	JNodePtr parentNode = dummyNode;
	JNodePtr currentNode = tree->root;
	JNodePtr selectedNode = NULL;

	while((currentNode != NULL) && (key != currentNode->key))
	{
		parentNode = currentNode;
		currentNode = JAVLTreeMoveNode(currentNode, key, tree->type);
	}

	if(currentNode == NULL) return DeleteFail;
	selectedNode = currentNode;

	// 자식 노드가 없는 경우(최하위 노드)
	if((selectedNode->left == NULL) && (selectedNode->right == NULL))
	{
		if(parentNode->left == currentNode) parentNode->left = NULL;
		else parentNode->right = NULL;
	}
	// 자식 노드가 하나밖에 없는 경우
	else if((selectedNode->left == NULL) || (selectedNode->right == NULL))
	{
		// child node of selected node
		JNodePtr scNode = NULL;

		if(selectedNode->left != NULL) scNode = selectedNode->left;
		else scNode = selectedNode->right;

		if(parentNode->left == selectedNode) parentNode->left = scNode;
		else parentNode->right = scNode;
	}
	// 자식 노드가 두 개 다 있는 경우
	else if((selectedNode->left != NULL) && (selectedNode->right != NULL))
	{
		// child node of selected node
		JNodePtr scNode = selectedNode->right;
		// parent node of child node of selected node
		JNodePtr spNode = selectedNode;

		while(scNode->left != NULL)
		{
			spNode = scNode;
			scNode = scNode->left;
		}

		void *tempKey = scNode->key;

		if(spNode->left == scNode) spNode->left = scNode->right;
		else spNode->right = scNode->right;

		JNodeSetKey(selectedNode, tempKey);
		selectedNode = scNode;
	}

	// 루트 노드 삭제 시 변경된 다른 노드로 바꾼다.
	if(dummyNode->right != tree->root) tree->root = dummyNode->right;

	DeleteJNode(&selectedNode);
	DeleteJNode(&dummyNode);

	if(JAVLTreeRebalance(tree) == NULL) result = DeleteFail;
	return result;
}

void JAVLTreePreorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	_PreorderTraverse(tree->root, tree->type);
	printf("\n");
}

void JAVLTreeInorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	_InorderTraverse(tree->root, tree->type);
	printf("\n");
}

void JAVLTreePostorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	_PostorderTraverse(tree->root, tree->type);
	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
/// Static Functions
////////////////////////////////////////////////////////////////////////////////

static JNodePtr JAVLTreeRotateLL(JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->left;

	parentNode->left = currentNode->right;
	currentNode->right = parentNode;

	return currentNode;
}

static JNodePtr JAVLTreeRotateLR(JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->left;

	parentNode->left = JAVLTreeRotateRR(currentNode);
	return JAVLTreeRotateLL(parentNode);
}

static JNodePtr JAVLTreeRotateRR(JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->right;

	parentNode->right = currentNode->left;
	currentNode->left = parentNode;

	return currentNode;
}

static JNodePtr JAVLTreeRotateRL(JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->right;

	parentNode->right = JAVLTreeRotateLL(currentNode);
	return JAVLTreeRotateRR(parentNode);
}

static JAVLTreePtr JAVLTreeRebalance(JAVLTreePtr tree)
{
	if(tree == NULL) return NULL;

	int heightDiff = JAVLTreeGetHeightDiff(tree->root);

	if(heightDiff > 1)
	{
		if(JAVLTreeGetHeightDiff(tree->root->left) > 0) tree->root = JAVLTreeRotateLL(tree->root);
		else tree->root = JAVLTreeRotateLR(tree->root);
	}

	if(heightDiff < -1)
	{
		if(JAVLTreeGetHeightDiff(tree->root->right) < 0) tree->root = JAVLTreeRotateRR(tree->root);
		else tree->root = JAVLTreeRotateRL(tree->root);
	}

	return tree;
}

static KeyType JAVLTreeCheckKeyType(KeyType type)
{
	switch(type)
	{
		case IntType:
		case CharType:
		case StringType:
			break;
		default:
			return Unknown;
	}
	return type;
}

static int JAVLTreeGetHeight(JNodePtr node)
{
	if(node == NULL) return 0;

	int leftHeight = JAVLTreeGetHeight(node->left);
	int rightHeight = JAVLTreeGetHeight(node->right);

	if(leftHeight > rightHeight) return leftHeight + 1;
	else return rightHeight + 1;
}

static int JAVLTreeGetHeightDiff(JNodePtr node)
{
	if(node == NULL) return 0;
	return JAVLTreeGetHeight(node->left) - JAVLTreeGetHeight(node->right);
}

static void JAVLTreeDeleteNodes(JNodePtr node)
{
	if(node == NULL) return;
	
	if(node->left != NULL)
	{
		JAVLTreeDeleteNodes(node->left);
		free(node->left);
		node->left = NULL;
	}

	if(node->right != NULL)
	{
		JAVLTreeDeleteNodes(node->right);
		free(node->right);
		node->right = NULL;
	}
}

static JNodePtr JAVLTreeMoveNode(JNodePtr node, void *key, KeyType type)
{
	switch(type)
	{
		case IntType:
			if(*((int*)(node->key)) > *((int*)(key))) node = node->left;
			else node = node->right;
			break;
		case CharType:
			if(*((char*)(node->key)) > *((char*)(key))) node = node->left;
			else node = node->right;
			break;
		case StringType:
			if(strncmp((char*)(node->key), (char*)(key), strlen((char*)(node->key))) < 0) node = node->left;
			else node = node->right;
			break;
		default: return NULL;
	}
	return node;
}

static void _PreorderTraverse(JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	_PrintKey(node, type);
	_PreorderTraverse(node->left, type);
	_PreorderTraverse(node->right, type);
}

static void _InorderTraverse(JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	_InorderTraverse(node->left, type);
	_PrintKey(node, type);
	_InorderTraverse(node->right, type);
}

static void _PostorderTraverse(JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	_PostorderTraverse(node->left, type);
	_PostorderTraverse(node->right, type);
	_PrintKey(node, type);
}

static void _PrintKey(JNodePtr node, KeyType type)
{
	switch(type)
	{
		case IntType:
			printf("%d ", *((int*)(node->key)));
			break;
		case CharType:
			printf("%c ", *((char*)(node->key)));
			break;
		case StringType:
			printf("%s ", (char*)(node->key));
			break;
		default: return;
	}
}

