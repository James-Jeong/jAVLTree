#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/javltree.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of JNode Static Functions
////////////////////////////////////////////////////////////////////////////////

static JNodePtr JNodeRotateLL(const JNodePtr node);
static JNodePtr JNodeRotateLR(const JNodePtr node);
static JNodePtr JNodeRotateRR(const JNodePtr node);
static JNodePtr JNodeRotateRL(const JNodePtr node);
static int JNodeGetHeight(const JNodePtr node);
static int JNodeGetHeightDiff(const JNodePtr node);
static void JNodeDeleteChilds(JNodePtr node);
static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type);
static void JNodePreorderTraverse(const JNodePtr node, KeyType type);
static void JNodeInorderTraverse(const JNodePtr node, KeyType type);
static void JNodePostorderTraverse(const JNodePtr node, KeyType type);
static void JNodePrintKey(const JNodePtr node, KeyType type);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of JAVLTree Static Function
////////////////////////////////////////////////////////////////////////////////

static JAVLTreePtr JAVLTreeRebalance(JAVLTreePtr tree);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of Util Static Functions
////////////////////////////////////////////////////////////////////////////////

static KeyType _CheckKeyType(KeyType type);
static int _GetCompareLength(const char *s1, const char *s2);

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
 * @brief 노드에 저장된 키의 주소를 반환하는 함수
 * @param node 노드 구조체 객체의 주소(입력, 읽기 전용)
 * @return 성공 시 저장된 키의 주소, 실패 시 NULL 반환
 */
void* JNodeGetKey(const JNodePtr node)
{
	if(node == NULL) return NULL;
	return node->key;
}

/**
 * @fn void* JNodeSetKey(JNodePtr node, void *key)
 * @brief 노드에 의 주소를 저장하는 함수
 * @param node 노드 구조체 객체의 주소(출력)
 * @param key 저장할 키의 주소(입력)
 * @return 성공 시 저장된 키의 주소, 실패 시 NULL 반환
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
	if(_CheckKeyType(type) == Unknown) return NULL;

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
		JNodeDeleteChilds(rootNode);
		free(rootNode);
	}

	free(*container);
	*container = NULL;

	return DeleteSuccess;
}

/**
 * @fn void* JAVLTreeGetData(const JAVLTreePtr tree)
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
 * @param key 저장할 노드의 키 주소(입력)
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
		currentNode = JNodeMove(currentNode, key, tree->type);
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
				if(strncmp((char*)(parentNode->key), (char*)(key), _GetCompareLength((char*)(parentNode->key), (char*)(key))) < 0) parentNode->left = newNode;
				else parentNode->right = newNode;
				break;
			default:
				DeleteJNode(&newNode);
				return NULL;
		}
	}
	else tree->root = newNode;

	if(JAVLTreeRebalance(tree) == NULL)
	{
		if(parentNode != NULL)
		{
			if(parentNode->left == newNode) parentNode->left = NULL;
			else parentNode->right = NULL;
		}
		else tree->root = NULL;
		DeleteJNode(&newNode);
		return NULL;
	}

	return tree;
}

/**
 * @fn DeleteResult JAVLTreeDeleteNodeKey(JAVLTreePtr tree, void *key)
 * @brief AVL Tree에 지정한 키를 가진 노드를 삭제하는 함수
 * @param tree AVL Tree 구조체 객체의 주소(츨력)
 * @param key 삭제할 키의 주소(입력)
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
		currentNode = JNodeMove(currentNode, key, tree->type);
	}

	if(currentNode == NULL)
	{
		DeleteJNode(&dummyNode);
		return DeleteFail;
	}
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

/**
 * @fn void JAVLTreePreorderTraverse(const JAVLTreePtr tree)
 * @brief AVL Tree 를 전위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 AVL Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JAVLTreePreorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	JNodePreorderTraverse(tree->root, tree->type);
	printf("\n");
}

/**
 * @fn void JAVLTreeInorderTraverse(const JAVLTreePtr tree)
 * @brief AVL Tree 를 중위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 AVL Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JAVLTreeInorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	JNodeInorderTraverse(tree->root, tree->type);
	printf("\n");
}

/**
 * @fn void JAVLTreePostorderTraverse(const JAVLTreePtr tree)
 * @brief AVL Tree 를 후위 순회하며 노드의 키를 출력하는 함수
 * @param tree 순회할 AVL Tree (입력, 읽기 전용)
 * @return 반환값 없음
 */
