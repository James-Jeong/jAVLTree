#include "../include/ttlib.h"
#include "../include/javltree.h"

////////////////////////////////////////////////////////////////////////////////
/// Definitions of Test
////////////////////////////////////////////////////////////////////////////////

DECLARE_TEST();

// ---------- Common Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test
////////////////////////////////////////////////////////////////////////////////

TEST(Node, CreateAndDeleteNode, {
	JNodePtr node = NewJNode();
	EXPECT_NOT_NULL(node);
	EXPECT_NUM_EQUAL(DeleteJNode(&node), DeleteSuccess, int);

	EXPECT_NUM_EQUAL(DeleteJNode(NULL), DeleteFail, int);
})

////////////////////////////////////////////////////////////////////////////////
/// AVL Tree Test
////////////////////////////////////////////////////////////////////////////////

TEST(AVLTree, CreateAndDeleteAVLTree, {
	JAVLTreePtr tree = NewJAVLTree(IntType);
	EXPECT_NOT_NULL(tree);
	EXPECT_NUM_EQUAL(DeleteJAVLTree(&tree), DeleteSuccess, int);

	EXPECT_NULL(NewJAVLTree(123));
	EXPECT_NUM_EQUAL(DeleteJAVLTree(NULL), DeleteFail, int);
})

// ---------- AVL Tree int Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (int)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_INT, SetKey, {
	JNodePtr node = NewJNode();

	int expected = 5;
	EXPECT_NOT_NULL(JNodeSetKey(node, &expected));
	EXPECT_PTR_EQUAL(node->key, &expected);
	EXPECT_NUM_EQUAL(*((int*)(node->key)), expected, int);

	EXPECT_NULL(JNodeSetKey(NULL, &expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_INT, GetKey, {
	JNodePtr node = NewJNode();

	int expected = 5;
	JNodeSetKey(node, &expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_NUM_EQUAL(*((int*)JNodeGetKey(node)), expected, int);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// AVLTree Test (int)
////////////////////////////////////////////////////////////////////////////////

TEST(AVLTree_INT, AddNode, {
	JAVLTreePtr tree = NewJAVLTree(IntType);
	int expected1 = 1;
	int expected2 = 2;
	int expected3 = 3;
	int expected4 = 4;
	int expected5 = 5;

	// 정상 동작 확인
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected4));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected5));
	EXPECT_NUM_EQUAL(*((int*)(tree->root->key)), expected3, int);
	
	JAVLTreeInorderTraverse(tree);

	// 중복 허용 테스트
	EXPECT_NULL(JAVLTreeAddNode(tree, &expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JAVLTreeAddNode(NULL, &expected1));
	EXPECT_NULL(JAVLTreeAddNode(tree, NULL));
	EXPECT_NULL(JAVLTreeAddNode(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_INT, SetData, {
	JAVLTreePtr tree = NewJAVLTree(IntType);
	int expected = 5;

	EXPECT_NOT_NULL(JAVLTreeSetData(tree, &expected));
	EXPECT_NUM_EQUAL(*((int*)(tree->data)), expected, int);

	EXPECT_NULL(JAVLTreeSetData(NULL, &expected));
	EXPECT_NULL(JAVLTreeSetData(tree, NULL));
	EXPECT_NULL(JAVLTreeSetData(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_INT, GetData, {
	JAVLTreePtr tree = NewJAVLTree(IntType);
	int expected = 5;

	JAVLTreeSetData(tree, &expected);
	EXPECT_NOT_NULL(JAVLTreeGetData(tree));
	EXPECT_NUM_EQUAL(*((int*)JAVLTreeGetData(tree)), 5, int);

	EXPECT_NULL(JAVLTreeGetData(NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_INT, DeleteNodeKey, {
	JAVLTreePtr tree = NewJAVLTree(IntType);
	int expected1 = 1;
	int expected2 = 2;
	int expected3 = 3;
	int expected4 = 4;
	int expected5 = 5;

	// 정상 동작 확인
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected4));

	JAVLTreeInorderTraverse(tree);

	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected1), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected3), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected4), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);

	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected5), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, &expected1), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, NULL), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, NULL), DeleteFail, int);

	DeleteJAVLTree(&tree);
})

