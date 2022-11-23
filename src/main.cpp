#include "backend.h"
#include "frontend.h"

int main()
{

    TreeNode* root = GetTreeRoot();
    DrawTree (root);
    
    InitLatexFile (root);
    
    TreeNode* d_root = GetDerivative (root, root);
    DrawTree (d_root);

    
    GeneratePdf ();


    DestructTree (root);
}