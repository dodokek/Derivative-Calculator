#include "backend.h"
#include "frontend.h"

int main()
{
    TreeNode* root = GetTreeRoot();

    TreeNode* d_root = GetDerivative (root);

    DrawTree (root);
    
    SimplifyTree (root);

    DrawTree (root);

    PrintInFile (root);


    DestructTree (root);
}