// ---------- AVL Tree char Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (char)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_CHAR, SetKey, {
	JNodePtr node = NewJNode();

	char expected = 'a';
	EXPECT_NOT_NULL(JNodeSetKey(node, &expected));
	EXPECT_PTR_EQUAL(node->key, &expected);
	EXPECT_NUM_EQUAL(*((char*)(node->key)), expected, int);

	EXPECT_NULL(JNodeSetKey(NULL, &expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_CHAR, GetKey, {
	JNodePtr node = NewJNode();

	char expected = 'a';
	JNodeSetKey(node, &expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_PTR_EQUAL(JNodeGetKey(node), &expected);
	EXPECT_NUM_EQUAL(*((char*)JNodeGetKey(node)), expected, int);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// AVLTree Test (char)
////////////////////////////////////////////////////////////////////////////////

TEST(AVLTree_CHAR, AddNode, {
	JAVLTreePtr tree = NewJAVLTree(CharType);
	char expected1 = 'a';
	char expected2 = 'b';
	char expected3 = 'c';
	char expected4 = 'd';
	char expected5 = 'e';

	// 정상 동작 확인
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected4));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected5));
	EXPECT_NUM_EQUAL(*((char*)(tree->root->key)), expected3, int);
	
	JAVLTreeInorderTraverse(tree);

	// 중복 허용 테스트
	EXPECT_NULL(JAVLTreeAddNode(tree, &expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JAVLTreeAddNode(NULL, &expected1));
	EXPECT_NULL(JAVLTreeAddNode(tree, NULL));
	EXPECT_NULL(JAVLTreeAddNode(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_CHAR, SetData, {
	JAVLTreePtr tree = NewJAVLTree(CharType);
	char expected = 'a';

	EXPECT_NOT_NULL(JAVLTreeSetData(tree, &expected));
	EXPECT_NUM_EQUAL(*((char*)(tree->data)), expected, int);

	EXPECT_NULL(JAVLTreeSetData(NULL, &expected));
	EXPECT_NULL(JAVLTreeSetData(tree, NULL));
	EXPECT_NULL(JAVLTreeSetData(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_CHAR, GetData, {
	JAVLTreePtr tree = NewJAVLTree(CharType);
	char expected = 'a';

	JAVLTreeSetData(tree, &expected);
	EXPECT_NOT_NULL(JAVLTreeGetData(tree));
	EXPECT_NUM_EQUAL(*((char*)JAVLTreeGetData(tree)), expected, int);

	EXPECT_NULL(JAVLTreeGetData(NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_CHAR, DeleteNodeKey, {
	JAVLTreePtr tree = NewJAVLTree(CharType);
	char expected1 = 'a';
	char expected2 = 'b';
	char expected3 = 'c';
	char expected4 = 'd';
	char expected5 = 'e';

	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected4));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, &expected5));
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected1), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected3), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected5), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);

	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, &expected5), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, &expected1), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, NULL), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, NULL), DeleteFail, int);

	DeleteJAVLTree(&tree);
})

// ---------- AVL Tree string Test ----------

////////////////////////////////////////////////////////////////////////////////
/// Node Test (string)
////////////////////////////////////////////////////////////////////////////////

TEST(Node_STRING, SetKey, {
	JNodePtr node = NewJNode();

	char *expected = "abc";
	EXPECT_NOT_NULL(JNodeSetKey(node, expected));
	EXPECT_PTR_EQUAL(node->key, expected);
	EXPECT_STR_EQUAL((char*)(node->key), expected);

	EXPECT_NULL(JNodeSetKey(NULL, expected));
	EXPECT_NULL(JNodeSetKey(node, NULL));
	EXPECT_NULL(JNodeSetKey(NULL, NULL));

	DeleteJNode(&node);
})

TEST(Node_STRING, GetKey, {
	JNodePtr node = NewJNode();

	char *expected = "abc";
	JNodeSetKey(node, expected);
	EXPECT_NOT_NULL(JNodeGetKey(node));
	EXPECT_STR_EQUAL((char*)JNodeGetKey(node), expected);

	EXPECT_NULL(JNodeGetKey(NULL));

	DeleteJNode(&node);
})

////////////////////////////////////////////////////////////////////////////////
/// AVLTree Test (string)
////////////////////////////////////////////////////////////////////////////////