void JAVLTreePostorderTraverse(const JAVLTreePtr tree)
{
	if(tree == NULL) return;
	JNodePostorderTraverse(tree->root, tree->type);
	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
/// JNode Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static JNodePtr JNodeRotateLL(JNodePtr node)
 * @brief 지정한 노드를 기준으로 하위 노드들에 대해 LL 인 상황에서 AVL Tree 를 균형있게 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeRotateLL(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->left;

	parentNode->left = currentNode->right;
	currentNode->right = parentNode;

	return currentNode;
}

/**
 * @fn static JNodePtr JNodeRotateLR(const JNodePtr node)
 * @brief 지정한 노드를 기준으로 하위 노드들에 대해 LR 인 상황에서 AVL Tree 를 균형있게 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeRotateLR(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->left;

	parentNode->left = JNodeRotateRR(currentNode);
	return JNodeRotateLL(parentNode);
}

/**
 * @fn static JNodePtr JNodeRotateRR(const JNodePtr node)
 * @brief 지정한 노드를 기준으로 하위 노드들에 대해 RR 인 상황에서 AVL Tree 를 균형있게 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeRotateRR(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->right;

	parentNode->right = currentNode->left;
	currentNode->left = parentNode;

	return currentNode;
}

/**
 * @fn static JNodePtr JNodeRotateRL(const JNodePtr node)
 * @brief 지정한 노드를 기준으로 하위 노드들에 대해 RL 인 상황에서 AVL Tree 를 균형있게 회전하는 함수
 * @param node 회전하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 회전된 기준 노드, 실패 시 NULL 반환
 */
static JNodePtr JNodeRotateRL(const JNodePtr node)
{
	if(node == NULL) return NULL;

	JNodePtr parentNode = node;
	JNodePtr currentNode = parentNode->right;

	parentNode->right = JNodeRotateLL(currentNode);
	return JNodeRotateRR(parentNode);
}

/**
 * @fn static int JNodeGetHeight(const JNodePtr node)
 * @brief AVL Tree 에서 지정한 노드의 높이를 구하는 함수(재귀)
 * 루트 노드는 1, 이하 자식 노드 레벨(높이)부터 1 씩 증가
 * @param node 높이를 구하기 위한 노드(입력, 읽기 전용) 
 * @return 성공 시 0 이상의 높이, 실패 시 0 반환
 */
static int JNodeGetHeight(const JNodePtr node)
{
	if(node == NULL) return 0;

	int leftHeight = JNodeGetHeight(node->left);
	int rightHeight = JNodeGetHeight(node->right);

	if(leftHeight > rightHeight) return leftHeight + 1;
	else return rightHeight + 1;
}

/**
 * @fn static int JNodeGetHeightDiff(const JNodePtr node)
 * @brief AVL Tree 에서 지정한 노드의 자식노드들의 높이 차이를 구하는 함수(재귀)
 * @param node 높이의 차이를 구하기 위한 기준 노드(입력, 읽기 전용) 
 * @return 성공 시 자식 노드들의 높이 차이, 실패 시 0 반환
 */
static int JNodeGetHeightDiff(const JNodePtr node)
{
	if(node == NULL) return 0;
	return JNodeGetHeight(node->left) - JNodeGetHeight(node->right);
}

/**
 * @fn static void JNodeDeleteChilds(JNodePtr node)
 * @brief AVL Tree 에 저장된 노드들을 모두 삭제하는 함수(재귀)
 * @param node 자식 노드들을 삭제하기 위한 기준 노드(입력)
 * @return 반환값 없음
 */
static void JNodeDeleteChilds(JNodePtr node)
{
	if(node == NULL) return;
	
	if(node->left != NULL)
	{
		JNodeDeleteChilds(node->left);
		free(node->left);
		node->left = NULL;
	}

	if(node->right != NULL)
	{
		JNodeDeleteChilds(node->right);
		free(node->right);
		node->right = NULL;
	}
}

/**
 * @fn static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type)
 * @brief 지정한 노드의 키와 전달받은 키를 비교해서 저장할 노드의 위치를 찾아 그 노드의 주소를 반환하는 함수 
 * @param node 위치를 움직일 노드의 주소(입력)
 * @param key 전달받은 키(입력)
 * @param type 키의 데이터 유형(입력)
 * @return 성공 시 위치가 변경된 노드의 주소, 실패 시 NULL 반환
 */
static JNodePtr JNodeMove(JNodePtr node, void *key, KeyType type)
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
		{
			if(strncmp((char*)(node->key), (char*)(key), _GetCompareLength((char*)(node->key), (char*)(key))) < 0) node = node->left;
			else node = node->right;
			break;
		}
		default: return NULL;
	}
	return node;
}

