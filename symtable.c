// symtable.c
// Symbol table module
// Author(s): Marián Šuľa
// Last Edit: 20.11.2024
#include "symtable.h"
/**
 * @brief Initializes a binary search tree symbol table.
 *
 * Sets the root of the symbol table to `NULL`.
 *
 * @param symTree A double pointer to the root of the symbol table.
 */
void symtableInit(bstSymtable **symTree)
{
    *symTree = NULL;
}

/**
 * @brief Returns the height of the binary search tree.
 *
 * If the tree is empty, returns 0. Otherwise, returns the height of the tree.
 *
 * @param symTree A pointer to the root of the binary search tree.
 * @return The height of the tree, or 0 if the tree is empty.
 */
int getHeight(bstSymtable **symTree)
{
    if (*symTree == NULL)
    {
        return 0;
    }
    return (*symTree)->height;
}
/**
 * @brief Updates the height of the binary search tree node.
 *
 * This function calculates and updates the height of the current node
 * based on the heights of its left and right subtrees. The height is
 * set to 1 plus the maximum of the left and right subtree heights.
 *
 * @param symTree A double pointer to the root of the binary search tree node whose height needs to be updated.
 */
void updateHeight(bstSymtable **symTree)
{

    if (*symTree != NULL)
    {
        int leftHeight = getHeight(&(*symTree)->left);
        int rightHeight = getHeight(&(*symTree)->right);

        if (leftHeight > rightHeight)
        {
            (*symTree)->height = 1 + leftHeight;
        }
        else
        {
            (*symTree)->height = 1 + rightHeight;
        }
    }
}
/**
 * @brief Calculates the balance factor of a node.
 *
 * The balance factor is the difference between the heights of the left and right subtrees.
 *
 * @param symTree A pointer to the root of the node.
 * @return The balance factor, or 0 if the node is NULL.
 */
int getBalanceFactor(bstSymtable **symTree)
{
    if (*symTree == NULL)
    {
        return 0;
    }
    return getHeight(&(*symTree)->left) - getHeight(&(*symTree)->right);
}
/**
 * @brief Performs a right rotation on the tree.
 *
 * Rotates the current node to the right, updating heights.
 *
 * @param symTree A double pointer to the root of the tree.
 * @return The new root after rotation.
 */
bstSymtable *rotateRight(bstSymtable **symTree)
{

    if (*symTree == NULL || (*symTree)->left == NULL)
    {
        return *symTree;
    }

    bstSymtable *tmpTreeLeft = (*symTree)->left;
    bstSymtable *tmpTreeRight = tmpTreeLeft->right;

    tmpTreeLeft->right = (*symTree);
    (*symTree)->left = tmpTreeRight;

    updateHeight(symTree);
    updateHeight(&tmpTreeLeft);

    return tmpTreeLeft;
}
/**
 * @brief Performs a left rotation on the tree.
 *
 * Rotates the current node to the left, updating heights.
 *
 * @param symTree A double pointer to the root of the tree.
 * @return The new root after rotation.
 */
bstSymtable *rotateLeft(bstSymtable **symTree)
{
    if (*symTree == NULL || (*symTree)->right == NULL)
    {
        return *symTree;
    }

    bstSymtable *tmpTreeLeft = (*symTree)->right;
    bstSymtable *tmpTreeRight = tmpTreeLeft->left;

    tmpTreeLeft->left = (*symTree);
    (*symTree)->right = tmpTreeRight;

    updateHeight(symTree);
    updateHeight(&tmpTreeLeft);

    return tmpTreeLeft;
}
/**
 * @brief Balances a binary search tree node.
 *
 * This function checks the balance factor of the node and performs the
 * appropriate rotation (LL, LR, RR, RL) to balance the tree.
 *
 * @param symTree A double pointer to the root of the tree to balance.
 * @return The new root of the balanced tree.
 */
