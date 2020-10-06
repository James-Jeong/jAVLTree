#ifndef __JHASHTABLE_H__
#define __JHASHTABLE_H__

///////////////////////////////////////////////////////////////////////////////
/// Enums
///////////////////////////////////////////////////////////////////////////////

// 메모리 헤제 결과 열거형
typedef enum DeleteResult
{
	// 실패
	DeleteFail = -1,
	// 성공
	DeleteSuccess = 1
} DeleteResult;

// 검색 결과 열거형
typedef enum FindResult
{
	// 실패
	FindFail = -1,
	// 성공
	FindSuccess = 1
} FindResult;

// 키 유형 열거형
typedef enum KeyType
{
	Unknown = -1,
	IntType = 1,
	CharType,
	StringType
} KeyType;

///////////////////////////////////////////////////////////////////////////////
/// Macro
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Definitions
///////////////////////////////////////////////////////////////////////////////

// Linked List 에서 key 를 관리하기 위한 노드 구조체
typedef struct _jnode_t {
	// Value
	void *key;
	// 이전 노드 주소
	struct _jnode_t *left;
	// 다음 노드 주소
	struct _jnode_t *right;
} JNode, *JNodePtr, **JNodePtrContainer;

// AVL Tree 구조체
typedef struct _javltree_t {
	// 키 데이터 유형
	KeyType type;
	// 루트 노드
	JNodePtr root;
	// 사용자 데이터
	void *data;
} JAVLTree, *JAVLTreePtr, **JAVLTreePtrContainer;

///////////////////////////////////////////////////////////////////////////////
// Functions for JNode
///////////////////////////////////////////////////////////////////////////////

JNodePtr NewJNode();
DeleteResult DeleteJNode(JNodePtrContainer container);
void* JNodeGetKey(const JNodePtr node);
void* JNodeSetKey(JNodePtr node, void *key);

///////////////////////////////////////////////////////////////////////////////
// Functions for JAVLTree
///////////////////////////////////////////////////////////////////////////////

JAVLTreePtr NewJAVLTree(int index);
DeleteResult DeleteJAVLTree(JAVLTreePtrContainer container);

void* JAVLTreeGetKey(const JAVLTreePtr tree);
void* JAVLTreeSetKey(JAVLTreePtr tree, void *data);

JAVLTreePtr JAVLTreeAddNode(JAVLTreePtr tree, void *data);
DeleteResult JAVLTreeDeleteNodeKey(JAVLTreePtr tree, void *key);


#endif

