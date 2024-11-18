// symtable.c
// Symbol table module
// Author(s): Václav Bergman, Marián Šuľa
// Last Edit: 15.11.2024
// zajtra dopisat
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

void dStringAddString(DSTRING *dstr, const char *str)
{
    while (*str)
    {
        dStringAddChar(dstr, *str++);
    }
}

void insertFunction(bstSymtable **symTree, const char *name, DATATYPE returnType, int paramCount, bool isDefined, bool buildIn, bool hasLocals, varData *varDatas)
{
    DSTRING *functionName = dStringCreate();
    dStringAddString(functionName, name);
    fceData *functionData = (fceData *)malloc(sizeof(fceData));
    if (functionData == NULL)
    {
        // Handle memory allocation failure
        printf("Memory allocation for fceData failed\n");
        exit(1); // Or handle it appropriately
    }
    bstSymtable *localTreei2f = NULL;
    if (hasLocals)
    {
        localTreei2f = (bstSymtable *)malloc(sizeof(struct symtable));
        symtableInit(&localTreei2f);
    }
    // Initialize the fields
    functionData->returnType = returnType;
    functionData->paramCount = paramCount;
    functionData->isDefined = isDefined;
    functionData->buildIn = buildIn;
    functionData->locals = (hasLocals) ? &localTreei2f : NULL;
    if (paramCount > 0)
    {
        functionData->params = (varData *)malloc(paramCount * sizeof(varData));
        if (functionData->params == NULL)
        {
            printf("Memory allocation for parameters failed\n");
            exit(1);
        }
        for (int i = 0; i < paramCount; i++)
        {
            functionData->params[i] = varDatas[i]; // Kopírovanie údajov parametra
        }
    }else{
        functionData->params = NULL;
    }

    symtableInsertFce(symTree, *functionName, functionData);
}
void insertVariables(const char *name, DATATYPE dataType, bool initialized, bool constant, bool isPar, bool use, bstSymtable **local)
{
    DSTRING *variableName = dStringCreate();
    dStringAddString(variableName, name);

    varData *variableData = (varData *)malloc(sizeof(varData));
    if (variableData == NULL)
    {
        // Handle memory allocation failure
        printf("Memory allocation for fceData failed\n");
        exit(1); // Or handle it appropriately
    }

    // Initialize the fields
    variableData->constant = constant;
    variableData->dataType = dataType;
    variableData->initialized = initialized;
    variableData->isPar = isPar;
    variableData->use = use;

    symtableInsertVar(local, *variableName, variableData);
}