TEST(AVLTree_STRING, AddNode, {
	JAVLTreePtr tree = NewJAVLTree(StringType);
	char *expected1 = "abc";
	char *expected2 = "de3f";
	char *expected3 = "11223";
	char *expected4 = "awdg";
	char *expected5 = "24g";

	// 정상 동작 확인
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected4));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected5));
	EXPECT_STR_EQUAL((char*)(tree->root->key), expected1);
	
	JAVLTreeInorderTraverse(tree);

	// 중복 허용 테스트
	EXPECT_NULL(JAVLTreeAddNode(tree, expected1));

	// NULL 이 입력값이면, NULL 을 반환
	EXPECT_NULL(JAVLTreeAddNode(NULL, expected1));
	EXPECT_NULL(JAVLTreeAddNode(tree, NULL));
	EXPECT_NULL(JAVLTreeAddNode(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_STRING, SetData, {
	JAVLTreePtr tree = NewJAVLTree(StringType);
	char* expected = "abc";

	EXPECT_NOT_NULL(JAVLTreeSetData(tree, expected));
	EXPECT_PTR_EQUAL(tree->data, expected);
	EXPECT_STR_EQUAL((char*)(tree->data), expected);

	EXPECT_NULL(JAVLTreeSetData(NULL, expected));
	EXPECT_NULL(JAVLTreeSetData(tree, NULL));
	EXPECT_NULL(JAVLTreeSetData(NULL, NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_STRING, GetData, {
	JAVLTreePtr tree = NewJAVLTree(StringType);
	char* expected = "abc";

	JAVLTreeSetData(tree, expected);
	EXPECT_NOT_NULL(JAVLTreeGetData(tree));
	EXPECT_PTR_EQUAL(JAVLTreeGetData(tree), expected);
	EXPECT_STR_EQUAL((char*)JAVLTreeGetData(tree), expected);

	EXPECT_NULL(JAVLTreeGetData(NULL));

	DeleteJAVLTree(&tree);
})

TEST(AVLTree_STRING, DeleteNodeKey, {
	JAVLTreePtr tree = NewJAVLTree(StringType);
	char *expected1 = "abc";
	char *expected2 = "de3f";
	char *expected3 = "11223";
	char *expected4 = "awdg";
	char *expected5 = "24g";

	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected1));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected2));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected3));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected4));
	EXPECT_NOT_NULL(JAVLTreeAddNode(tree, expected5));
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, expected1), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, expected3), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, expected5), DeleteSuccess, int);
	JAVLTreeInorderTraverse(tree);

	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, expected1), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, expected1), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(tree, NULL), DeleteFail, int);
	EXPECT_NUM_EQUAL(JAVLTreeDeleteNodeKey(NULL, NULL), DeleteFail, int);

	DeleteJAVLTree(&tree);
})

////////////////////////////////////////////////////////////////////////////////
/// Main Function
////////////////////////////////////////////////////////////////////////////////

int main()
{
    CREATE_TESTSUIT();

    REGISTER_TESTS(
		// @ Common Test -----------------------------------------
		Test_Node_CreateAndDeleteNode,
		Test_AVLTree_CreateAndDeleteAVLTree,

		// @ INT Test -------------------------------------------
		Test_Node_INT_SetKey,
		Test_Node_INT_GetKey,
		Test_AVLTree_INT_AddNode,
		Test_AVLTree_INT_SetData,
		Test_AVLTree_INT_GetData,
		Test_AVLTree_INT_DeleteNodeKey,

		// @ CHAR Test -------------------------------------------
		Test_Node_CHAR_SetKey,
		Test_Node_CHAR_GetKey,
		Test_AVLTree_CHAR_AddNode,
		Test_AVLTree_CHAR_SetData,
		Test_AVLTree_CHAR_GetData,
		Test_AVLTree_CHAR_DeleteNodeKey,

		// @ STRING Test -------------------------------------------
		Test_Node_STRING_SetKey,
		Test_Node_STRING_GetKey,
		Test_AVLTree_STRING_AddNode,
		Test_AVLTree_STRING_SetData,
		Test_AVLTree_STRING_GetData,
		Test_AVLTree_STRING_DeleteNodeKey
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

	return 1;
}

