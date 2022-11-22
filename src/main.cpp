#include "backend.h"
#include "frontend.h"

int main()
{
    TreeNode* root = GetTreeRoot();

    TreeNode* d_root = GetDerivative (root);

    DrawTree (root);

    DrawTree (d_root);
    while(SimplifyTree(d_root)) ;
    DrawTree (d_root);

    PrintInFile (d_root);

    DestructTree (root);
}