void symtableInsertBuildInFce(bstSymtable **symTree)
{
    // Functions for loading values
    // readstr
    insertFunction(symTree, "readstr", (DATATYPE){true, false, T_STR}, 0, true, true, false, NULL);
    // readi32
    insertFunction(symTree, "readi32", (DATATYPE){true, false, T_INT}, 0, true, true, false, NULL);
    // readf64
    insertFunction(symTree, "readf64", (DATATYPE){true, false, T_FLOAT}, 0, true, true, false, NULL);

    // Function for printing out values
    // the dataType of term in write(term) doesnt matter
    varData varDatas[5];
    varDatas[0].dataType = (DATATYPE){true, false, T_KEYWORD};
    insertFunction(symTree, "write", (DATATYPE){false, true, T_KEYWORD}, 1, true, true, false, varDatas);

    // Functions for converting number values
    varDatas[0].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "i2f", (DATATYPE){false, false, T_FLOAT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_FLOAT};
    insertFunction(symTree, "f2i", (DATATYPE){false, false, T_INT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_KEYWORD};
    // Functions for string operations
    insertFunction(symTree, "string", (DATATYPE){false, false, T_STR}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "length", (DATATYPE){false, false, T_INT}, 1, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "concat", (DATATYPE){false, false, T_STR}, 3, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_INT};
    varDatas[2].dataType = (DATATYPE){false, false, T_INT};
    insertFunction(symTree, "substring", (DATATYPE){true, false, T_STR}, 3, true, true, false, varDatas);
    varDatas[0].dataType = (DATATYPE){false, false, T_STR};
    varDatas[1].dataType = (DATATYPE){false, false, T_STR};
    insertFunction(symTree, "strcmp", (DATATYPE){false, false, T_STR}, 2, true, true, false, varDatas);
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

void printTree(bstSymtable *root, int level, const char *prefix)
{
    if (root == NULL)
    {
        return;
    }

    // Print the current node with its key and height
    printf("%s%s[%s,%d]\n", prefix, (level > 0) ? "+" : "-", root->key, root->height);

    // Prepare new prefix for child nodes
    char newPrefix[100];
    if (level > 0)
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s  ", prefix); // Indentation for child nodes
    }
    else
    {
        snprintf(newPrefix, sizeof(newPrefix), "%s| ", prefix); // For root node
    }

    // Recursively print the left subtree
    if (root->left != NULL)
    {
        printTree(root->left, level + 1, newPrefix);
    }
    // Recursively print the right subtree
    if (root->right != NULL)
    {
        printTree(root->right, level + 1, newPrefix);
    }
}
// funguje
int main()
{
    bstSymtable *symTree;
    symtableInit(&symTree);
    DSTRING *readstr = dStringCreate();
    dStringAddString(readstr, "chr");
    symtableInsertBuildInFce(&symTree);
    bstSymtable *result = symtableSearch(&symTree, *readstr);
    printTree(symTree, 0, "");
    printf("paramType:%d\n", ((fceData*)result->data)->params[0].dataType.type);
    printf("ret:%d\n", ((fceData*)result->data)->returnType.type);

    bstSymtable *localTree;
    symtableInit(&localTree);
    insertVariables("ahoj", (DATATYPE){false, false, T_STR}, true, true, true, true, &localTree);
    insertVariables("cau", (DATATYPE){false, false, T_INT}, true, true, true, true, &localTree);
    insertVariables("pa", (DATATYPE){false, false, T_STR}, true, true, true, true, &localTree);
    insertVariables("kar", (DATATYPE){true, false, T_FLOAT}, true, true, true, true, &localTree);
    // vlozil som do funkcie lokalny strom
    printTree(localTree, 0, "");
    ((fceData *)result->data)->locals = &localTree;

    DSTRING *str = dStringCreate();
    dStringAddString(str, "kar");

    if (((fceData *)result->data)->locals != NULL)
    {
        printf("Found \n");
        printf("Address stored in: %p\n", (void *)((fceData *)result->data)->locals);

        // Skontrolujte, či sa správne inicializovalo
        if (((fceData *)result->data)->locals != NULL)
        {
            printf("Local tree is initialized%d\n", ((fceData *)result->data)->returnType.type);
        }

        bstSymtable *resLocal = symtableSearch(((fceData *)result->data)->locals, *str);
        if (resLocal != NULL)
        {
            printf("Found in locals %d\n", ((varData*)resLocal->data)->dataType.type);
        }
        else
        {
            printf("Not found in locals\n");
        }
    }
    else
    {
        printf("Not Found \n");
    }
    //  Create test variables
    //  varData var1 = {1, true, false, false, true}; // Variable 'a'
    //  varData var2 = {2, true, true, false, true};  // Variable 'b'

    // // Create DSTRING keys for the variables
    // TOKEN *token = createToken();
    // token->type = T_ID;
    // token->current_attribute = DSTR;
    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'a'); // Variable 'a'

    // // Insert the variable into the symbol table
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var1);
    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'b'); // Variable 'b'
    // // Insert the second variable into the symbol table
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'c'); // Variable 'b'
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'd'); // Variable 'b'
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 1); // Variable 'b'
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'g'); // Variable 'b'
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'h'); // Variable 'b'
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // token->attribute.dStr = dStringCreate();
    // dStringAddString(token->attribute.dStr, "ifj");
    // symtableInsertVar(&symTree, *token->attribute.dStr, &var2);

    // printTree(symTree, 0, "");
    // // Delete the root node (variable 'a')
    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'c');
    // symtableDelete(&symTree, *token->attribute.dStr);
    // // Search for 'a' (should not be found)
    // bstSymtable *result = symtableSearch(&symTree, *token->attribute.dStr);
    // if (result != NULL)
    // {
    //     printf("Variable 'a' found after deletion.\n");
    // }
    // else
    // {
    //     printf("Variable 'a' successfully deleted.\n");
    // }

    // // Search for 'b' (should be found as it replaces 'a')
    // token->attribute.dStr = dStringCreate();
    // dStringAddChar(token->attribute.dStr, 'b'); // Variable 'b'
    // result = symtableSearch(&symTree, *token->attribute.dStr);
    // if (result != NULL)
    // {
    //     printf("Variable 'b' found after deletion of 'a'.\n");
    // }
    // else
    // {
    //     printf("Variable 'b' not found after deletion.\n");
    // }
    // pristup k lokalnym datam fce
    // varData *cau = ((fceData*)(symTree)->data)->locals;
    // cau->dataType;
    // ((varData*)((fceData*)(symTree)->data)->locals)->dataType;
    return 0;
}