bstSymtable *balance(bstSymtable **symTree)
{
    if (*symTree == NULL)
        return NULL;

    updateHeight(symTree);

    int balanceFactor = getBalanceFactor(symTree);

    // LL
    if (balanceFactor > 1 && getBalanceFactor(&(*symTree)->left) >= 0)
    {
        return rotateRight(symTree);
    }

    // LR
    if (balanceFactor > 1 && getBalanceFactor(&(*symTree)->left) < 0)
    {
        (*symTree)->left = rotateLeft(&(*symTree)->left);
        return rotateRight(symTree);
    }

    // RR
    if (balanceFactor < -1 && getBalanceFactor(&(*symTree)->right) <= 0)
    {
        return rotateLeft(symTree);
    }

    // RL
    if (balanceFactor < -1 && getBalanceFactor(&(*symTree)->right) > 0)
    {
        (*symTree)->right = rotateRight(&(*symTree)->right);
        return rotateLeft(symTree);
    }

    return *symTree;
}
/**
 * @brief Searches for a node in the symbol table.
 *
 * This function recursively searches for a node with the given key in the
 * binary search tree symbol table. It returns the node if found, or NULL if not.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param key The key to search for in the symbol table.
 * @return A pointer to the node with the matching key, or NULL if not found.
 */
bstSymtable *symtableSearch(bstSymtable **symTree, DSTRING key)
{
    if (symTree == NULL || *symTree == NULL)
    {
        return NULL;
    }
    else if (strcmp(key.str, (*symTree)->key) < 0)
    {
        return symtableSearch(&(*symTree)->left, key);
    }
    else if (strcmp(key.str, (*symTree)->key) > 0)
    {
        return symtableSearch(&(*symTree)->right, key);
    }
    else
    {
        return *symTree;
    }
}
/**
 * @brief Helper function to insert a new node into the symbol table.
 *
 * This function recursively inserts a new node into the binary search tree
 * symbol table. If a node with the same key already exists, its data is updated.
 * The function also updates the height of the affected nodes and balances the tree.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param key The key to insert or update in the symbol table.
 * @param data The data associated with the key.
 */
void symtableInsert(bstSymtable **symTree, DSTRING key, void *data)
{
    if (*symTree == NULL)
    {
        bstSymtable *newNode = (bstSymtable *)malloc(sizeof(bstSymtable));
        if (newNode == NULL)
        {
            return;
        }

        newNode->data = data;
        newNode->key = key.str;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->dataType = -1;
        newNode->height = 1;

        *symTree = newNode;
    }
    else if (strcmp(key.str, (*symTree)->key) < 0)
    {
        symtableInsertVar(&(*symTree)->left, key, data);
    }
    else if (strcmp(key.str, (*symTree)->key) > 0)
    {
        symtableInsertVar(&(*symTree)->right, key, data);
    }
    else
    {
        (*symTree)->data = data;
    }
    updateHeight(symTree);
    *symTree = balance(symTree);
}
/**
 * @brief Function to insert a variable node into the symbol table.
 *
 * This function inserts a new node into the symbol table using `symtableInsert`
 * and sets the `dataType` of the node to `var`.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param key The key to insert in the symbol table.
 * @param data The data associated with the key.
 */
void symtableInsertVar(bstSymtable **symTree, DSTRING key, void *data)
{
    symtableInsert(symTree, key, data);
    (*symTree)->dataType = var;
}
/**
 * @brief Function to insert a function node into the symbol table.
 *
 * This function inserts a new node into the symbol table using `symtableInsert`
 * and sets the `dataType` of the node to `fce`.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param key The key to insert in the symbol table.
 * @param data The data associated with the key.
 */
void symtableInsertFce(bstSymtable **symTree, DSTRING key, void *data)
{
    symtableInsert(symTree, key, data);
    (*symTree)->dataType = fce;
}

/**
 * @brief Appends a C-string to a dynamic string.
 *
 * Adds each character of the null-terminated C-string `str` to the dynamic string `dstr`.
 *
 * @param dstr Pointer to the dynamic string.
 * @param str Null-terminated C-string to append.
 */

