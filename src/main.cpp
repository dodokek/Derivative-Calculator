// #include "TXLib.h"
#include "backend.h"
#include "frontend.h"

int main()
{


    TreeNode* root = GetG();

    DrawTree (root);


    InitLatexFile (root);
    
    TreeNode* d_root = GetDerivative (root, TEX_OUT);
    DrawTree (d_root);
    PrintBranch (d_root, RESULT);

    PrintMacloren (root, 3);
    
    GeneratePdf ();


    DestructTree (root);
}