/**
 * @fn static void JNodePreorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 전위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePreorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodePrintKey(node, type);
	JNodePreorderTraverse(node->left, type);
	JNodePreorderTraverse(node->right, type);
}

/**
 * @fn static void JNodeInorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 중위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodeInorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodeInorderTraverse(node->left, type);
	JNodePrintKey(node, type);
	JNodeInorderTraverse(node->right, type);
}

/**
 * @fn static void JNodePostorderTraverse(const JNodePtr node, KeyType type)
 * @brief 지정한 노드를 기준으로 후위 순회하며 키를 출력하는 함수(재귀)
 * @param node 순회할 기준 노드의 주소(입력)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePostorderTraverse(const JNodePtr node, KeyType type)
{
	if(node == NULL) return;
	JNodePostorderTraverse(node->left, type);
	JNodePostorderTraverse(node->right, type);
	JNodePrintKey(node, type);
}

/**
 * @fn static void JNodePrintKey(const JNodePtr node, KeyType type)
 * @brief 지정한 노드의 키를 출력하는 함수
 * @param node 출력할 노드의 주소(입력, 읽기 전용)
 * @param type 출력할 키의 데이터 유형(입력)
 * @return 반환값 없음
 */
static void JNodePrintKey(const JNodePtr node, KeyType type)
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

////////////////////////////////////////////////////////////////////////////////
/// JAVLTree Static Function
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static JAVLTreePtr JAVLTreeRebalance(JAVLTreePtr tree)
 * @brief AVL Tree 의 높이 균형을 맞추도록 노드들의 위치를 재배열하는 함수
 * @param tree AVL Tree 의 주소(입력)
 * @return 성공 시 AVL Tree 의 주소, 실패 시 NULL 반환
 */
static JAVLTreePtr JAVLTreeRebalance(JAVLTreePtr tree)
{
	if(tree == NULL) return NULL;

	int heightDiff = JNodeGetHeightDiff(tree->root);

	if(heightDiff > 1)
	{
		if(JNodeGetHeightDiff(tree->root->left) > 0) tree->root = JNodeRotateLL(tree->root);
		else tree->root = JNodeRotateLR(tree->root);
	}

	if(heightDiff < -1)
	{
		if(JNodeGetHeightDiff(tree->root->right) < 0) tree->root = JNodeRotateRR(tree->root);
		else tree->root = JNodeRotateRL(tree->root);
	}

	return tree;
}

////////////////////////////////////////////////////////////////////////////////
/// Util Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static KeyType _CheckKeyType(KeyType type)
 * @brief 지정한 키 데이터 유형이 정의된 유형인지 검사하는 함수
 * @param type 검사할 키 데이터 유형(입력)
 * @return 성공 시 전달받은 키 데이터 유형, 실패 시 Unknown 반환(KeyType 열거형 참고)
 */
static KeyType _CheckKeyType(KeyType type)
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

/**
 * @fn static int _GetCompareLength(const char *s1, const char *s2)
 * @brief 두 문자열을 비교할 때 가장 긴 문자열의 길이를 반환하는 함수
 * @param s1 첫 번째 비교할 문자열(입력, 읽기 전용)
 * @param s2 두 번째 비교할 문자열(입력, 읽기 전용)
 * @return 항상 비교할 길이 반환
 */
static int _GetCompareLength(const char *s1, const char *s2)
{
	int s1Length = strlen(s1);
	int s2Length = strlen(s2);
	return s1Length > s2Length ? s1Length : s2Length;
}