void dStringAddString(DSTRING *dstr, const char *str)
{
    while (*str)
    {
        dStringAddChar(dstr, *str++);
    }
}
/**
 * @brief Inserts a function into the symbol table.
 *
 * Checks if a function with the given name exists in the symbol table. If it does, 
 * updates its parameters. Otherwise, creates a new function entry with the 
 * specified attributes and inserts it into the symbol table.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param name The name of the function to insert.
 * @param returnType The return type of the function.
 * @param newParamCount The number of parameters for the function.
 * @param isDefined Specifies whether the function is defined.
 * @param buildIn Specifies whether the function is a built-in function.
 * @param hasLocals Indicates if the function has a local symbol table.
 * @param newVarDatas An array of parameter data for the function.
 */
void insertFunction(bstSymtable **symTree, const char *name, DATATYPE returnType, int newParamCount, bool isDefined, bool buildIn, bool hasLocals, varData *newVarDatas)
{
    DSTRING *functionName = dStringCreate();
    dStringAddString(functionName, name);

    bstSymtable *existingNode = symtableSearch(symTree, *functionName);
    if (existingNode != NULL)
    {
        fceData *existingData = existingNode->data;

        if (newParamCount > 0)
        {

            varData *newParams = (varData *)realloc(existingData->params, newParamCount * sizeof(varData));
            if (newParams == NULL)
            {
                exit(INTERNAL_ERROR);
            }
            existingData->params = newParams;


            existingData->params[newParamCount - 1] = newVarDatas[0];

            existingData->paramCount = newParamCount;
        }

        return;
    }
    else
    {
        fceData *functionData = (fceData *)malloc(sizeof(fceData));
        if (functionData == NULL)
        {

            exit(INTERNAL_ERROR);
        }
        bstSymtable *localTree = NULL;
        if (hasLocals)
        {
            localTree = (bstSymtable *)malloc(sizeof(struct symtable));
            symtableInit(&localTree);
        }

        functionData->returnType = returnType;
        functionData->paramCount = newParamCount;
        functionData->isDefined = isDefined;
        functionData->buildIn = buildIn;
        functionData->locals = (hasLocals) ? &localTree : NULL;

        if (newParamCount > 0)
        {
            functionData->params = (varData *)malloc(newParamCount * sizeof(varData));
            if (functionData->params == NULL)
            {

                exit(INTERNAL_ERROR);
            }
            for (int i = 0; i < newParamCount; i++)
            {
                functionData->params[i] = newVarDatas[i];
            }
        }
        else
        {
            functionData->params = NULL;
        }
        symtableInsertFce(symTree, *functionName, functionData);
    }


}
/**
 * @brief Inserts a variable into a local symbol table.
 *
 * Creates a new variable entry with the specified attributes and inserts it into 
 * the given local symbol table.
 *
 * @param name The name of the variable to insert.
 * @param dataType The data type of the variable.
 * @param initialized Specifies whether the variable is initialized.
 * @param constant Indicates if the variable is constant.
 * @param isPar Specifies whether the variable is a function parameter.
 * @param use Indicates if the variable is in use.
 * @param local A double pointer to the local symbol table where the variable will be inserted.
 */
void insertVariables(const char *name, DATATYPE dataType, bool initialized, bool constant, bool isPar, bool use, bstSymtable **local)
{
    DSTRING *variableName = dStringCreate();
    dStringAddString(variableName, name);

    varData *variableData = (varData *)malloc(sizeof(varData));
    if (variableData == NULL)
    {

        exit(INTERNAL_ERROR);
    }

    variableData->constant = constant;
    variableData->dataType = dataType;
    variableData->initialized = initialized;
    variableData->isPar = isPar;
    variableData->use = use;

    symtableInsertVar(local, *variableName, variableData);
}
/**
 * @brief Inserts built-in functions into the symbol table.
 *
 * Adds predefined built-in functions such as input/output operations, string manipulation,
 * type conversion, and others into the symbol table. Each function is inserted with its
 * name, return type, parameter count, and relevant attributes.
 *
 * @param symTree A double pointer to the root of the symbol table.
 */
