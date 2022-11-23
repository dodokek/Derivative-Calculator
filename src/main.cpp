#include "backend.h"
#include "frontend.h"

int main()
{

    TreeNode* root = GetTreeRoot();
    
    InitLatexFile (root);
    
    TreeNode* d_root = GetDerivative (root, root);

    DrawTree (root);
    DrawTree (d_root);
    PrintBranch (d_root);    
    while(SimplifyTree(d_root)) ;
    
    DrawTree (d_root);

    GeneratePdf ();

    DestructTree (root);
}