void symtableInsertBuildInFce(bstSymtable **symTree)
{
    // Functions for loading values
    insertFunction(symTree, "readstr", (DATATYPE){true, false, T_STR}, 0, true, true, false, NULL);
    insertFunction(symTree, "readi32", (DATATYPE){true, false, T_INT}, 0, true, true, false, NULL);
    insertFunction(symTree, "readf64", (DATATYPE){true, false, T_FLOAT}, 0, true, true, false, NULL);

    // Function for printing out values
    varData varDatas[5];
    varDatas[0].dataType = (DATATYPE){true, false, T_KEYWORD};
    insertFunction(symTree, "write", (DATATYPE){false, true, T_KEYWORD}, 1, true, true, false, varDatas);

    // Functions for converting number values
    varDatas[0].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "i2f", (DATATYPE){false, false, T_FLOAT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_FLOAT};
    insertFunction(symTree, "f2i", (DATATYPE){false, false, T_INT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    // Functions for string operations
    insertFunction(symTree, "string", (DATATYPE){false, false, T_STR}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "length", (DATATYPE){false, false, T_INT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "concat", (DATATYPE){false, false, T_STR}, 2, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_INT};
    varDatas[2].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "substring", (DATATYPE){true, false, T_STR}, 3, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "strcmp", (DATATYPE){false, false, T_INT}, 2, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "ord", (DATATYPE){false, false, T_INT}, 2, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "chr", (DATATYPE){false, false, T_STR}, 1, true, true, false, varDatas);
}
/**
 * @brief Helper function to replace a node with its rightmost descendant.
 *
 * This function recursively finds the rightmost node in the binary search
 * tree and replaces the `target` node with the rightmost node's key and data.
 * After replacement, the rightmost node is removed from the tree.
 *
 * @param target The node to be replaced.
 * @param symTree A double pointer to the root of the tree where the replacement occurs.
 */
void symtableReplaceByRightmost(bstSymtable *target, bstSymtable **symTree)
{
    if (*symTree == NULL)
    {
        return;
    }
    if ((*symTree)->right == NULL)
    {
        target->key = (*symTree)->key;
        target->data = (*symTree)->data;

        bstSymtable *tmp = *symTree;

        *symTree = (*symTree)->left;

        free(tmp);
    }
    else
    {
        symtableReplaceByRightmost(target, &(*symTree)->right);
    }
    updateHeight(symTree);
}
/**
 * @brief Deletes a node from the symbol table.
 *
 * This function removes a node with the given key from the binary search tree.
 * If the node has two children, it is replaced by the rightmost node of its
 * left subtree. Otherwise, it is replaced by its single child or removed if
 * it has none.
 *
 * @param symTree A double pointer to the root of the symbol table.
 * @param key The key of the node to delete.
 */
void symtableDelete(bstSymtable **symTree, DSTRING key)
{
    if (*symTree == NULL)
    {
        return;
    }

    if (strcmp(key.str, (*symTree)->key) < 0)
    {
        symtableDelete(&(*symTree)->left, key);
    }
    else if (strcmp(key.str, (*symTree)->key) > 0)
    {
        symtableDelete(&(*symTree)->right, key);
    }
    else
    {
        if ((*symTree)->left != NULL && (*symTree)->right != NULL)
        {
            symtableReplaceByRightmost(*symTree, &(*symTree)->left);
        }
        else
        {
            bstSymtable *tmp;

            if ((*symTree)->left != NULL)
            {
                tmp = (*symTree)->left;
            }
            else
            {
                tmp = (*symTree)->right;
            }

            free(*symTree);
            *symTree = tmp;
        }
    }
    if (*symTree != NULL)
    {
        updateHeight(symTree);
    }
}
/**
 * @brief Recursively frees all nodes in the symbol table.
 *
 * This function recursively frees each node and sets the root pointer to NULL.
 *
 * @param symTree A double pointer to the root of the symbol table.
 */
void symtableDispose(bstSymtable **symTree)
{
    if (*symTree != NULL)
    {
        symtableDispose(&((*symTree)->left));
        symtableDispose(&((*symTree)->right));

        free(*symTree);
        *symTree = NULL;
    }
}